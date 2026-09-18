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

#include <HashMapPro/HashMap.h>

#include <gtest/gtest.h>

#include <string>

using InnerMap = HashMapPro::HashMap<int, std::string>;
using OuterMap = HashMapPro::HashMap<int, InnerMap>;

// Verifies operator[] on the outer map default-constructs an empty
// inner map for a missing key.
TEST(NestedMapLifetime, OuterDefaultConstructsInner) {
    OuterMap outer{8};

    EXPECT_FALSE(outer.contains(1));

    InnerMap& inner = outer[1];

    EXPECT_TRUE(inner.empty());
    EXPECT_TRUE(outer.contains(1));
}

// Verifies inner maps can be populated and read back through the
// outer map.
TEST(NestedMapLifetime, PopulateAndReadInnerMaps) {
    OuterMap outer{8};

    outer[1].insert(10, "ten");
    outer[1].insert(20, "twenty");
    outer[2].insert(30, "thirty");

    EXPECT_EQ(outer.at(1).size(), 2u);
    EXPECT_EQ(outer.at(1).at(10), "ten");
    EXPECT_EQ(outer.at(1).at(20), "twenty");
    EXPECT_EQ(outer.at(2).size(), 1u);
    EXPECT_EQ(outer.at(2).at(30), "thirty");
}

// Verifies mutating an inner map through a reference obtained from
// the outer map persists across subsequent lookups.
TEST(NestedMapLifetime, MutateInnerThroughOuter) {
    OuterMap outer{8};
    outer[1].insert(10, "ten");

    outer[1].at(10) = "updated";
    outer[1].insert(11, "eleven");

    EXPECT_EQ(outer.at(1).size(), 2u);
    EXPECT_EQ(outer.at(1).at(10), "updated");
    EXPECT_EQ(outer.at(1).at(11), "eleven");
}

// Verifies clearing the outer map destroys every inner map it owned,
// leaving the outer map empty and safely reusable.
TEST(NestedMapLifetime, ClearOuterDestroysInnerMaps) {
    OuterMap outer{8};
    outer[1].insert(10, "ten");
    outer[2].insert(20, "twenty");

    EXPECT_EQ(outer.size(), 2u);

    outer.clear();

    EXPECT_EQ(outer.size(), 0u);
    EXPECT_TRUE(outer.empty());

    outer[3].insert(30, "thirty");

    EXPECT_EQ(outer.size(), 1u);
    EXPECT_EQ(outer.at(3).at(30), "thirty");
}

// Verifies copying the outer map performs a deep copy of every inner
// map, so mutating a copy's inner map does not affect the original.
TEST(NestedMapLifetime, CopyOuterDeepCopiesInnerMaps) {
    OuterMap original{8};
    original[1].insert(10, "ten");

    OuterMap copy = original;
    copy[1].insert(11, "eleven");

    EXPECT_EQ(original.at(1).size(), 1u);
    EXPECT_EQ(copy.at(1).size(), 2u);
}
