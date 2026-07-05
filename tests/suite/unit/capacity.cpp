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

#include <common/framework.h>

#include <string>

// Verifies empty() is true immediately after construction.
static void empty_on_construction() {
    HashMapPro::HashMap<int, std::string> map{8};

    CHK(map.empty() == true);
}

// Verifies empty() becomes false after inserting an element.
static void not_empty_after_insert() {
    HashMapPro::HashMap<int, std::string> map{8};
    map.insert(1, "one");

    CHK(map.empty() == false);
}

// Verifies empty() returns true after clearing all elements.
static void empty_after_clear() {
    HashMapPro::HashMap<int, std::string> map{8};
    map.insert(1, "one");
    map.clear();

    CHK(map.empty() == true);
}

// Verifies size() correctly increases on insert and decreases on erase.
static void size_tracks_insert_and_erase() {
    HashMapPro::HashMap<int, std::string> map{8};

    CHK(map.size() == 0);

    map.insert(1, "one");
    map.insert(2, "two");

    CHK(map.size() == 2);

    (void)map.erase(1);

    CHK(map.size() == 1);
}

// Verifies size() remains unchanged when inserting a duplicate key.
static void size_unchanged_on_duplicate_insert() {
    HashMapPro::HashMap<int, std::string> map{8};
    map.insert(1, "one");
    map.insert(1, "uno");

    CHK(map.size() == 1);
}

// Verifies capacity() equals the initial bucket count provided at construction.
static void capacity_reflects_initial_bucket_count() {
    HashMapPro::HashMap<int, std::string> map{32};

    CHK(map.capacity() == 32);
}

// Verifies capacity() increases when load factor threshold is exceeded.
static void capacity_grows_past_load_factor() {
    HashMapPro::HashMap<int, int> map{4};

    for (int i = 0; i < 3; ++i) {
        map.insert(i, i);
    }

    CHK(map.capacity() == 4);

    map.insert(3, 3);

    CHK(map.capacity() > 4);
}

// Verifies capacity() remains unchanged after erasing elements.
static void capacity_does_not_shrink_after_erase() {
    HashMapPro::HashMap<int, std::string> map{8};
    map.insert(1, "one");
    map.insert(2, "two");

    std::size_t before = map.capacity();

    (void)map.erase(1);
    (void)map.erase(2);

    CHK(map.capacity() == before);
    CHK(map.empty() == true);
}

// Executes all capacity-related test cases.
static void run_tests() {
    RUN(empty_on_construction);
    RUN(not_empty_after_insert);
    RUN(empty_after_clear);
    RUN(size_tracks_insert_and_erase);
    RUN(size_unchanged_on_duplicate_insert);
    RUN(capacity_reflects_initial_bucket_count);
    RUN(capacity_grows_past_load_factor);
    RUN(capacity_does_not_shrink_after_erase);
}

REGISTER_TEST_SUITE();