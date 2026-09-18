// HashMap Zero-Bucket Construction Regression Test Suite
// Verifies constructing a map with an explicit bucket count of zero
// is clamped safely to a minimum usable capacity, rather than
// producing a broken or unusable map.
//
// Covers:
// - constructing with bucketCount == 0 clamps capacity to 1
// - a zero-bucket-constructed map starts empty
// - a zero-bucket-constructed map grows correctly on insertion
// - the default constructor uses the documented default of 16
// - reserve(0) on any map is a no-op

#include <HashMapPro/HashMap.h>

#include <gtest/gtest.h>

#include <string>

// Verifies a bucket count of 0 is clamped to the minimum capacity.
TEST(ZeroBucketConstruction, ZeroBucketCountClampsOne) {
    HashMapPro::HashMap<int, std::string> map{0};

    EXPECT_EQ(map.capacity(), 1u);
}

// Verifies a zero-bucket-constructed map starts in a valid, empty
// state.
TEST(ZeroBucketConstruction, ZeroBucketMapStartsEmpty) {
    HashMapPro::HashMap<int, std::string> map{0};

    EXPECT_EQ(map.size(), 0u);
    EXPECT_TRUE(map.empty());
}

// Verifies a zero-bucket-constructed map grows correctly once
// elements are inserted into it.
TEST(ZeroBucketConstruction, ZeroBucketMapGrowsInsert) {
    HashMapPro::HashMap<int, std::string> map{0};

    for (int i = 0; i < 10; ++i) {
        map.insert(i, "value");
    }

    EXPECT_EQ(map.size(), 10u);
    EXPECT_GT(map.capacity(), 1u);

    for (int i = 0; i < 10; ++i) {
        EXPECT_TRUE(map.contains(i));
    }
}

// Verifies the default constructor (no argument) uses the
// documented default bucket count of 16.
TEST(ZeroBucketConstruction, DefaultConstructorDocumentedDefault) {
    HashMapPro::HashMap<int, std::string> map;

    EXPECT_EQ(map.capacity(), 16u);
}

// Verifies reserve(0) is a no-op and does not alter capacity.
TEST(ZeroBucketConstruction, ReserveZeroIsNoOp) {
    HashMapPro::HashMap<int, std::string> map{8};

    map.reserve(0);

    EXPECT_EQ(map.capacity(), 8u);
}
