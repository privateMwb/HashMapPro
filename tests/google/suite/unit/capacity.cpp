// HashMap Capacity Test Suite
// Verifies size tracking, capacity behavior, and load-factor-based resizing.
//
// Covers:
// - empty() returns true for a newly constructed map
// - empty() becomes false after insertion
// - empty() returns true after clear()
// - size() correctly tracks insertions and erasures
// - duplicate inserts do not increase size
// - capacity() matches the initial bucket count
// - capacity() grows when load factor threshold is exceeded
// - capacity() does not shrink after deletions

#include <HashMapPro/HashMap.h>

#include <gtest/gtest.h>

#include <string>

// Verifies empty() is true immediately after construction.
TEST(Capacity, EmptyOnConstruction) {
    HashMapPro::HashMap<int, std::string> map{8};

    EXPECT_TRUE(map.empty());
}

// Verifies empty() becomes false after inserting an element.
TEST(Capacity, NotEmptyAfterInsert) {
    HashMapPro::HashMap<int, std::string> map{8};
    map.insert(1, "one");

    EXPECT_FALSE(map.empty());
}

// Verifies empty() returns true after clearing all elements.
TEST(Capacity, EmptyAfterClear) {
    HashMapPro::HashMap<int, std::string> map{8};
    map.insert(1, "one");
    map.clear();

    EXPECT_TRUE(map.empty());
}

// Verifies size() correctly increases on insert and decreases on erase.
TEST(Capacity, SizeTracksInsertAndErase) {
    HashMapPro::HashMap<int, std::string> map{8};

    EXPECT_EQ(map.size(), 0u);

    map.insert(1, "one");
    map.insert(2, "two");

    EXPECT_EQ(map.size(), 2u);

    (void)map.erase(1);

    EXPECT_EQ(map.size(), 1u);
}

// Verifies size() remains unchanged when inserting a duplicate key.
TEST(Capacity, SizeUnchangedOnDuplicateInsert) {
    HashMapPro::HashMap<int, std::string> map{8};
    map.insert(1, "one");
    map.insert(1, "uno");

    EXPECT_EQ(map.size(), 1u);
}

// Verifies capacity() equals the initial bucket count provided at construction.
TEST(Capacity, CapacityReflectsInitialBucketCount) {
    HashMapPro::HashMap<int, std::string> map{32};

    EXPECT_EQ(map.capacity(), 32u);
}

// Verifies capacity() increases when load factor threshold is exceeded.
TEST(Capacity, CapacityGrowsPastLoadFactor) {
    HashMapPro::HashMap<int, int> map{4};

    for (int i = 0; i < 3; ++i) {
        map.insert(i, i);
    }

    EXPECT_EQ(map.capacity(), 4u);

    map.insert(3, 3);

    EXPECT_GT(map.capacity(), 4u);
}

// Verifies capacity() remains unchanged after erasing elements.
TEST(Capacity, CapacityDoesNotShrinkAfterErase) {
    HashMapPro::HashMap<int, std::string> map{8};
    map.insert(1, "one");
    map.insert(2, "two");

    std::size_t before = map.capacity();

    (void)map.erase(1);
    (void)map.erase(2);

    EXPECT_EQ(map.capacity(), before);
    EXPECT_TRUE(map.empty());
}
