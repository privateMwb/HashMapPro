// HashMap Nested Map Lifetime Test Suite
// Verifies a HashMap can safely hold other HashMap instances as
// values, and that construction, mutation, and destruction all
// propagate correctly through the nested ownership.
//
// Covers:
// - a HashMap of HashMaps default-constructs inner values on access
// - inner maps can be populated and read back through the outer map
// - mutating an inner map through the outer map persists correctly
// - clearing the outer map destroys every inner map it owns
// - copying the outer map deep-copies every inner map it holds

#include <common/framework.h>

#include <string>

using InnerMap = HashMapPro::HashMap<int, std::string>;
using OuterMap = HashMapPro::HashMap<int, InnerMap>;

// Verifies operator[] on the outer map default-constructs an empty
// inner map for a missing key.
static void outer_default_constructs_inner() {
    OuterMap outer{8};

    CHK(outer.contains(1) == false);

    InnerMap& inner = outer[1];

    CHK(inner.empty() == true);
    CHK(outer.contains(1) == true);
}

// Verifies inner maps can be populated and read back through the
// outer map.
static void populate_and_read_inner_maps() {
    OuterMap outer{8};

    outer[1].insert(10, "ten");
    outer[1].insert(20, "twenty");
    outer[2].insert(30, "thirty");

    CHK(outer.at(1).size() == 2);
    CHK(outer.at(1).at(10) == "ten");
    CHK(outer.at(1).at(20) == "twenty");
    CHK(outer.at(2).size() == 1);
    CHK(outer.at(2).at(30) == "thirty");
}

// Verifies mutating an inner map through a reference obtained from
// the outer map persists across subsequent lookups.
static void mutate_inner_through_outer() {
    OuterMap outer{8};
    outer[1].insert(10, "ten");

    outer[1].at(10) = "updated";
    outer[1].insert(11, "eleven");

    CHK(outer.at(1).size() == 2);
    CHK(outer.at(1).at(10) == "updated");
    CHK(outer.at(1).at(11) == "eleven");
}

// Verifies clearing the outer map destroys every inner map it owned,
// leaving the outer map empty and safely reusable.
static void clear_outer_destroys_inner_maps() {
    OuterMap outer{8};
    outer[1].insert(10, "ten");
    outer[2].insert(20, "twenty");

    CHK(outer.size() == 2);

    outer.clear();

    CHK(outer.size() == 0);
    CHK(outer.empty() == true);

    outer[3].insert(30, "thirty");

    CHK(outer.size() == 1);
    CHK(outer.at(3).at(30) == "thirty");
}

// Verifies copying the outer map performs a deep copy of every inner
// map, so mutating a copy's inner map does not affect the original.
static void copy_outer_deep_copies_inner_maps() {
    OuterMap original{8};
    original[1].insert(10, "ten");

    OuterMap copy = original;
    copy[1].insert(11, "eleven");

    CHK(original.at(1).size() == 1);
    CHK(copy.at(1).size() == 2);
}

// Executes all nested map lifetime test cases.
static void run_tests() {
    RUN(outer_default_constructs_inner);
    RUN(populate_and_read_inner_maps);
    RUN(mutate_inner_through_outer);
    RUN(clear_outer_destroys_inner_maps);
    RUN(copy_outer_deep_copies_inner_maps);
}

REGISTER_TEST_SUITE();
