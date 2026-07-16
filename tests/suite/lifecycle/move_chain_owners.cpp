// HashMap Move Chain Test Suite
// Verifies ownership transfers correctly across a chain of multiple
// move operations, through both move construction and move assignment.
//
// Covers:
// - ownership transferred through a chain of move constructions
// - ownership transferred through a chain of move assignments
// - a map moved into a function parameter and moved back out
// - every intermediate owner in a move chain is left empty

#include <common/framework.h>

#include <string>
#include <utility>

// Moves a map into a function by value and returns it, exercising a
// move-in / move-out round trip through a function boundary.
static HashMapPro::HashMap<int, std::string>
passThrough(HashMapPro::HashMap<int, std::string> map) {
    map.insert(999, "added inside");
    return map;
}

// Verifies ownership is correctly transferred through a chain of
// move constructions, leaving every intermediate owner empty.
static void move_construction_chain() {
    HashMapPro::HashMap<int, std::string> a{8};
    a.insert(1, "one");
    a.insert(2, "two");

    HashMapPro::HashMap<int, std::string> b{std::move(a)};
    HashMapPro::HashMap<int, std::string> c{std::move(b)};
    HashMapPro::HashMap<int, std::string> d{std::move(c)};

    CHK(d.size() == 2);
    CHK(d.at(1) == "one");
    CHK(d.at(2) == "two");

    // NOLINTBEGIN(clang-analyzer-cplusplus.Move)
    // HashMap guarantees moved-from containers become empty.
    CHK(a.size() == 0);
    CHK(b.size() == 0);
    CHK(c.size() == 0);
    // NOLINTEND(clang-analyzer-cplusplus.Move)
}

// Verifies ownership is correctly transferred through a chain of
// move assignments into already-constructed maps.
static void move_assignment_chain() {
    HashMapPro::HashMap<int, std::string> a{8};
    a.insert(1, "one");

    HashMapPro::HashMap<int, std::string> b{4};
    b.insert(99, "stale-b");

    HashMapPro::HashMap<int, std::string> c{4};
    c.insert(98, "stale-c");

    b = std::move(a);
    c = std::move(b);

    CHK(c.size() == 1);
    CHK(c.at(1) == "one");
    CHK(c.contains(98) == false);

    // NOLINTBEGIN(clang-analyzer-cplusplus.Move)
    // HashMap guarantees moved-from containers become empty.
    CHK(a.size() == 0);
    CHK(b.size() == 0);
    // NOLINTEND(clang-analyzer-cplusplus.Move)
}

// Verifies a map survives being moved into a function parameter and
// moved back out through the return value.
static void move_through_function_boundary() {
    HashMapPro::HashMap<int, std::string> source{8};
    source.insert(1, "one");

    HashMapPro::HashMap<int, std::string> result = passThrough(std::move(source));

    CHK(result.size() == 2);
    CHK(result.at(1) == "one");
    CHK(result.at(999) == "added inside");

    // NOLINTBEGIN(clang-analyzer-cplusplus.Move)
    // HashMap guarantees moved-from containers become empty.
    CHK(source.size() == 0);
    // NOLINTEND(clang-analyzer-cplusplus.Move)
}

// Executes all move chain test cases.
static void run_tests() {
    RUN(move_construction_chain);
    RUN(move_assignment_chain);
    RUN(move_through_function_boundary);
}

REGISTER_TEST_SUITE();
