// HashMap Rehash Threshold Boundary Regression Test Suite
// Verifies automatic rehashing triggers at exactly the right element
// count relative to the max load factor, neither early nor late.
//
// Covers:
// - capacity is unchanged while load factor stays at or below 0.75
// - capacity doubles on the exact insertion that pushes load factor
//   past 0.75
// - the boundary holds at a second, larger capacity
// - load_factor() reports the expected value immediately before and
//   after the triggering insertion
// - max_load_factor() reports the documented constant

#include <common/framework.h>

#include <string>

// Verifies capacity is unchanged for every insertion up to and
// including the load factor threshold, for a capacity-4 map.
static void capacity_unchanged_up_to_threshold() {
    HashMapPro::HashMap<int, std::string> map{4};

    // 0.75 * 4 == 3, so the 3rd element keeps load factor at exactly
    // the threshold and must not trigger growth.
    map.insert(1, "one");
    map.insert(2, "two");
    map.insert(3, "three");

    CHK(map.capacity() == 4);
}

// Verifies the exact insertion that pushes load factor past 0.75
// triggers a capacity doubling, for a capacity-4 map.
static void capacity_doubles_on_threshold_crossing() {
    HashMapPro::HashMap<int, std::string> map{4};

    map.insert(1, "one");
    map.insert(2, "two");
    map.insert(3, "three");

    CHK(map.capacity() == 4);

    // The 4th element makes load factor 4/4 == 1.0, exceeding 0.75.
    map.insert(4, "four");

    CHK(map.capacity() == 8);
}

// Verifies the same threshold behavior holds at a larger starting
// capacity of 8, where the boundary sits at 6 elements.
static void boundary_holds_at_larger_capacity() {
    HashMapPro::HashMap<int, int> map{8};

    // 0.75 * 8 == 6, so 6 elements must not trigger growth.
    for (int i = 0; i < 6; ++i) {
        map.insert(i, i);
    }

    CHK(map.capacity() == 8);

    // The 7th element crosses the threshold.
    map.insert(6, 6);

    CHK(map.capacity() == 16);
}

// Verifies load_factor() reports the expected values immediately
// before and after the insertion that triggers growth.
static void load_factor_reported_correctly_at_boundary() {
    HashMapPro::HashMap<int, std::string> map{4};

    map.insert(1, "one");
    map.insert(2, "two");
    map.insert(3, "three");

    CHK(map.load_factor() == 0.75);

    map.insert(4, "four");

    // After growth to capacity 8 with 4 elements, load factor is 0.5.
    CHK(map.load_factor() == 0.5);
}

// Verifies max_load_factor() reports the documented constant of 0.75.
static void max_load_factor_reports_documented_constant() {
    CHK((HashMapPro::HashMap<int, int>::max_load_factor()) == 0.75);
}

// Executes all rehash threshold boundary test cases.
static void run_tests() {
    RUN(capacity_unchanged_up_to_threshold);
    RUN(capacity_doubles_on_threshold_crossing);
    RUN(boundary_holds_at_larger_capacity);
    RUN(load_factor_reported_correctly_at_boundary);
    RUN(max_load_factor_reports_documented_constant);
}

REGISTER_TEST_SUITE();
