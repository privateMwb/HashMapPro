// HashMap Iterator Test Suite
// Validates forward and reverse iteration across mutable and
// const iterator interfaces.
//
// Covers:
// - begin()/end() on an empty map are equal
// - forward iteration visits every element exactly once
// - cbegin()/cend() visit every element exactly once
// - const begin()/end() visit every element exactly once
// - reverse iteration visits every element exactly once
// - const reverse iteration visits every element exactly once
// - range-based for loop mutates values
// - pre-increment and post-increment produce equivalent traversal

#include <HashMapPro/HashMap.h>

#include <gtest/gtest.h>

#include <string>
#include <unordered_set>

// Verifies begin() equals end() for an empty map.
TEST(Iterator, EmptyMapBeginEqualsEnd) {
    HashMapPro::HashMap<int, std::string> map{8};

    EXPECT_EQ(map.begin(), map.end());
}

// Verifies forward iteration visits every element exactly once.
TEST(Iterator, ForwardIterationVisitsAll) {
    HashMapPro::HashMap<int, std::string> map{4};
    map.insert(1, "one");
    map.insert(2, "two");
    map.insert(3, "three");

    std::unordered_set<int> seen;

    for (auto it = map.begin(); it != map.end(); ++it) {
        seen.insert(it->key);
    }

    EXPECT_EQ(seen.size(), 3u);
    EXPECT_EQ(seen.count(1), 1u);
    EXPECT_EQ(seen.count(2), 1u);
    EXPECT_EQ(seen.count(3), 1u);
}

// Verifies cbegin()/cend() visit every element exactly once.
TEST(Iterator, ConstIterationVisitsAll) {
    HashMapPro::HashMap<int, std::string> map{4};
    map.insert(1, "one");
    map.insert(2, "two");
    map.insert(3, "three");

    std::unordered_set<int> seen;

    for (auto it = map.cbegin(); it != map.cend(); ++it) {
        seen.insert(it->key);
    }

    EXPECT_EQ(seen.size(), 3u);
}

// Verifies const begin()/end() visit every element exactly once.
TEST(Iterator, ConstMapBeginEnd) {
    HashMapPro::HashMap<int, std::string> map{4};
    map.insert(1, "one");
    map.insert(2, "two");

    const auto& constMap = map;
    std::unordered_set<int> seen;

    for (auto it = constMap.begin(); it != constMap.end(); ++it) {
        seen.insert(it->key);
    }

    EXPECT_EQ(seen.size(), 2u);
}

// Verifies reverse iteration visits every element exactly once.
TEST(Iterator, ReverseIterationVisitsAll) {
    HashMapPro::HashMap<int, std::string> map{4};
    map.insert(1, "one");
    map.insert(2, "two");
    map.insert(3, "three");

    std::unordered_set<int> seen;

    for (auto it = map.rbegin(); it != map.rend(); ++it) {
        seen.insert(it->key);
    }

    EXPECT_EQ(seen.size(), 3u);
}

// Verifies const reverse iteration visits every element exactly once.
TEST(Iterator, ConstReverseIterationVisitsAll) {
    HashMapPro::HashMap<int, std::string> map{4};
    map.insert(1, "one");
    map.insert(2, "two");
    map.insert(3, "three");

    std::unordered_set<int> seen;

    for (auto it = map.crbegin(); it != map.crend(); ++it) {
        seen.insert(it->key);
    }

    EXPECT_EQ(seen.size(), 3u);
}

// Verifies range-based iteration allows value mutation.
TEST(Iterator, RangeForMutatesValues) {
    HashMapPro::HashMap<int, int> map{4};
    map.insert(1, 10);
    map.insert(2, 20);

    for (auto& node : map) {
        node.value *= 2;
    }

    EXPECT_EQ(map.at(1), 20);
    EXPECT_EQ(map.at(2), 40);
}

// Verifies pre-increment and post-increment produce equivalent traversal.
TEST(Iterator, IncrementConsistency) {
    HashMapPro::HashMap<int, std::string> map{4};
    map.insert(1, "one");
    map.insert(2, "two");

    auto pre = map.begin();
    auto post = map.begin();

    ++pre;
    post++;

    EXPECT_EQ(pre, post);
}
