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

#include <common/framework.h>

#include <string>

// Verifies a bucket count of 0 is clamped to the minimum capacity.
static void zero_bucket_count_clamps_to_one() {
    HashMapPro::HashMap<int, std::string> map{0};

    CHK(map.capacity() == 1);
}

// Verifies a zero-bucket-constructed map starts in a valid, empty
// state.
static void zero_bucket_map_starts_empty() {
    HashMapPro::HashMap<int, std::string> map{0};

    CHK(map.size() == 0);
    CHK(map.empty() == true);
}

// Verifies a zero-bucket-constructed map grows correctly once
// elements are inserted into it.
static void zero_bucket_map_grows_on_insert() {
    HashMapPro::HashMap<int, std::string> map{0};

    for (int i = 0; i < 10; ++i) {
        map.insert(i, "value");
    }

    CHK(map.size() == 10);
    CHK(map.capacity() > 1);

    for (int i = 0; i < 10; ++i) {
        CHK(map.contains(i) == true);
    }
}

// Verifies the default constructor (no argument) uses the
// documented default bucket count of 16.
static void default_constructor_uses_documented_default() {
    HashMapPro::HashMap<int, std::string> map;

    CHK(map.capacity() == 16);
}

// Verifies reserve(0) is a no-op and does not alter capacity.
static void reserve_zero_is_no_op() {
    HashMapPro::HashMap<int, std::string> map{8};

    map.reserve(0);

    CHK(map.capacity() == 8);
}

// Executes all zero-bucket construction test cases.
static void run_tests() {
    RUN(zero_bucket_count_clamps_to_one);
    RUN(zero_bucket_map_starts_empty);
    RUN(zero_bucket_map_grows_on_insert);
    RUN(default_constructor_uses_documented_default);
    RUN(reserve_zero_is_no_op);
}

REGISTER_TEST_SUITE();
