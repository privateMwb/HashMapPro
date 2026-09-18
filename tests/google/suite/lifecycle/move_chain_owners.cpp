// HashMap Move Chain Test Suite
// Verifies ownership transfers correctly across a chain of multiple
// move operations, through both move construction and move assignment.
//
// Covers:
// - ownership transferred through a chain of move constructions
// - ownership transferred through a chain of move assignments
// - a map moved into a function parameter and moved back out
// - every intermediate owner in a move chain is left empty

#include <HashMapPro/HashMap.h>

#include <gtest/gtest.h>

#include <string>
#include <utility>

// Moves a map into a function by value and returns it, exercising a
// move-in / move-out round trip through a function boundary.
static HashMapPro::HashMap<int, std::string>
passThrough(HashMapPro::HashMap<int, std::string> map) {
    map.insert(999, "added inside");
    return map;
}

// Verifies ownership is correctly transferred through a chain of
// move constructions, leaving every intermediate owner empty.
TEST(MoveChainOwners, MoveConstructionChain) {
    HashMapPro::HashMap<int, std::string> a{8};
    a.insert(1, "one");
    a.insert(2, "two");

    HashMapPro::HashMap<int, std::string> b{std::move(a)};
    HashMapPro::HashMap<int, std::string> c{std::move(b)};
    HashMapPro::HashMap<int, std::string> d{std::move(c)};

    EXPECT_EQ(d.size(), 2u);
    EXPECT_EQ(d.at(1), "one");
    EXPECT_EQ(d.at(2), "two");

    // NOLINTBEGIN(clang-analyzer-cplusplus.Move)
    // HashMap guarantees moved-from containers become empty.
    EXPECT_EQ(a.size(), 0u);
    EXPECT_EQ(b.size(), 0u);
    EXPECT_EQ(c.size(), 0u);
    // NOLINTEND(clang-analyzer-cplusplus.Move)
}

// Verifies ownership is correctly transferred through a chain of
// move assignments into already-constructed maps.
TEST(MoveChainOwners, MoveAssignmentChain) {
    HashMapPro::HashMap<int, std::string> a{8};
    a.insert(1, "one");

    HashMapPro::HashMap<int, std::string> b{4};
    b.insert(99, "stale-b");

    HashMapPro::HashMap<int, std::string> c{4};
    c.insert(98, "stale-c");

    b = std::move(a);
    c = std::move(b);

    EXPECT_EQ(c.size(), 1u);
    EXPECT_EQ(c.at(1), "one");
    EXPECT_FALSE(c.contains(98));

    // NOLINTBEGIN(clang-analyzer-cplusplus.Move)
    // HashMap guarantees moved-from containers become empty.
    EXPECT_EQ(a.size(), 0u);
    EXPECT_EQ(b.size(), 0u);
    // NOLINTEND(clang-analyzer-cplusplus.Move)
}

// Verifies a map survives being moved into a function parameter and
// moved back out through the return value.
TEST(MoveChainOwners, MoveThroughFunctionBoundary) {
    HashMapPro::HashMap<int, std::string> source{8};
    source.insert(1, "one");

    HashMapPro::HashMap<int, std::string> result = passThrough(std::move(source));

    EXPECT_EQ(result.size(), 2u);
    EXPECT_EQ(result.at(1), "one");
    EXPECT_EQ(result.at(999), "added inside");

    // NOLINTBEGIN(clang-analyzer-cplusplus.Move)
    // HashMap guarantees moved-from containers become empty.
    EXPECT_EQ(source.size(), 0u);
    // NOLINTEND(clang-analyzer-cplusplus.Move)
}
