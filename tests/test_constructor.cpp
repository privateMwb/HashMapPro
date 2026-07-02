// HashMap Constructor Test Suite
// Verifies constructor, copy, and move behavior.
//
// Covers:
// - construction with a specified bucket count
// - zero bucket count is clamped to the minimum
// - default constructor uses the default bucket count
// - newly constructed map is empty
// - copy construction performs a deep copy
// - copy assignment performs a deep copy
// - self copy assignment preserves the existing state
// - move construction transfers ownership
// - move assignment transfers ownership
// - self move assignment preserves a valid state

#include "test_helper.h"

#include <string>

// Verifies construction with the requested bucket count.
static void basic_construction() {
    HashMapPro::HashMap<int, int> map{16};

    CHK(map.capacity() == 16);
    CHK(map.size()     == 0);
    CHK(map.empty()    == true);
}

// Verifies zero buckets are clamped to the minimum capacity.
static void zero_bucket_count() {
    HashMapPro::HashMap<int, int> map{0};

    CHK(map.capacity() == 1);
    CHK(map.size()     == 0);
}

// Verifies the default constructor uses the default bucket count.
static void default_bucket_count() {
    HashMapPro::HashMap<int, int> map;

    CHK(map.capacity() == 16);
    CHK(map.empty()    == true);
}

// Verifies a newly constructed map is empty.
static void initial_state() {
    HashMapPro::HashMap<int, std::string> map{8};

    CHK(map.contains(1) == false);
    CHK(map.find(1)     == map.end());
    CHK(map.begin()     == map.end());
}

// Verifies copy construction performs a deep copy.
static void copy_construction() {
    HashMapPro::HashMap<int, std::string> a{8};
    a.insert(1, "one");
    a.insert(2, "two");

    HashMapPro::HashMap<int, std::string> b{a};

    CHK(b.size()      == 2);
    CHK(b.at(1)       == "one");
    CHK(b.at(2)       == "two");
    CHK(a.size()      == 2);

    b.insert(3, "three");
    CHK(a.contains(3) == false);
}

// Verifies copy assignment replaces the previous state with a deep copy.
static void copy_assignment() {
    HashMapPro::HashMap<int, std::string> a{8};
    a.insert(1, "one");

    HashMapPro::HashMap<int, std::string> b{4};
    b.insert(99, "stale");

    b = a;

    CHK(b.size()       == 1);
    CHK(b.at(1)        == "one");
    CHK(b.contains(99) == false);
}

// Verifies self copy assignment preserves the existing state.
static void copy_assignment_self() {
    HashMapPro::HashMap<int, std::string> a{8};
    a.insert(1, "one");

    a = a;

    CHK(a.size() == 1);
    CHK(a.at(1)  == "one");
}

// Verifies move construction transfers ownership to the destination.
static void move_construction() {
    HashMapPro::HashMap<int, std::string> a{8};
    a.insert(1, "one");

    HashMapPro::HashMap<int, std::string> b{std::move(a)};

    CHK(b.size()     == 1);
    CHK(b.at(1)      == "one");
    CHK(a.size()     == 0);
    CHK(a.capacity() == 0);
}

// Verifies move assignment transfers ownership to the destination.
static void move_assignment() {
    HashMapPro::HashMap<int, std::string> a{8};
    a.insert(1, "one");

    HashMapPro::HashMap<int, std::string> b{4};
    b.insert(99, "stale");

    b = std::move(a);

    CHK(b.size()     == 1);
    CHK(b.at(1)      == "one");
    CHK(a.size()     == 0);
    CHK(a.capacity() == 0);
}

// Verifies self move assignment preserves a valid map state.
static void self_move_assignment() {
    HashMapPro::HashMap<int, std::string> a{8};
    a.insert(1, "one");

    a = std::move(a);

    CHK(a.size() == 1);
    CHK(a.at(1)  == "one");
}

// Executes all constructor test cases.
void run_constructor_tests() {
    setTitle("Constructor Tests");

    RUN(basic_construction);
    RUN(zero_bucket_count);
    RUN(default_bucket_count);
    RUN(initial_state);
    RUN(copy_construction);
    RUN(copy_assignment);
    RUN(copy_assignment_self);
    RUN(move_construction);
    RUN(move_assignment);
    RUN(self_move_assignment);

    std::cout << "\n";
}