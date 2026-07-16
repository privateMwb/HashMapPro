// HashMap Hash Collision Chain Regression Test Suite
// Verifies HashMap remains fully correct in the pathological case
// where every key collides into a single bucket, forcing a long
// linear chain and exercising the O(chain length) fallback path.
//
// Covers:
// - all colliding keys can be inserted and are individually findable
// - lookup on a long chain returns the correct value, not just any
// - update correctly targets one specific key within a long chain
// - iteration visits every element of a single long chain exactly once
// - a long chain still triggers rehashing based on element count,
//   not bucket occupancy
// - erasing every element one by one from a long chain empties it
//   without corrupting the remaining links

#include <common/framework.h>

#include <cstddef>
#include <string>
#include <unordered_set>

// Forces every key into bucket 0, regardless of capacity, producing
// a worst-case single-bucket chain.
struct AllSameHash {
    std::size_t operator()(int) const {
        return 0;
    }
};

using CollidingMap = HashMapPro::HashMap<int, std::string, AllSameHash>;

// Verifies every key in a long collision chain can be inserted and
// is individually findable afterward.
static void all_colliding_keys_insertable_and_findable() {
    CollidingMap map{8};

    for (int i = 0; i < 50; ++i) {
        map.insert(i, "value-" + std::to_string(i));
    }

    CHK(map.size() == 50);

    for (int i = 0; i < 50; ++i) {
        CHK(map.contains(i) == true);
    }
}

// Verifies lookup within a long chain returns the value belonging to
// the exact requested key, not a neighboring one.
static void lookup_returns_correct_value_in_long_chain() {
    CollidingMap map{8};

    for (int i = 0; i < 50; ++i) {
        map.insert(i, "value-" + std::to_string(i));
    }

    CHK(map.at(0) == "value-0");
    CHK(map.at(25) == "value-25");
    CHK(map.at(49) == "value-49");
}

// Verifies update correctly targets exactly one key within a long
// collision chain, leaving all others untouched.
static void update_targets_correct_key_in_long_chain() {
    CollidingMap map{8};

    for (int i = 0; i < 50; ++i) {
        map.insert(i, "original");
    }

    bool updated = map.update(25, "changed");

    CHK(updated == true);

    for (int i = 0; i < 50; ++i) {
        if (i == 25) {
            CHK(map.at(i) == "changed");
        } else {
            CHK(map.at(i) == "original");
        }
    }
}

// Verifies iteration over a single long chain visits every element
// exactly once.
static void iteration_visits_every_element_of_chain() {
    CollidingMap map{8};

    for (int i = 0; i < 50; ++i) {
        map.insert(i, "value");
    }

    std::unordered_set<int> seen;

    for (const auto& node : map) {
        seen.insert(node.key);
    }

    CHK(seen.size() == 50);
}

// Verifies a long collision chain still triggers rehashing based on
// element count, since growth is governed by size() versus
// capacity(), not by how many distinct buckets are actually in use.
static void long_chain_still_triggers_rehash() {
    CollidingMap map{4};

    for (int i = 0; i < 10; ++i) {
        map.insert(i, "value");
    }

    CHK(map.capacity() > 4);
    CHK(map.size() == 10);
}

// Verifies erasing every element of a long chain one at a time
// leaves the map empty without corrupting the remaining links along
// the way.
static void erasing_entire_chain_empties_map() {
    CollidingMap map{8};

    for (int i = 0; i < 20; ++i) {
        map.insert(i, "value");
    }

    for (int i = 0; i < 20; ++i) {
        bool erased = map.erase(i);
        CHK(erased == true);
        CHK(map.size() == static_cast<std::size_t>(19 - i));
    }

    CHK(map.empty() == true);
}

// Executes all hash collision chain test cases.
static void run_tests() {
    RUN(all_colliding_keys_insertable_and_findable);
    RUN(lookup_returns_correct_value_in_long_chain);
    RUN(update_targets_correct_key_in_long_chain);
    RUN(iteration_visits_every_element_of_chain);
    RUN(long_chain_still_triggers_rehash);
    RUN(erasing_entire_chain_empties_map);
}

REGISTER_TEST_SUITE();
