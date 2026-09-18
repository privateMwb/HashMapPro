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

#include <HashMapPro/HashMap.h>

#include <gtest/gtest.h>

#include <stdexcept>
#include <string>

// Verifies operator[] inserts a default-constructed value for a missing key.
TEST(Access, SubscriptMissingKey) {
    HashMapPro::HashMap<int, std::string> map{8};

    EXPECT_FALSE(map.contains(1));

    std::string& value = map[1];

    EXPECT_EQ(value, "");
    EXPECT_TRUE(map.contains(1));
    EXPECT_EQ(map.size(), 1u);
}

// Verifies operator[] returns the existing value without creating a new element.
TEST(Access, SubscriptExistingKey) {
    HashMapPro::HashMap<int, std::string> map{8};
    map.insert(1, "one");

    EXPECT_EQ(map[1], "one");
    EXPECT_EQ(map.size(), 1u);
}

// Verifies operator[] returns a mutable reference to the stored value.
TEST(Access, SubscriptMutation) {
    HashMapPro::HashMap<int, std::string> map{8};
    map[1] = "one";

    EXPECT_EQ(map.at(1), "one");

    map[1] = "uno";

    EXPECT_EQ(map.at(1), "uno");
    EXPECT_EQ(map.size(), 1u);
}

// Verifies at() returns the value associated with an existing key.
TEST(Access, AtExistingKey) {
    HashMapPro::HashMap<int, std::string> map{8};
    map.insert(1, "one");

    EXPECT_EQ(map.at(1), "one");
}

// Verifies at() throws std::out_of_range for a missing key.
TEST(Access, AtMissingKeyThrows) {
    HashMapPro::HashMap<int, std::string> map{8};

    bool threw = false;

    try {
        (void)map.at(1);
    } catch (const std::out_of_range&) {
        threw = true;
    }

    EXPECT_TRUE(threw);
}

// Verifies the const overload of at() returns the value for an existing key.
TEST(Access, ConstAtExistingKey) {
    HashMapPro::HashMap<int, std::string> map{8};
    map.insert(1, "one");

    const auto& constMap = map;

    EXPECT_EQ(constMap.at(1), "one");
}

// Verifies the const overload of at() throws std::out_of_range for a missing key.
TEST(Access, ConstAtMissingKeyThrows) {
    HashMapPro::HashMap<int, std::string> map{8};

    const auto& constMap = map;
    bool threw = false;

    try {
        (void)constMap.at(1);
    } catch (const std::out_of_range&) {
        threw = true;
    }

    EXPECT_TRUE(threw);
}
