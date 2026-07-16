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

#include <common/framework.h>

#include <string>

// Verifies capacity does not grow further during a bulk insert once
// enough capacity was reserved ahead of time.
static void reserve_avoids_rehash_during_bulk_insert() {
    HashMapPro::HashMap<int, int> map{4};

    map.reserve(100);
    std::size_t capacityAfterReserve = map.capacity();

    for (int i = 0; i < 100; ++i) {
        map.insert(i, i);
    }

    CHK(map.capacity() == capacityAfterReserve);
    CHK(map.size() == 100);
}

// Verifies a reserve, bulk insert, then bulk update workflow leaves
// the map with every key present and every value updated.
static void reserve_insert_then_update_workflow() {
    HashMapPro::HashMap<int, std::string> map{4};

    map.reserve(50);

    for (int i = 0; i < 50; ++i) {
        map.insert(i, "initial");
    }

    for (int i = 0; i < 50; ++i) {
        bool updated = map.update(i, "updated");
        CHK(updated == true);
    }

    CHK(map.size() == 50);

    for (int i = 0; i < 50; ++i) {
        CHK(map.at(i) == "updated");
    }
}

// Verifies reserve() is a no-op when the map already has sufficient
// capacity for the requested number of elements.
static void reserve_no_op_when_already_sufficient() {
    HashMapPro::HashMap<int, int> map{128};

    std::size_t before = map.capacity();

    map.reserve(10);

    CHK(map.capacity() == before);
}

// Verifies a reserve, followed by interleaved erase and reinsertion,
// leaves the map with a correct and consistent final state.
static void reserve_with_erase_and_reinsert() {
    HashMapPro::HashMap<int, std::string> map{4};

    map.reserve(30);

    for (int i = 0; i < 30; ++i) {
        map.insert(i, "value");
    }

    for (int i = 0; i < 30; i += 3) {
        (void)map.erase(i);
    }

    CHK(map.size() == 20);

    for (int i = 0; i < 30; i += 3) {
        map.insert(i, "reinserted");
    }

    CHK(map.size() == 30);

    for (int i = 0; i < 30; i += 3) {
        CHK(map.at(i) == "reinserted");
    }
}

// Verifies iteration after a reserve/insert/update sequence sees
// every element's final, updated value.
static void iteration_sees_updates_after_reserve() {
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

    CHK(sum == expected);
}

// Executes all reserve-then-update workflow test cases.
static void run_tests() {
    RUN(reserve_avoids_rehash_during_bulk_insert);
    RUN(reserve_insert_then_update_workflow);
    RUN(reserve_no_op_when_already_sufficient);
    RUN(reserve_with_erase_and_reinsert);
    RUN(iteration_sees_updates_after_reserve);
}

REGISTER_TEST_SUITE();
