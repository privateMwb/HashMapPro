// HashMap Insert/Erase/Iterate Workflow Test Suite
// Verifies insertion, erasure, and iteration remain consistent with
// one another across a combined workflow, rather than testing each
// operation in isolation.
//
// Covers:
// - iteration reflects exactly the elements currently inserted
// - erasing mid-iteration-cycle keeps remaining elements consistent
// - iteration count matches size() after a mix of insert and erase
// - interleaved insert/erase/find operations agree on map state
// - clearing after iteration leaves the map in a fresh, iterable state

#include <HashMapPro/HashMap.h>

#include <gtest/gtest.h>

#include <string>
#include <unordered_set>

// Verifies iteration visits exactly the elements currently present,
// no more and no fewer.
TEST(InsertEraseIterate, IterationReflectsCurrentElements) {
    HashMapPro::HashMap<int, std::string> map{8};
    map.insert(1, "one");
    map.insert(2, "two");
    map.insert(3, "three");

    std::unordered_set<int> seen;

    for (const auto& node : map) {
        seen.insert(node.key);
    }

    EXPECT_EQ(seen.size(), 3u);
    EXPECT_EQ(seen.count(1), 1u);
    EXPECT_EQ(seen.count(2), 1u);
    EXPECT_EQ(seen.count(3), 1u);
}

// Verifies that erasing an element, then re-iterating, shows the
// remaining elements only, in a self-consistent state.
TEST(InsertEraseIterate, EraseThenIterateIsConsistent) {
    HashMapPro::HashMap<int, std::string> map{8};
    map.insert(1, "one");
    map.insert(2, "two");
    map.insert(3, "three");

    bool erased = map.erase(2);
    EXPECT_TRUE(erased);

    std::unordered_set<int> seen;

    for (const auto& node : map) {
        seen.insert(node.key);
    }

    EXPECT_EQ(seen.size(), 2u);
    EXPECT_EQ(seen.count(1), 1u);
    EXPECT_EQ(seen.count(2), 0u);
    EXPECT_EQ(seen.count(3), 1u);
}

// Verifies the number of elements visited by iteration always
// matches size() after a mix of insert and erase operations.
TEST(InsertEraseIterate, IterationCountMatchesSize) {
    HashMapPro::HashMap<int, int> map{4};

    for (int i = 0; i < 10; ++i) {
        map.insert(i, i);
    }

    for (int i = 0; i < 10; i += 2) {
        (void)map.erase(i);
    }

    std::size_t counted = 0;

    for (auto it = map.begin(); it != map.end(); ++it) {
        ++counted;
    }

    EXPECT_EQ(counted, map.size());
    EXPECT_EQ(map.size(), 5u);
}

// Verifies interleaved insert, erase, and find operations all agree
// with each other about the map's current state.
TEST(InsertEraseIterate, InterleavedInsertEraseFind) {
    HashMapPro::HashMap<int, std::string> map{8};

    map.insert(1, "one");
    EXPECT_NE(map.find(1), map.end());

    map.insert(2, "two");
    (void)map.erase(1);

    EXPECT_EQ(map.find(1), map.end());
    EXPECT_NE(map.find(2), map.end());

    map.insert(1, "one-again");

    EXPECT_NE(map.find(1), map.end());
    EXPECT_EQ(map.at(1), "one-again");
    EXPECT_EQ(map.size(), 2u);
}

// Verifies clearing a map after iterating over it leaves it in a
// fresh state that can be populated and iterated again.
TEST(InsertEraseIterate, ClearAfterIterationAllowsReuse) {
    HashMapPro::HashMap<int, std::string> map{8};
    map.insert(1, "one");
    map.insert(2, "two");

    std::size_t firstPassCount = 0;

    for (auto it = map.begin(); it != map.end(); ++it) {
        ++firstPassCount;
    }

    EXPECT_EQ(firstPassCount, 2u);

    map.clear();
    EXPECT_EQ(map.begin(), map.end());

    map.insert(3, "three");

    std::size_t secondPassCount = 0;

    for (auto it = map.begin(); it != map.end(); ++it) {
        ++secondPassCount;
    }

    EXPECT_EQ(secondPassCount, 1u);
}
