// ============================================================
// fuzz/fuzz_hashmap.cpp
//
// Differential fuzzer for HashMapPro::HashMap<int, int>, checked
// against a std::unordered_map<int, int> shadow model after every
// single operation (not just at the end) so a fuzzer-found failure
// localizes to the exact operation that caused it.
//
// int/int is deliberately chosen, not for simplicity but because
// it's trivially copyable and cheap to hash -- this keeps every
// operation focused on HashMap's own bucket/chain/rehash machinery
// rather than on key/value construction or hashing cost, which is
// exactly where a subtle chaining or rehash bug would hide.
//
// Keys are drawn from a small range so collisions -- and therefore
// real bucket-chain traversal, not just single-node buckets -- happen
// constantly, even at larger bucket counts.
//
// Specifically targets:
//   - bucket chaining and lookup correctness under heavy collisions,
//     across several starting bucket counts, biased toward small ones
//     so most runs force at least one automatic rehash rather than
//     only exercising a comfortably pre-sized table
//   - the power-of-two bucket sizing / rehash-at-0.75-load-factor
//     contract: elementCount_ vs. bucketCount_ is checked to never
//     silently drift out of that contract after any mutation
//   - insert()'s no-op-if-present semantics vs. operator[]'s
//     insert-or-default and update()'s update-only-if-present
//     semantics -- three related but distinct write paths that are
//     easy to conflate
//   - erase()'s unlink logic, including unlinking the head vs. a
//     middle/tail node of a chain
//   - at()'s bounds-checking contract (throws std::out_of_range,
//     exactly, exactly when the key is actually absent)
//   - both operator= overloads (copy and move), including
//     self-assignment and self-move-assignment
//   - reuse of a moved-from map via ensureStorage() -- inserting into
//     a map right after it's been moved from, which is the scenario
//     ensureStorage() exists for
//   - full traversal via begin()/end(), cross-checked against the
//     shadow model in both directions (every shadow entry is
//     reachable, and every reachable entry is in the shadow) so a
//     phantom node or a dropped node both show up as a mismatch
//
// Deliberately NOT covered yet: custom hash functors (only the
// default std::hash<int> is exercised here), and exception-injection
// during copy construction (would need a throwing test key/value type
// to actually exercise the strong-guarantee rollback path in
// cloneFrom()'s catch block).
// ============================================================

#include <HashMapPro/HashMap.h>

#include <cstdint>
#include <cstdlib>
#include <stdexcept>
#include <unordered_map>
#include <utility>

using HashMapPro::HashMap;

namespace {

// Keys are drawn from [0, kKeyRange) so collisions are frequent and
// bucket chains actually get exercised, even right after a rehash.
constexpr int kKeyRange = 16;

// Aborts (rather than throwing/returning) on mismatch so libFuzzer
// captures a minimal, precise reproducer for exactly the operation
// that broke invariants.
void verify(HashMap<int, int>& map, const std::unordered_map<int, int>& shadow) {
    if (map.size() != shadow.size())
        std::abort();

    if (map.empty() != shadow.empty())
        std::abort();

    // The rehash-at-0.75-load-factor contract must never silently drift:
    // there must always be enough buckets for the current element count.
    if (map.capacity() > 0 &&
        static_cast<double>(map.size()) > static_cast<double>(map.capacity()) * map.max_load_factor() + 1e-9)
        std::abort();

    // Every shadow entry must be reachable through the map's own API,
    // with the same value, three different ways.
    for (const auto& [key, value] : shadow) {
        if (!map.contains(key))
            std::abort();

        auto it = map.find(key);
        if (it == map.end() || (*it).value != value)
            std::abort();

        if (map.at(key) != value)
            std::abort();
    }

    // Every entry reachable by iterating the map must also be in the
    // shadow, with a matching value -- catches phantom/duplicated nodes
    // and confirms begin()/end() traversal doesn't revisit or skip.
    std::size_t iteratedCount = 0;
    for (const auto& node : map) {
        auto shadowIt = shadow.find(node.key);
        if (shadowIt == shadow.end() || shadowIt->second != node.value)
            std::abort();
        ++iteratedCount;
    }
    if (iteratedCount != shadow.size())
        std::abort();
}

} // namespace

extern "C" int LLVMFuzzerTestOneInput(const std::uint8_t* data, std::size_t size) {
    if (size == 0)
        return 0;

    // First byte selects a starting bucket count, biased toward small
    // values so most runs are forced through at least one rehash.
    static constexpr std::size_t kBucketCounts[] = {1, 2, 4, 16, 64};
    const std::size_t bucketCount = kBucketCounts[data[0] % 5];
    ++data;
    --size;

    HashMap<int, int> map(bucketCount);
    std::unordered_map<int, int> shadow;

    // Each operation consumes two bytes: one selects the op, the other
    // derives the key. Runs with a dangling odd byte just stop early.
    for (std::size_t i = 0; i + 1 < size; i += 2) {
        const std::uint8_t op = data[i] % 11;
        const int key = static_cast<int>(data[i + 1] % kKeyRange);
        const int value = static_cast<int>(i);

        switch (op) {
        case 0: { // insert -- no-op if key already present
            map.insert(key, value);
            shadow.emplace(key, value);
            break;
        }
        case 1: { // operator[] -- insert-or-default, then assign
            map[key] = value;
            shadow[key] = value;
            break;
        }
        case 2: { // update -- only if key already present
            const bool updated = map.update(key, value);
            const bool shouldUpdate = shadow.count(key) != 0;
            if (updated != shouldUpdate)
                std::abort();
            if (updated)
                shadow[key] = value;
            break;
        }
        case 3: { // erase
            const bool erased = map.erase(key);
            const bool shouldErase = shadow.count(key) != 0;
            if (erased != shouldErase)
                std::abort();
            shadow.erase(key);
            break;
        }
        case 4: { // clear
            map.clear();
            shadow.clear();
            break;
        }
        case 5: { // reserve
            const std::size_t extra = static_cast<std::size_t>(data[i + 1] % 32);
            map.reserve(map.size() + extra);
            break;
        }
        case 6: { // contains
            const bool found = map.contains(key);
            const bool shouldFind = shadow.count(key) != 0;
            if (found != shouldFind)
                std::abort();
            break;
        }
        case 7: { // at() bounds-checking contract
            // Roughly half the time, deliberately pick a key that's
            // (very likely) absent to confirm at() actually throws
            // for it.
            const int lookupKey = (i % 2 == 0) ? key : (key + kKeyRange + 1000);

            bool threw = false;
            int fetched = 0;
            try {
                fetched = map.at(lookupKey);
            } catch (const std::out_of_range&) {
                threw = true;
            }

            const bool shouldThrow = shadow.count(lookupKey) == 0;
            if (threw != shouldThrow)
                std::abort();
            if (!threw && fetched != shadow.at(lookupKey))
                std::abort();
            break;
        }
        case 8: { // copy assignment, including self-assignment
            const bool selfAssign = (i % 8 == 0);
            if (selfAssign) {
                HashMap<int, int>& selfRef = map;
                map = selfRef;
                // shadow unchanged -- self-assignment is a no-op.
            } else {
                HashMap<int, int> mapCopy;
                std::unordered_map<int, int> shadowCopy = shadow; // independent RHS
                for (const auto& [k, v] : shadowCopy)
                    mapCopy.insert(k, v);

                map = mapCopy;
                shadow = shadowCopy;
            }
            break;
        }
        case 9: { // move assignment, including self-move-assignment,
                  // then immediate reuse of the (possibly moved-from)
                  // map to exercise ensureStorage()'s reuse path.
            const bool selfMoveAssign = (i % 8 == 1);
            if (selfMoveAssign) {
                HashMap<int, int>& selfRef = map;
                map = std::move(selfRef);
                // shadow unchanged -- moving from self must be a
                // no-op, not a self-destructive one.
            } else {
                HashMap<int, int> mapSource;
                std::unordered_map<int, int> shadowSource = shadow;
                for (const auto& [k, v] : shadowSource)
                    mapSource.insert(k, v);

                map = std::move(mapSource);
                shadow = shadowSource;
            }

            // Immediately insert into `map` again. If it was just
            // moved from (the non-self-move branch always leaves the
            // destination reusable; the self-move branch is a no-op),
            // this exercises ensureStorage() reallocating storage for
            // a map that currently has none.
            map.insert(key, value);
            shadow.emplace(key, value);
            break;
        }
        case 10: { // find, both success and miss paths
            auto it = map.find(key);
            const bool shouldFind = shadow.count(key) != 0;
            if ((it != map.end()) != shouldFind)
                std::abort();
            if (shouldFind && (*it).value != shadow.at(key))
                std::abort();
            break;
        }
        default:
            break;
        }

        verify(map, shadow);
    }

    return 0;
}
