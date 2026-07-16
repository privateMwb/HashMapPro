// HashMap Insert/Erase/Iterate Workflow Test Suite
// Verifies insertion, erasure, and iteration remain consistent with
// one another across a combined workflow, rather than testing each
// operation in isolation.
//
// Covers:
// - iteration reflects exactly the elements currently inserted
// - erasing mid-iteration-cycle keeps remaining elements consistent
// - iteration count matches size() after a mix of insert and erase
// - interleaved insert/erase/find operations agree on map state
// - clearing after iteration leaves the map in a fresh, iterable state

#include <common/framework.h>

#include <string>
#include <unordered_set>

// Verifies iteration visits exactly the elements currently present,
// no more and no fewer.
static void iteration_reflects_current_elements() {
    HashMapPro::HashMap<int, std::string> map{8};
    map.insert(1, "one");
    map.insert(2, "two");
    map.insert(3, "three");

    std::unordered_set<int> seen;

    for (const auto& node : map) {
        seen.insert(node.key);
    }

    CHK(seen.size() == 3);
    CHK(seen.count(1) == 1);
    CHK(seen.count(2) == 1);
    CHK(seen.count(3) == 1);
}

// Verifies that erasing an element, then re-iterating, shows the
// remaining elements only, in a self-consistent state.
static void erase_then_iterate_is_consistent() {
    HashMapPro::HashMap<int, std::string> map{8};
    map.insert(1, "one");
    map.insert(2, "two");
    map.insert(3, "three");

    bool erased = map.erase(2);
    CHK(erased == true);

    std::unordered_set<int> seen;

    for (const auto& node : map) {
        seen.insert(node.key);
    }

    CHK(seen.size() == 2);
    CHK(seen.count(1) == 1);
    CHK(seen.count(2) == 0);
    CHK(seen.count(3) == 1);
}

// Verifies the number of elements visited by iteration always
// matches size() after a mix of insert and erase operations.
static void iteration_count_matches_size() {
    HashMapPro::HashMap<int, int> map{4};

    for (int i = 0; i < 10; ++i) {
        map.insert(i, i);
    }

    for (int i = 0; i < 10; i += 2) {
        (void)map.erase(i);
    }

    std::size_t counted = 0;

    for (auto it = map.begin(); it != map.end(); ++it) {
        ++counted;
    }

    CHK(counted == map.size());
    CHK(map.size() == 5);
}

// Verifies interleaved insert, erase, and find operations all agree
// with each other about the map's current state.
static void interleaved_insert_erase_find() {
    HashMapPro::HashMap<int, std::string> map{8};

    map.insert(1, "one");
    CHK(map.find(1) != map.end());

    map.insert(2, "two");
    (void)map.erase(1);

    CHK(map.find(1) == map.end());
    CHK(map.find(2) != map.end());

    map.insert(1, "one-again");

    CHK(map.find(1) != map.end());
    CHK(map.at(1) == "one-again");
    CHK(map.size() == 2);
}

// Verifies clearing a map after iterating over it leaves it in a
// fresh state that can be populated and iterated again.
static void clear_after_iteration_allows_reuse() {
    HashMapPro::HashMap<int, std::string> map{8};
    map.insert(1, "one");
    map.insert(2, "two");

    std::size_t firstPassCount = 0;

    for (auto it = map.begin(); it != map.end(); ++it) {
        ++firstPassCount;
    }

    CHK(firstPassCount == 2);

    map.clear();
    CHK(map.begin() == map.end());

    map.insert(3, "three");

    std::size_t secondPassCount = 0;

    for (auto it = map.begin(); it != map.end(); ++it) {
        ++secondPassCount;
    }

    CHK(secondPassCount == 1);
}

// Executes all insert/erase/iterate workflow test cases.
static void run_tests() {
    RUN(iteration_reflects_current_elements);
    RUN(erase_then_iterate_is_consistent);
    RUN(iteration_count_matches_size);
    RUN(interleaved_insert_erase_find);
    RUN(clear_after_iteration_allows_reuse);
}

REGISTER_TEST_SUITE();
