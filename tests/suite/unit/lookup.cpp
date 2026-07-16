// HashMap Lookup Test Suite
// Validates find and contains behavior across mutable and
// const lookup operations.
//
// Covers:
// - find returns an iterator to an existing key
// - find returns end() for a missing key
// - find allows value mutation through the returned iterator
// - const find returns a const_iterator
// - const find returns end() for a missing key
// - contains returns true for an existing key
// - contains returns false for a missing key
// - contains reflects state after erase

#include <common/framework.h>

#include <string>

// Verifies find returns an iterator to an existing element.
static void find_existing_key() {
    HashMapPro::HashMap<int, std::string> map{8};
    map.insert(1, "one");

    auto it = map.find(1);

    CHK(it != map.end());
    CHK(it->key == 1);
    CHK(it->value == "one");
}

// Verifies find returns end() for a missing key.
static void find_missing_key() {
    HashMapPro::HashMap<int, std::string> map{8};
    map.insert(1, "one");

    auto it = map.find(2);

    CHK(it == map.end());
}

// Verifies the iterator returned by find allows value mutation.
static void find_mutates_value() {
    HashMapPro::HashMap<int, std::string> map{8};
    map.insert(1, "one");

    auto it = map.find(1);
    it->value = "uno";

    CHK(map.at(1) == "uno");
}

// Verifies the const overload of find returns a const_iterator.
static void const_find_existing_key() {
    HashMapPro::HashMap<int, std::string> map{8};
    map.insert(1, "one");

    const auto& constMap = map;
    auto it = constMap.find(1);

    CHK(it != constMap.end());
    CHK(it->key == 1);
    CHK(it->value == "one");
}

// Verifies the const overload of find returns end() for a missing key.
static void const_find_missing_key() {
    HashMapPro::HashMap<int, std::string> map{8};
    map.insert(1, "one");

    const auto& constMap = map;
    auto it = constMap.find(2);

    CHK(it == constMap.end());
}

// Verifies contains returns true for an existing key.
static void contains_existing_key() {
    HashMapPro::HashMap<int, std::string> map{8};
    map.insert(1, "one");

    CHK(map.contains(1) == true);
}

// Verifies contains returns false for a missing key.
static void contains_missing_key() {
    HashMapPro::HashMap<int, std::string> map{8};

    CHK(map.contains(1) == false);
}

// Verifies contains reflects the map state after erase.
static void contains_after_erase() {
    HashMapPro::HashMap<int, std::string> map{8};
    (void)map.insert(1, "one");
    (void)map.erase(1);

    CHK(map.contains(1) == false);
}

// Executes all lookup test cases.
static void run_tests() {
    RUN(find_existing_key);
    RUN(find_missing_key);
    RUN(find_mutates_value);
    RUN(const_find_existing_key);
    RUN(const_find_missing_key);
    RUN(contains_existing_key);
    RUN(contains_missing_key);
    RUN(contains_after_erase);
}

REGISTER_TEST_SUITE();