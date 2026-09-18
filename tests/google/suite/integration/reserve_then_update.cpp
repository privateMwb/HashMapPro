// HashMap Reserve Then Update Workflow Test Suite
// Verifies reserve() correctly combines with subsequent bulk
// insertion, updates, lookups, and iteration in a single workflow,
// confirming pre-sized capacity is honored throughout.
//
// Covers:
// - reserve() grows capacity enough to avoid rehashing during
//   a subsequent bulk insert
// - reserve() followed by bulk insert then bulk update is consistent
// - reserve() is a no-op when capacity is already sufficient
// - reserve() combined with erase and reinsertion keeps state correct
// - iteration after reserve/insert/update sees every updated value

#include <HashMapPro/HashMap.h>

#include <gtest/gtest.h>

#include <string>

// Verifies capacity does not grow further during a bulk insert once
// enough capacity was reserved ahead of time.
TEST(ReserveThenUpdate, ReserveAvoidsRehashDuringBulkInsert) {
    HashMapPro::HashMap<int, int> map{4};

    map.reserve(100);
    std::size_t capacityAfterReserve = map.capacity();

    for (int i = 0; i < 100; ++i) {
        map.insert(i, i);
    }

    EXPECT_EQ(map.capacity(), capacityAfterReserve);
    EXPECT_EQ(map.size(), 100u);
}

// Verifies a reserve, bulk insert, then bulk update workflow leaves
// the map with every key present and every value updated.
TEST(ReserveThenUpdate, ReserveInsertThenUpdateWorkflow) {
    HashMapPro::HashMap<int, std::string> map{4};

    map.reserve(50);

    for (int i = 0; i < 50; ++i) {
        map.insert(i, "initial");
    }

    for (int i = 0; i < 50; ++i) {
        bool updated = map.update(i, "updated");
        EXPECT_TRUE(updated);
    }

    EXPECT_EQ(map.size(), 50u);

    for (int i = 0; i < 50; ++i) {
        EXPECT_EQ(map.at(i), "updated");
    }
}

// Verifies reserve() is a no-op when the map already has sufficient
// capacity for the requested number of elements.
TEST(ReserveThenUpdate, ReserveNoOpWhenAlreadySufficient) {
    HashMapPro::HashMap<int, int> map{128};

    std::size_t before = map.capacity();

    map.reserve(10);

    EXPECT_EQ(map.capacity(), before);
}

// Verifies a reserve, followed by interleaved erase and reinsertion,
// leaves the map with a correct and consistent final state.
TEST(ReserveThenUpdate, ReserveWithEraseAndReinsert) {
    HashMapPro::HashMap<int, std::string> map{4};

    map.reserve(30);

    for (int i = 0; i < 30; ++i) {
        map.insert(i, "value");
    }

    for (int i = 0; i < 30; i += 3) {
        (void)map.erase(i);
    }

    EXPECT_EQ(map.size(), 20u);

    for (int i = 0; i < 30; i += 3) {
        map.insert(i, "reinserted");
    }

    EXPECT_EQ(map.size(), 30u);

    for (int i = 0; i < 30; i += 3) {
        EXPECT_EQ(map.at(i), "reinserted");
    }
}

// Verifies iteration after a reserve/insert/update sequence sees
// every element's final, updated value.
TEST(ReserveThenUpdate, IterationSeesUpdatesAfterReserve) {
    HashMapPro::HashMap<int, int> map{4};

    map.reserve(20);

    for (int i = 0; i < 20; ++i) {
        map.insert(i, 0);
    }

    for (int i = 0; i < 20; ++i) {
        (void)map.update(i, i * 2);
    }

    int sum = 0;

    for (const auto& node : map) {
        sum += node.value;
    }

    int expected = 0;
    for (int i = 0; i < 20; ++i) {
        expected += i * 2;
    }

    EXPECT_EQ(sum, expected);
}
