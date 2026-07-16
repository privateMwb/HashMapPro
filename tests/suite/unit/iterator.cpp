// HashMap Iterator Test Suite
// Validates forward and reverse iteration across mutable and
// const iterator interfaces.
//
// Covers:
// - begin()/end() on an empty map are equal
// - forward iteration visits every element exactly once
// - cbegin()/cend() visit every element exactly once
// - const begin()/end() visit every element exactly once
// - reverse iteration visits every element exactly once
// - const reverse iteration visits every element exactly once
// - range-based for loop mutates values
// - pre-increment and post-increment produce equivalent traversal

#include <common/framework.h>

#include <string>
#include <unordered_set>

// Verifies begin() equals end() for an empty map.
static void empty_map_begin_equals_end() {
    HashMapPro::HashMap<int, std::string> map{8};

    CHK(map.begin() == map.end());
}

// Verifies forward iteration visits every element exactly once.
static void forward_iteration_visits_all() {
    HashMapPro::HashMap<int, std::string> map{4};
    map.insert(1, "one");
    map.insert(2, "two");
    map.insert(3, "three");

    std::unordered_set<int> seen;

    for (auto it = map.begin(); it != map.end(); ++it) {
        seen.insert(it->key);
    }

    CHK(seen.size() == 3);
    CHK(seen.count(1) == 1);
    CHK(seen.count(2) == 1);
    CHK(seen.count(3) == 1);
}

// Verifies cbegin()/cend() visit every element exactly once.
static void const_iteration_visits_all() {
    HashMapPro::HashMap<int, std::string> map{4};
    map.insert(1, "one");
    map.insert(2, "two");
    map.insert(3, "three");

    std::unordered_set<int> seen;

    for (auto it = map.cbegin(); it != map.cend(); ++it) {
        seen.insert(it->key);
    }

    CHK(seen.size() == 3);
}

// Verifies const begin()/end() visit every element exactly once.
static void const_map_begin_end() {
    HashMapPro::HashMap<int, std::string> map{4};
    map.insert(1, "one");
    map.insert(2, "two");

    const auto& constMap = map;
    std::unordered_set<int> seen;

    for (auto it = constMap.begin(); it != constMap.end(); ++it) {
        seen.insert(it->key);
    }

    CHK(seen.size() == 2);
}

// Verifies reverse iteration visits every element exactly once.
static void reverse_iteration_visits_all() {
    HashMapPro::HashMap<int, std::string> map{4};
    map.insert(1, "one");
    map.insert(2, "two");
    map.insert(3, "three");

    std::unordered_set<int> seen;

    for (auto it = map.rbegin(); it != map.rend(); ++it) {
        seen.insert(it->key);
    }

    CHK(seen.size() == 3);
}

// Verifies const reverse iteration visits every element exactly once.
static void const_reverse_iteration_visits_all() {
    HashMapPro::HashMap<int, std::string> map{4};
    map.insert(1, "one");
    map.insert(2, "two");
    map.insert(3, "three");

    std::unordered_set<int> seen;

    for (auto it = map.crbegin(); it != map.crend(); ++it) {
        seen.insert(it->key);
    }

    CHK(seen.size() == 3);
}

// Verifies range-based iteration allows value mutation.
static void range_for_mutates_values() {
    HashMapPro::HashMap<int, int> map{4};
    map.insert(1, 10);
    map.insert(2, 20);

    for (auto& node : map) {
        node.value *= 2;
    }

    CHK(map.at(1) == 20);
    CHK(map.at(2) == 40);
}

// Verifies pre-increment and post-increment produce equivalent traversal.
static void increment_consistency() {
    HashMapPro::HashMap<int, std::string> map{4};
    map.insert(1, "one");
    map.insert(2, "two");

    auto pre = map.begin();
    auto post = map.begin();

    ++pre;
    post++;

    CHK(pre == post);
}

// Executes all iterator test cases.
static void run_tests() {
    RUN(empty_map_begin_equals_end);
    RUN(forward_iteration_visits_all);
    RUN(const_iteration_visits_all);
    RUN(const_map_begin_end);
    RUN(reverse_iteration_visits_all);
    RUN(const_reverse_iteration_visits_all);
    RUN(range_for_mutates_values);
    RUN(increment_consistency);
}

REGISTER_TEST_SUITE();