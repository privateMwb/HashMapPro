// HashMap Custom Hash Workflow Test Suite
// Verifies a HashMap configured with a user-supplied key type and
// hash functor behaves correctly across a full combined workflow:
// insertion, lookup, update, erase, iteration, copying, and growth.
//
// Covers:
// - insertion and lookup with a custom key type and hash functor
// - update and erase operate correctly on custom keys
// - iteration visits every element keyed by a custom type
// - copying a map preserves custom-keyed elements independently
// - rehashing redistributes custom-keyed elements without data loss

#include <HashMapPro/HashMap.h>

#include <gtest/gtest.h>

#include <cstddef>
#include <functional>
#include <string>
#include <unordered_set>

// A 2D grid coordinate used as a HashMap key.
struct Point {
    int x;
    int y;

    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

// Hash functor for Point, combining the hashes of its two members.
struct PointHash {
    std::size_t operator()(const Point& p) const {
        std::size_t hx = std::hash<int>{}(p.x);
        std::size_t hy = std::hash<int>{}(p.y);

        return hx ^ (hy + 0x9e3779b9 + (hx << 6) + (hx >> 2));
    }
};

using PointMap = HashMapPro::HashMap<Point, std::string, PointHash>;

// Verifies insertion and lookup work correctly with a custom key
// type and hash functor.
TEST(CustomHashWorkflow, CustomKeyInsertAndLookup) {
    PointMap map{8};

    map.insert({0, 0}, "origin");
    map.insert({1, 1}, "diagonal");
    map.insert({-2, 5}, "off-axis");

    EXPECT_EQ(map.size(), 3u);
    EXPECT_TRUE(map.contains({0, 0}));
    EXPECT_FALSE(map.contains({9, 9}));
    EXPECT_EQ(map.at({1, 1}), "diagonal");
    EXPECT_EQ(map.at({-2, 5}), "off-axis");
}

// Verifies update and erase correctly target custom-keyed elements.
TEST(CustomHashWorkflow, CustomKeyUpdateAndErase) {
    PointMap map{8};
    map.insert({3, 4}, "point-a");
    map.insert({5, 6}, "point-b");

    bool updated = map.update({3, 4}, "point-a-updated");
    EXPECT_TRUE(updated);
    EXPECT_EQ(map.at({3, 4}), "point-a-updated");

    bool erased = map.erase({5, 6});
    EXPECT_TRUE(erased);
    EXPECT_FALSE(map.contains({5, 6}));
    EXPECT_EQ(map.size(), 1u);
}

// Verifies iteration visits every element keyed by a custom type
// exactly once.
TEST(CustomHashWorkflow, CustomKeyIterationVisitsAll) {
    PointMap map{8};
    map.insert({0, 0}, "a");
    map.insert({1, 0}, "b");
    map.insert({0, 1}, "c");

    std::unordered_set<std::string> seen;

    for (const auto& node : map) {
        seen.insert(node.value);
    }

    EXPECT_EQ(seen.size(), 3u);
    EXPECT_EQ(seen.count("a"), 1u);
    EXPECT_EQ(seen.count("b"), 1u);
    EXPECT_EQ(seen.count("c"), 1u);
}

// Verifies copying a map with custom-keyed elements produces an
// independent deep copy.
TEST(CustomHashWorkflow, CustomKeyCopyIsIndependent) {
    PointMap original{8};
    original.insert({1, 1}, "shared");

    PointMap copy = original;
    copy.insert({2, 2}, "copy-only");

    EXPECT_EQ(original.size(), 1u);
    EXPECT_EQ(copy.size(), 2u);
    EXPECT_FALSE(original.contains({2, 2}));
}

// Verifies rehashing redistributes custom-keyed elements without
// losing or corrupting any of them.
TEST(CustomHashWorkflow, CustomKeySurvivesRehash) {
    PointMap map{4};

    for (int i = 0; i < 20; ++i) {
        map.insert({i, -i}, "point-" + std::to_string(i));
    }

    EXPECT_GT(map.capacity(), 4u);
    EXPECT_EQ(map.size(), 20u);

    for (int i = 0; i < 20; ++i) {
        EXPECT_EQ(map.at({i, -i}), "point-" + std::to_string(i));
    }
}
