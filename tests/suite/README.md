# Test Suite

This document describes the test categories under `suite/` — what each
one verifies, and the individual test files it contains.

Unlike the benchmark suite, tests validate the library's own
correctness directly — there is no reference implementation to compare
against, so results are simply pass or fail.

Every test suite registers itself automatically via
`REGISTER_TEST_SUITE()` at startup, and is assigned a sequential id
within its category (e.g. `U1`, `U2` for Unit; `L1`, `L2` for
Lifecycle) — there's no suite list to maintain by hand. This applies
uniformly across every category below.

---

## Concurrency

Verifies thread-safety — concurrent reads, writes serialized through
an external mutex, and correctness when each thread owns its own
independent instance — since HashMap provides no internal
synchronization of its own.

### Tests

- `concurrent_read_only.cpp` — Concurrent find()/contains()/at()/const
  iteration from multiple threads on an already-populated map all
  agree, and size is unchanged afterward
- `mutex_guarded_writes.cpp` — Concurrent insert, update, erase, and a
  mix of insert/erase across threads are all correct when serialized
  through an external mutex
- `thread_local_instances.cpp` — Independent per-thread map instances
  compute correctly with no cross-thread interference, including many
  short-lived thread launches and mid-thread destruction

---

## Integration

Verifies multiple components working together end-to-end — cache-like
usage, a custom key/hash workflow, combined insert/erase/iterate
sequences, reserve() interacting with later mutation, and iteration
around automatic rehashing.

### Tests

- `cache_usage_pattern.cpp` — Miss falls back to a default, hit/
  overwrite via operator[], clear-then-reuse, and a get-or-compute
  pattern that only computes once per key
- `custom_hash_workflow.cpp` — Insert, lookup, update, erase,
  iteration, copy, and rehash all correct together on a custom key
  type with a custom hash functor
- `insert_erase_iterate.cpp` — Iteration stays consistent with size()
  through a mix of insert, erase, find, and clear
- `rehash_during_iteration.cpp` — An outstanding iterator survives a
  non-rehashing insert, and data/fresh-iteration/traversal all stay
  correct across an insert that does trigger a rehash
- `reserve_then_update.cpp` — reserve() combined with bulk insert,
  bulk update, erase/reinsert, and iteration all remain consistent

---

## Lifecycle

Verifies object lifetime operations — construction, destruction,
copying, and moving — including nested ownership and exception safety
during cloning.

### Tests

- `construct_destroy_cycle.cpp` — Repeated construct/destroy cycles,
  nested and sibling scopes, and a vector of maps all tear down
  cleanly without residual state
- `exception_safe_clone.cpp` — A throwing value type doesn't leak
  resources or corrupt state when copy construction or copy assignment
  fails partway through cloning
- `move_chain_owners.cpp` — Ownership transfers correctly through a
  chain of move constructions, a chain of move assignments, and a
  move-in/move-out round trip through a function boundary
- `nested_map_lifetime.cpp` — A HashMap of HashMaps default-constructs,
  populates, mutates, clears, and deep-copies its inner maps correctly
- `repeated_move_reuse.cpp` — A map is safely reusable immediately
  after being moved from, across many cycles, alternating as source
  and destination, and after a rehash

---

## Regression

Verifies that a specific, previously fixed bug stays fixed. One test
per resolved issue, added at the time the fix lands.

### Tests

- `erase_last_node.cpp` — erase() correctly relinks a bucket chain
  regardless of whether the erased node is the head, tail, middle, or
  the only/last remaining element
- `hash_collision_chain.cpp` — Insert, lookup, update, iteration,
  rehash, and erase all stay correct on a pathological single-bucket
  collision chain
- `power_of_two_rounding.cpp` — Bucket counts are always normalized to
  a power of two across construction, reserve(), and rehash-triggered
  growth
- `rehash_threshold_boundary.cpp` — Automatic rehashing triggers on
  exactly the insertion that pushes load factor past 0.75, not early
  or late
- `sparse_bucket_iteration.cpp` — Forward and reverse iteration
  correctly skip leading, interior, and trailing empty buckets
- `zero_bucket_construction.cpp` — A requested bucket count of 0 is
  clamped to capacity 1 rather than producing a broken map

---

## Unit

Verifies individual functions or methods in isolation — the smallest
testable unit of behavior, independent of the categories above.

### Tests

- `access.cpp` — operator[] insert/existing/mutation, at() and const
  at() on existing and missing keys
- `capacity.cpp` — empty(), size() tracking across insert/erase/
  duplicate insert, capacity() at construction, growth past the load
  factor, and no shrink after erase
- `constructor.cpp` — Construction with a given/zero/default bucket
  count, initial empty state, copy/move construction and assignment,
  and self-assignment
- `iterator.cpp` — begin()==end() on an empty map, forward/const/
  reverse/const-reverse iteration, mutation through range-for, and
  pre-/post-increment consistency
- `lookup.cpp` — find() and const find() on existing/missing keys,
  mutation through find()'s returned iterator, and contains()
  including after erase
- `modifiers.cpp` — insert() on a new/existing key, update() on an
  existing/missing key, erase() on an existing/missing key, clear()
  and reinsertion, and a rehash-triggering insert
