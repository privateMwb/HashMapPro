// HashMap Erase Last Node Regression Test Suite
// Verifies erase() correctly relinks a bucket chain regardless of
// where in the chain the erased node sits, using a hash functor that
// forces every key into the same bucket to exercise real chains.
//
// Covers:
// - erasing the only node in a single-element bucket empties it
// - erasing the head of a multi-node chain relinks to the next node
// - erasing the tail of a multi-node chain relinks the prior node
// - erasing a middle node relinks around it correctly
// - erasing the last remaining element in the whole map empties it
// - erasing an already-erased key returns false without side effects

#include <HashMapPro/HashMap.h>

#include <gtest/gtest.h>

#include <cstddef>
#include <string>

// Forces every key into bucket 0, guaranteeing a real bucket chain
// regardless of std::hash<int>'s actual distribution.
struct AllSameHash {
    std::size_t operator()(int) const {
        return 0;
    }
};

using CollidingMap = HashMapPro::HashMap<int, std::string, AllSameHash>;

// Verifies erasing the only node in a bucket leaves that bucket, and
// the map, empty.
TEST(EraseLastNode, EraseOnlyNodeInBucket) {
    CollidingMap map{8};
    map.insert(1, "one");

    bool erased = map.erase(1);

    EXPECT_TRUE(erased);
    EXPECT_EQ(map.size(), 0u);
    EXPECT_TRUE(map.empty());
}

// Verifies erasing the head of a multi-node chain correctly relinks
// the bucket to point at the next node. Insertion prepends, so the
// most recently inserted key is the head of the chain.
TEST(EraseLastNode, EraseHeadOfChain) {
    CollidingMap map{8};
    map.insert(1, "one");
    map.insert(2, "two");
    map.insert(3, "three");

    // 3 is the head (most recently inserted).
    bool erased = map.erase(3);

    EXPECT_TRUE(erased);
    EXPECT_EQ(map.size(), 2u);
    EXPECT_TRUE(map.contains(1));
    EXPECT_TRUE(map.contains(2));
    EXPECT_FALSE(map.contains(3));
}

// Verifies erasing the tail of a multi-node chain correctly relinks
// the previous node's next pointer. Since insertion prepends, the
// first-inserted key is the tail of the chain.
TEST(EraseLastNode, EraseTailOfChain) {
    CollidingMap map{8};
    map.insert(1, "one");
    map.insert(2, "two");
    map.insert(3, "three");

    // 1 is the tail (first inserted).
    bool erased = map.erase(1);

    EXPECT_TRUE(erased);
    EXPECT_EQ(map.size(), 2u);
    EXPECT_FALSE(map.contains(1));
    EXPECT_TRUE(map.contains(2));
    EXPECT_TRUE(map.contains(3));
}

// Verifies erasing a middle node correctly relinks around it without
// disturbing either neighbor.
TEST(EraseLastNode, EraseMiddleOfChain) {
    CollidingMap map{8};
    map.insert(1, "one");
    map.insert(2, "two");
    map.insert(3, "three");
    map.insert(4, "four");

    // Chain (head to tail) is 4, 3, 2, 1 — erase the middle, 3.
    bool erased = map.erase(3);

    EXPECT_TRUE(erased);
    EXPECT_EQ(map.size(), 3u);
    EXPECT_TRUE(map.contains(1));
    EXPECT_TRUE(map.contains(2));
    EXPECT_FALSE(map.contains(3));
    EXPECT_TRUE(map.contains(4));
    EXPECT_EQ(map.at(1), "one");
    EXPECT_EQ(map.at(2), "two");
    EXPECT_EQ(map.at(4), "four");
}

// Verifies erasing the last remaining element in the whole map,
// after removing all others first, leaves the map empty.
TEST(EraseLastNode, EraseLastRemainingElement) {
    CollidingMap map{8};
    map.insert(1, "one");
    map.insert(2, "two");

    (void)map.erase(1);

    EXPECT_EQ(map.size(), 1u);

    bool erased = map.erase(2);

    EXPECT_TRUE(erased);
    EXPECT_EQ(map.size(), 0u);
    EXPECT_TRUE(map.empty());
}

// Verifies erasing a key that was already erased returns false and
// does not disturb the remaining elements.
TEST(EraseLastNode, EraseAlreadyErasedKeyReturnsFalse) {
    CollidingMap map{8};
    map.insert(1, "one");
    map.insert(2, "two");

    (void)map.erase(1);

    bool secondErase = map.erase(1);

    EXPECT_FALSE(secondErase);
    EXPECT_EQ(map.size(), 1u);
    EXPECT_TRUE(map.contains(2));
}
