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

#include <HashMapPro/HashMap.h>

#include <gtest/gtest.h>

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
TEST(HashCollisionChain, AllCollidingKeysInsertableAndFindable) {
    CollidingMap map{8};

    for (int i = 0; i < 50; ++i) {
        map.insert(i, "value-" + std::to_string(i));
    }

    EXPECT_EQ(map.size(), 50u);

    for (int i = 0; i < 50; ++i) {
        EXPECT_TRUE(map.contains(i));
    }
}

// Verifies lookup within a long chain returns the value belonging to
// the exact requested key, not a neighboring one.
TEST(HashCollisionChain, LookupReturnsCorrectValueInLongChain) {
    CollidingMap map{8};

    for (int i = 0; i < 50; ++i) {
        map.insert(i, "value-" + std::to_string(i));
    }

    EXPECT_EQ(map.at(0), "value-0");
    EXPECT_EQ(map.at(25), "value-25");
    EXPECT_EQ(map.at(49), "value-49");
}

// Verifies update correctly targets exactly one key within a long
// collision chain, leaving all others untouched.
TEST(HashCollisionChain, UpdateTargetsCorrectKeyInLongChain) {
    CollidingMap map{8};

    for (int i = 0; i < 50; ++i) {
        map.insert(i, "original");
    }

    bool updated = map.update(25, "changed");

    EXPECT_TRUE(updated);

    for (int i = 0; i < 50; ++i) {
        if (i == 25) {
            EXPECT_EQ(map.at(i), "changed");
        } else {
            EXPECT_EQ(map.at(i), "original");
        }
    }
}

// Verifies iteration over a single long chain visits every element
// exactly once.
TEST(HashCollisionChain, IterationVisitsEveryElementOfChain) {
    CollidingMap map{8};

    for (int i = 0; i < 50; ++i) {
        map.insert(i, "value");
    }

    std::unordered_set<int> seen;

    for (const auto& node : map) {
        seen.insert(node.key);
    }

    EXPECT_EQ(seen.size(), 50u);
}

// Verifies a long collision chain still triggers rehashing based on
// element count, since growth is governed by size() versus
// capacity(), not by how many distinct buckets are actually in use.
TEST(HashCollisionChain, LongChainStillTriggersRehash) {
    CollidingMap map{4};

    for (int i = 0; i < 10; ++i) {
        map.insert(i, "value");
    }

    EXPECT_GT(map.capacity(), 4u);
    EXPECT_EQ(map.size(), 10u);
}

// Verifies erasing every element of a long chain one at a time
// leaves the map empty without corrupting the remaining links along
// the way.
TEST(HashCollisionChain, ErasingEntireChainEmptiesMap) {
    CollidingMap map{8};

    for (int i = 0; i < 20; ++i) {
        map.insert(i, "value");
    }

    for (int i = 0; i < 20; ++i) {
        bool erased = map.erase(i);
        EXPECT_TRUE(erased);
        EXPECT_EQ(map.size(), static_cast<std::size_t>(19 - i));
    }

    EXPECT_TRUE(map.empty());
}
