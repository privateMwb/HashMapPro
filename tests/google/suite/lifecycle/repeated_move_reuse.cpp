// HashMap Repeated Move Reuse Test Suite
// Verifies a map can be moved from and reused repeatedly across many
// cycles without corruption, relying on its lazily-allocated storage.
//
// Covers:
// - a map is safely reusable immediately after being moved from
// - repeated move-out/reuse cycles each behave independently
// - a map alternately used as move source and move destination
// - reuse after move works correctly following a rehash

#include <HashMapPro/HashMap.h>

#include <gtest/gtest.h>

#include <string>
#include <utility>

// Verifies a map is safely reusable immediately after being moved from.
TEST(RepeatedMoveReuse, ReuseImmediatelyAfterMove) {
    HashMapPro::HashMap<int, std::string> source{8};
    source.insert(1, "one");

    HashMapPro::HashMap<int, std::string> destination{std::move(source)};

    EXPECT_EQ(source.size(), 0u);

    source.insert(2, "two");

    EXPECT_EQ(source.size(), 1u);
    EXPECT_EQ(source.at(2), "two");
    EXPECT_EQ(destination.size(), 1u);
    EXPECT_EQ(destination.at(1), "one");
}

// Verifies many move-out/reuse cycles each behave independently,
// without state leaking from one cycle into the next.
TEST(RepeatedMoveReuse, RepeatedMoveReuseCycles) {
    HashMapPro::HashMap<int, int> map{8};

    for (int cycle = 0; cycle < 20; ++cycle) {
        map.insert(cycle, cycle * 10);

        EXPECT_EQ(map.size(), 1u);

        HashMapPro::HashMap<int, int> drained{std::move(map)};

        EXPECT_EQ(map.size(), 0u);
        EXPECT_EQ(drained.size(), 1u);
        EXPECT_EQ(drained.at(cycle), cycle * 10);
    }
}

// Verifies the same map variable can alternate between being a move
// source and a move destination across successive operations.
TEST(RepeatedMoveReuse, AlternatingSourceAndDestination) {
    HashMapPro::HashMap<int, std::string> a{8};
    HashMapPro::HashMap<int, std::string> b{8};

    a.insert(1, "from-a");

    b = std::move(a);
    EXPECT_EQ(b.size(), 1u);
    EXPECT_EQ(a.size(), 0u);

    a.insert(2, "from-a-again");
    a = std::move(b);

    EXPECT_EQ(a.size(), 1u);
    EXPECT_EQ(a.at(1), "from-a");
    EXPECT_EQ(b.size(), 0u);
}

// Verifies a map remains correctly reusable after being moved from
// following a rehash, confirming lazy storage recovers properly.
TEST(RepeatedMoveReuse, ReuseAfterMoveFollowingRehash) {
    HashMapPro::HashMap<int, int> map{4};

    for (int i = 0; i < 10; ++i) {
        map.insert(i, i);
    }

    EXPECT_GT(map.capacity(), 4u);

    HashMapPro::HashMap<int, int> moved{std::move(map)};

    EXPECT_EQ(map.size(), 0u);
    EXPECT_EQ(moved.size(), 10u);

    map.insert(100, 100);

    EXPECT_EQ(map.size(), 1u);
    EXPECT_EQ(map.at(100), 100);
}
