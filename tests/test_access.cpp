// HashMap Access Test Suite
// Validates element access through operator[] and at(), including
// insertion behavior, mutation, const access, and exception handling.
//
// Covers:
// - operator[] inserts a default value for a missing key
// - operator[] returns an existing value
// - operator[] allows value mutation
// - at() returns an existing value
// - at() throws for a missing key
// - const at() returns an existing value
// - const at() throws for a missing key

#include "test_helper.h"

#include <string>
#include <stdexcept>

// Verifies operator[] inserts a default-constructed value for a missing key.
static void subscript_missing_key() {
    HashMapPro::HashMap<int, std::string> map{8};

    CHK(map.contains(1) == false);

    std::string& value = map[1];

    CHK(value           == "");
    CHK(map.contains(1) == true);
    CHK(map.size()      == 1);
}

// Verifies operator[] returns the existing value without creating a new element.
static void subscript_existing_key() {
    HashMapPro::HashMap<int, std::string> map{8};
    map.insert(1, "one");

    CHK(map[1]     == "one");
    CHK(map.size() == 1);
}

// Verifies operator[] returns a mutable reference to the stored value.
static void subscript_mutation() {
    HashMapPro::HashMap<int, std::string> map{8};
    map[1] = "one";

    CHK(map.at(1) == "one");

    map[1] = "uno";

    CHK(map.at(1)  == "uno");
    CHK(map.size() == 1);
}

// Verifies at() returns the value associated with an existing key.
static void at_existing_key() {
    HashMapPro::HashMap<int, std::string> map{8};
    map.insert(1, "one");

    CHK(map.at(1) == "one");
}

// Verifies at() throws std::out_of_range for a missing key.
static void at_missing_key_throws() {
    HashMapPro::HashMap<int, std::string> map{8};

    bool threw = false;

    try {
        (void)map.at(1);
    } catch (const std::out_of_range&) {
        threw = true;
    }

    CHK(threw == true);
}

// Verifies the const overload of at() returns the value for an existing key.
static void const_at_existing_key() {
    HashMapPro::HashMap<int, std::string> map{8};
    map.insert(1, "one");

    const auto& constMap = map;

    CHK(constMap.at(1) == "one");
}

// Verifies the const overload of at() throws std::out_of_range for a missing key.
static void const_at_missing_key_throws() {
    HashMapPro::HashMap<int, std::string> map{8};

    const auto& constMap = map;
    bool threw = false;

    try {
        (void)constMap.at(1);
    } catch (const std::out_of_range&) {
        threw = true;
    }

    CHK(threw == true);
}

// Executes all access test cases.
void run_access_tests() {
    setTitle("Access Tests");

    RUN(subscript_missing_key);
    RUN(subscript_existing_key);
    RUN(subscript_mutation);
    RUN(at_existing_key);
    RUN(at_missing_key_throws);
    RUN(const_at_existing_key);
    RUN(const_at_missing_key_throws);

    std::cout << "\n";
}