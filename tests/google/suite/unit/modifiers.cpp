// HashMap Modifiers Test Suite
// Validates insert, update, erase, and clear behavior, including
// duplicate-key handling and load-factor-triggered rehashing.
//
// Covers:
// - insert adds a new key/value pair
// - insert on an existing key is a no-op
// - update modifies an existing key's value
// - update on a missing key returns false
// - erase removes an existing key
// - erase on a missing key returns false
// - clear removes all elements and resets size
// - clear followed by reinsertion
// - insert triggers rehash growth past the max load factor

#include <HashMapPro/HashMap.h>

#include <gtest/gtest.h>

#include <string>

// Verifies insert adds a new key/value pair.
TEST(Modifiers, InsertNewKey) {
    HashMapPro::HashMap<int, std::string> map{8};

    map.insert(1, "one");

    EXPECT_EQ(map.size(), 1u);
    EXPECT_TRUE(map.contains(1));
    EXPECT_EQ(map.at(1), "one");
}

// Verifies insert does not overwrite an existing key.
TEST(Modifiers, InsertExistingKeyNoOp) {
    HashMapPro::HashMap<int, std::string> map{8};

    map.insert(1, "one");
    map.insert(1, "uno");

    EXPECT_EQ(map.size(), 1u);
    EXPECT_EQ(map.at(1), "one");
}

// Verifies update modifies the value of an existing key.
TEST(Modifiers, UpdateExistingKey) {
    HashMapPro::HashMap<int, std::string> map{8};
    map.insert(1, "one");

    bool updated = map.update(1, "uno");

    EXPECT_TRUE(updated);
    EXPECT_EQ(map.at(1), "uno");
    EXPECT_EQ(map.size(), 1u);
}

// Verifies update returns false for a missing key.
TEST(Modifiers, UpdateMissingKey) {
    HashMapPro::HashMap<int, std::string> map{8};

    bool updated = map.update(1, "one");

    EXPECT_FALSE(updated);
    EXPECT_FALSE(map.contains(1));
    EXPECT_EQ(map.size(), 0u);
}

// Verifies erase removes an existing key.
TEST(Modifiers, EraseExistingKey) {
    HashMapPro::HashMap<int, std::string> map{8};
    map.insert(1, "one");
    map.insert(2, "two");

    bool erased = map.erase(1);

    EXPECT_TRUE(erased);
    EXPECT_FALSE(map.contains(1));
    EXPECT_TRUE(map.contains(2));
    EXPECT_EQ(map.size(), 1u);
}

// Verifies erase returns false for a missing key.
TEST(Modifiers, EraseMissingKey) {
    HashMapPro::HashMap<int, std::string> map{8};
    map.insert(1, "one");

    bool erased = map.erase(2);

    EXPECT_FALSE(erased);
    EXPECT_EQ(map.size(), 1u);
}

// Verifies clear removes all elements while preserving capacity.
TEST(Modifiers, ClearResetsMap) {
    HashMapPro::HashMap<int, std::string> map{8};
    map.insert(1, "one");
    map.insert(2, "two");
    map.insert(3, "three");

    map.clear();

    EXPECT_EQ(map.size(), 0u);
    EXPECT_TRUE(map.empty());
    EXPECT_FALSE(map.contains(1));
    EXPECT_EQ(map.capacity(), 8u);
}

// Verifies the map remains usable after clear.
TEST(Modifiers, ClearThenReinsert) {
    HashMapPro::HashMap<int, std::string> map{8};
    map.insert(1, "one");
    map.clear();
    map.insert(2, "two");

    EXPECT_EQ(map.size(), 1u);
    EXPECT_FALSE(map.contains(1));
    EXPECT_TRUE(map.contains(2));
}

// Verifies insertion beyond the load factor triggers rehashing.
TEST(Modifiers, InsertTriggersRehash) {
    HashMapPro::HashMap<int, int> map{4};

    for (int i = 0; i < 10; ++i) {
        map.insert(i, i * 10);
    }

    EXPECT_EQ(map.size(), 10u);
    EXPECT_GT(map.capacity(), 4u);

    for (int i = 0; i < 10; ++i) {
        EXPECT_EQ(map.at(i), i * 10);
    }
}
