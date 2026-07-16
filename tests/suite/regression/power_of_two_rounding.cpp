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

#include <common/framework.h>

#include <string>

// Verifies a bucket count that is already a power of two is left
// unchanged by construction.
static void already_power_of_two_unchanged() {
    HashMapPro::HashMap<int, std::string> map{64};

    CHK(map.capacity() == 64);
}

// Verifies a non-power-of-two bucket count is rounded up to the
// next power of two.
static void non_power_of_two_rounds_up() {
    HashMapPro::HashMap<int, std::string> a{17};
    CHK(a.capacity() == 32);

    HashMapPro::HashMap<int, std::string> b{33};
    CHK(b.capacity() == 64);

    HashMapPro::HashMap<int, std::string> c{100};
    CHK(c.capacity() == 128);
}

// Verifies requesting a single bucket keeps the capacity at 1.
static void single_bucket_stays_at_one() {
    HashMapPro::HashMap<int, std::string> map{1};

    CHK(map.capacity() == 1);
}

// Verifies reserve()'s internally computed bucket count is rounded
// up to a power of two, not left at whatever raw value was computed.
static void reserve_target_rounds_up() {
    HashMapPro::HashMap<int, std::string> map{4};

    map.reserve(10);

    std::size_t capacity = map.capacity();

    CHK((capacity & (capacity - 1)) == 0);
    CHK(capacity >= 10);
}

// Verifies capacity remains a power of two after growth triggered by
// crossing the load factor threshold during insertion.
static void capacity_stays_power_of_two_after_growth() {
    HashMapPro::HashMap<int, int> map{4};

    for (int i = 0; i < 50; ++i) {
        map.insert(i, i);

        std::size_t capacity = map.capacity();
        CHK((capacity & (capacity - 1)) == 0);
    }
}

// Executes all power-of-two rounding test cases.
static void run_tests() {
    RUN(already_power_of_two_unchanged);
    RUN(non_power_of_two_rounds_up);
    RUN(single_bucket_stays_at_one);
    RUN(reserve_target_rounds_up);
    RUN(capacity_stays_power_of_two_after_growth);
}

REGISTER_TEST_SUITE();
