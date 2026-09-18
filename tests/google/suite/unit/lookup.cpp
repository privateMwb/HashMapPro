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

#include <HashMapPro/HashMap.h>

#include <gtest/gtest.h>

#include <string>

// Verifies find returns an iterator to an existing element.
TEST(Lookup, FindExistingKey) {
    HashMapPro::HashMap<int, std::string> map{8};
    map.insert(1, "one");

    auto it = map.find(1);

    EXPECT_NE(it, map.end());
    EXPECT_EQ(it->key, 1);
    EXPECT_EQ(it->value, "one");
}

// Verifies find returns end() for a missing key.
TEST(Lookup, FindMissingKey) {
    HashMapPro::HashMap<int, std::string> map{8};
    map.insert(1, "one");

    auto it = map.find(2);

    EXPECT_EQ(it, map.end());
}

// Verifies the iterator returned by find allows value mutation.
TEST(Lookup, FindMutatesValue) {
    HashMapPro::HashMap<int, std::string> map{8};
    map.insert(1, "one");

    auto it = map.find(1);
    it->value = "uno";

    EXPECT_EQ(map.at(1), "uno");
}

// Verifies the const overload of find returns a const_iterator.
TEST(Lookup, ConstFindExistingKey) {
    HashMapPro::HashMap<int, std::string> map{8};
    map.insert(1, "one");

    const auto& constMap = map;
    auto it = constMap.find(1);

    EXPECT_NE(it, constMap.end());
    EXPECT_EQ(it->key, 1);
    EXPECT_EQ(it->value, "one");
}

// Verifies the const overload of find returns end() for a missing key.
TEST(Lookup, ConstFindMissingKey) {
    HashMapPro::HashMap<int, std::string> map{8};
    map.insert(1, "one");

    const auto& constMap = map;
    auto it = constMap.find(2);

    EXPECT_EQ(it, constMap.end());
}

// Verifies contains returns true for an existing key.
TEST(Lookup, ContainsExistingKey) {
    HashMapPro::HashMap<int, std::string> map{8};
    map.insert(1, "one");

    EXPECT_TRUE(map.contains(1));
}

// Verifies contains returns false for a missing key.
TEST(Lookup, ContainsMissingKey) {
    HashMapPro::HashMap<int, std::string> map{8};

    EXPECT_FALSE(map.contains(1));
}

// Verifies contains reflects the map state after erase.
TEST(Lookup, ContainsAfterErase) {
    HashMapPro::HashMap<int, std::string> map{8};
    (void)map.insert(1, "one");
    (void)map.erase(1);

    EXPECT_FALSE(map.contains(1));
}
