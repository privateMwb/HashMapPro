// HashMap Constructor Test Suite
// Verifies constructor, copy, and move behavior.
//
// Covers:
// - construction with a specified bucket count
// - zero bucket count is clamped to the minimum
// - default constructor uses the default bucket count
// - newly constructed map is empty
// - copy construction performs a deep copy
// - copy assignment performs a deep copy
// - self copy assignment preserves the existing state
// - move construction transfers ownership
// - move assignment transfers ownership
// - self move assignment preserves a valid state

#include <HashMapPro/HashMap.h>

#include <gtest/gtest.h>

#include <string>
#include <utility>

// Verifies construction with the requested bucket count.
TEST(Constructor, BasicConstruction) {
    HashMapPro::HashMap<int, int> map{16};

    EXPECT_EQ(map.capacity(), 16u);
    EXPECT_EQ(map.size(), 0u);
    EXPECT_TRUE(map.empty());
}

// Verifies zero buckets are clamped to the minimum capacity.
TEST(Constructor, ZeroBucketCount) {
    HashMapPro::HashMap<int, int> map{0};

    EXPECT_EQ(map.capacity(), 1u);
    EXPECT_EQ(map.size(), 0u);
}

// Verifies the default constructor uses the default bucket count.
TEST(Constructor, DefaultBucketCount) {
    HashMapPro::HashMap<int, int> map;

    EXPECT_EQ(map.capacity(), 16u);
    EXPECT_TRUE(map.empty());
}

// Verifies a newly constructed map is empty.
TEST(Constructor, InitialState) {
    HashMapPro::HashMap<int, std::string> map{8};

    EXPECT_FALSE(map.contains(1));
    EXPECT_EQ(map.find(1), map.end());
    EXPECT_EQ(map.begin(), map.end());
}

// Verifies copy construction performs a deep copy.
TEST(Constructor, CopyConstruction) {
    HashMapPro::HashMap<int, std::string> a{8};
    a.insert(1, "one");
    a.insert(2, "two");

    HashMapPro::HashMap<int, std::string> b{a};

    EXPECT_EQ(b.size(), 2u);
    EXPECT_EQ(b.at(1), "one");
    EXPECT_EQ(b.at(2), "two");
    EXPECT_EQ(a.size(), 2u);

    b.insert(3, "three");
    EXPECT_FALSE(a.contains(3));
}

// Verifies copy assignment replaces the previous state with a deep copy.
TEST(Constructor, CopyAssignment) {
    HashMapPro::HashMap<int, std::string> a{8};
    a.insert(1, "one");

    HashMapPro::HashMap<int, std::string> b{4};
    b.insert(99, "stale");

    b = a;

    EXPECT_EQ(b.size(), 1u);
    EXPECT_EQ(b.at(1), "one");
    EXPECT_FALSE(b.contains(99));
}

// Verifies self copy assignment preserves the existing state.
TEST(Constructor, CopyAssignmentSelf) {
    HashMapPro::HashMap<int, std::string> a{8};
    a.insert(1, "one");

    a = a;

    EXPECT_EQ(a.size(), 1u);
    EXPECT_EQ(a.at(1), "one");
}

// Verifies move construction transfers ownership to the destination.
TEST(Constructor, MoveConstruction) {
    HashMapPro::HashMap<int, std::string> a{8};
    a.insert(1, "one");

    HashMapPro::HashMap<int, std::string> b{std::move(a)};

    EXPECT_EQ(b.size(), 1u);
    EXPECT_EQ(b.at(1), "one");

    // NOLINTBEGIN(clang-analyzer-cplusplus.Move)
    EXPECT_EQ(a.size(), 0u);
    EXPECT_EQ(a.capacity(), 0u);
    // NOLINTEND(clang-analyzer-cplusplus.Move)
}

// Verifies move assignment transfers ownership to the destination.
TEST(Constructor, MoveAssignment) {
    HashMapPro::HashMap<int, std::string> a{8};
    a.insert(1, "one");

    HashMapPro::HashMap<int, std::string> b{4};
    b.insert(99, "stale");

    b = std::move(a);

    EXPECT_EQ(b.size(), 1u);
    EXPECT_EQ(b.at(1), "one");

    // NOLINTBEGIN(clang-analyzer-cplusplus.Move)
    EXPECT_EQ(a.size(), 0u);
    EXPECT_EQ(a.capacity(), 0u);
    // NOLINTEND(clang-analyzer-cplusplus.Move)
}

// Verifies self move assignment preserves a valid map state.
TEST(Constructor, SelfMoveAssignment) {
    HashMapPro::HashMap<int, std::string> a{8};
    a.insert(1, "one");

    a = std::move(a);

    EXPECT_EQ(a.size(), 1u);
    EXPECT_EQ(a.at(1), "one");
}
