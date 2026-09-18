// HashMap Power-of-Two Rounding Regression Test Suite
// Verifies bucket counts are always normalized to a power of two,
// across construction, reserve(), and automatic rehash growth.
//
// Covers:
// - a bucket count that is already a power of two is left unchanged
// - a non-power-of-two bucket count is rounded up to the next one
// - a bucket count of 1 stays at 1
// - reserve()'s computed target is rounded up to a power of two
// - capacity remains a power of two after rehash-triggered growth

#include <HashMapPro/HashMap.h>

#include <gtest/gtest.h>

#include <string>

// Verifies a bucket count that is already a power of two is left
// unchanged by construction.
TEST(PowerOfTwoRounding, AlreadyPowerOfTwoUnchanged) {
    HashMapPro::HashMap<int, std::string> map{64};

    EXPECT_EQ(map.capacity(), 64u);
}

// Verifies a non-power-of-two bucket count is rounded up to the
// next power of two.
TEST(PowerOfTwoRounding, NonPowerOfTwoRoundsUp) {
    HashMapPro::HashMap<int, std::string> a{17};
    EXPECT_EQ(a.capacity(), 32u);

    HashMapPro::HashMap<int, std::string> b{33};
    EXPECT_EQ(b.capacity(), 64u);

    HashMapPro::HashMap<int, std::string> c{100};
    EXPECT_EQ(c.capacity(), 128u);
}

// Verifies requesting a single bucket keeps the capacity at 1.
TEST(PowerOfTwoRounding, SingleBucketStaysAtOne) {
    HashMapPro::HashMap<int, std::string> map{1};

    EXPECT_EQ(map.capacity(), 1u);
}

// Verifies reserve()'s internally computed bucket count is rounded
// up to a power of two, not left at whatever raw value was computed.
TEST(PowerOfTwoRounding, ReserveTargetRoundsUp) {
    HashMapPro::HashMap<int, std::string> map{4};

    map.reserve(10);

    std::size_t capacity = map.capacity();

    EXPECT_EQ(capacity & (capacity - 1), 0u);
    EXPECT_GE(capacity, 10u);
}

// Verifies capacity remains a power of two after growth triggered by
// crossing the load factor threshold during insertion.
TEST(PowerOfTwoRounding, CapacityStaysPowerOfTwoAfterGrowth) {
    HashMapPro::HashMap<int, int> map{4};

    for (int i = 0; i < 50; ++i) {
        map.insert(i, i);

        std::size_t capacity = map.capacity();
        EXPECT_EQ(capacity & (capacity - 1), 0u);
    }
}
