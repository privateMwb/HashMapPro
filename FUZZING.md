# Fuzzing

HashMapPro is fuzzed via [ClusterFuzzLite](https://google.github.io/clusterfuzzlite/),
running on every pull request that touches the fuzzed files, plus a
longer scheduled batch run every night.

## What's covered

**`fuzz_hashmap.cpp`** is a differential fuzzer: it runs the same
sequence of operations against `HashMap<int, int>` and a
`std::unordered_map<int, int>` shadow model, comparing size and
contents after every single operation (not just at the end), so a
failing input localizes to the exact operation that broke an
invariant.

Keys are drawn from a small range (`[0, 16)`) rather than spread
across the full `int` domain, so collisions — and therefore real
bucket-chain traversal, not just single-node buckets — happen
constantly, even right after the table has just grown.

Specifically exercised:

- **Bucket chaining and lookup correctness under heavy collisions**,
  across several starting bucket counts, biased toward small ones so
  most runs force at least one automatic rehash rather than only
  exercising a comfortably pre-sized table.
- **The power-of-two bucket sizing / 0.75-load-factor rehash
  contract.** After every mutation, the harness checks that
  `size() / capacity()` never silently exceeds `max_load_factor()` —
  the invariant the whole automatic-rehash design rests on.
- **Three related but distinct write paths**, deliberately checked
  against each other rather than only against the shadow: `insert()`
  (no-op if the key is already present), `operator[]` (insert-or-default,
  then assign), and `update()` (updates only if the key is already
  present). A bug that quietly conflates any two of these would show
  up as a mismatch on the very next operation.
- **`erase()`'s unlink logic**, including unlinking the head node of a
  chain versus a middle or tail node — both happen naturally given the
  collision-heavy key range.
- **`at()`'s bounds-checking contract**: the harness deliberately picks
  a key that's (very likely) absent roughly half the time and confirms
  `std::out_of_range` is thrown exactly when, and only when, the key is
  actually absent.
- **Both `operator=` overloads (copy and move), including
  self-assignment and self-move-assignment.** Self-assignment/move are
  both required to be no-ops rather than self-destructive; the harness
  checks the shadow model doesn't change in that case.
- **Reuse of a moved-from map.** Right after a (non-self) move
  assignment, the harness inserts into the destination again — the
  exact scenario `ensureStorage()` exists to make safe, since a
  moved-from `HashMap` otherwise holds no bucket array at all.
- **Full traversal via `begin()`/`end()`**, cross-checked against the
  shadow model in both directions: every shadow entry must be
  reachable through the map's iterator, and every node the iterator
  visits must be in the shadow. This catches a dropped node and a
  phantom/duplicated node equally well, neither of which a
  size()-only check would notice.

Built and run under both AddressSanitizer and UndefinedBehaviorSanitizer.

## What's deliberately NOT covered yet

- **Custom hash functors.** Only the default `std::hash<int>` is
  exercised. A harness parameterized over a deliberately
  collision-heavy or adversarial custom `Hash` would be a natural
  follow-up, not a change to this one.
- **Exception injection during copy construction/assignment.** This
  harness never makes an operation throw, so it never actually
  exercises the strong-exception-guarantee rollback path in
  `cloneFrom()`'s catch block — only its non-throwing path. A throwing
  test key or value type (throws on the Nth construction/copy) is what
  a follow-up harness would need to actually stress that catch block.

## Running locally

```bash
git clone --recursive https://github.com/google/oss-fuzz.git
cd oss-fuzz
python infra/helper.py build_fuzzers --sanitizer address HashMapPro /path/to/HashMapPro
python infra/helper.py run_fuzzer HashMapPro fuzz_hashmap
```

Or, without OSS-Fuzz's tooling, directly with clang:

```bash
clang++ -std=c++20 -fsanitize=fuzzer,address \
  -Iinclude \
  fuzz/fuzz_hashmap.cpp \
  -o fuzz_hashmap

./fuzz_hashmap
```

Add `-fsanitize=fuzzer,undefined` instead to run under UBSan.

## Reproducing a crash

ClusterFuzzLite uploads the failing input as a workflow artifact when
a run fails. Download it, then:

```bash
./fuzz_hashmap path/to/crash-<hash>
```

This replays that exact byte sequence through
`LLVMFuzzerTestOneInput()` once, deterministically — no sanitizer flags
needed beyond however the binary was already built.

## Adding a new harness

1. Add `fuzz/fuzz_<target>.cpp` with an `extern "C" int
   LLVMFuzzerTestOneInput(const uint8_t*, size_t)` entry point.
2. Add the matching compile + link block to `.clusterfuzzlite/build.sh`.
3. No workflow changes needed — `cflite_pr.yml`/`cflite_batch.yml`
   build and run every binary `build.sh` produces in `$OUT`.
