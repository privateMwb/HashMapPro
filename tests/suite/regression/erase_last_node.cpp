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

#include <common/framework.h>

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
static void erase_only_node_in_bucket() {
    CollidingMap map{8};
    map.insert(1, "one");

    bool erased = map.erase(1);

    CHK(erased == true);
    CHK(map.size() == 0);
    CHK(map.empty() == true);
}

// Verifies erasing the head of a multi-node chain correctly relinks
// the bucket to point at the next node. Insertion prepends, so the
// most recently inserted key is the head of the chain.
static void erase_head_of_chain() {
    CollidingMap map{8};
    map.insert(1, "one");
    map.insert(2, "two");
    map.insert(3, "three");

    // 3 is the head (most recently inserted).
    bool erased = map.erase(3);

    CHK(erased == true);
    CHK(map.size() == 2);
    CHK(map.contains(1) == true);
    CHK(map.contains(2) == true);
    CHK(map.contains(3) == false);
}

// Verifies erasing the tail of a multi-node chain correctly relinks
// the previous node's next pointer. Since insertion prepends, the
// first-inserted key is the tail of the chain.
static void erase_tail_of_chain() {
    CollidingMap map{8};
    map.insert(1, "one");
    map.insert(2, "two");
    map.insert(3, "three");

    // 1 is the tail (first inserted).
    bool erased = map.erase(1);

    CHK(erased == true);
    CHK(map.size() == 2);
    CHK(map.contains(1) == false);
    CHK(map.contains(2) == true);
    CHK(map.contains(3) == true);
}

// Verifies erasing a middle node correctly relinks around it without
// disturbing either neighbor.
static void erase_middle_of_chain() {
    CollidingMap map{8};
    map.insert(1, "one");
    map.insert(2, "two");
    map.insert(3, "three");
    map.insert(4, "four");

    // Chain (head to tail) is 4, 3, 2, 1 — erase the middle, 3.
    bool erased = map.erase(3);

    CHK(erased == true);
    CHK(map.size() == 3);
    CHK(map.contains(1) == true);
    CHK(map.contains(2) == true);
    CHK(map.contains(3) == false);
    CHK(map.contains(4) == true);
    CHK(map.at(1) == "one");
    CHK(map.at(2) == "two");
    CHK(map.at(4) == "four");
}

// Verifies erasing the last remaining element in the whole map,
// after removing all others first, leaves the map empty.
static void erase_last_remaining_element() {
    CollidingMap map{8};
    map.insert(1, "one");
    map.insert(2, "two");

    (void)map.erase(1);

    CHK(map.size() == 1);

    bool erased = map.erase(2);

    CHK(erased == true);
    CHK(map.size() == 0);
    CHK(map.empty() == true);
}

// Verifies erasing a key that was already erased returns false and
// does not disturb the remaining elements.
static void erase_already_erased_key_returns_false() {
    CollidingMap map{8};
    map.insert(1, "one");
    map.insert(2, "two");

    (void)map.erase(1);

    bool secondErase = map.erase(1);

    CHK(secondErase == false);
    CHK(map.size() == 1);
    CHK(map.contains(2) == true);
}

// Executes all erase-last-node test cases.
static void run_tests() {
    RUN(erase_only_node_in_bucket);
    RUN(erase_head_of_chain);
    RUN(erase_tail_of_chain);
    RUN(erase_middle_of_chain);
    RUN(erase_last_remaining_element);
    RUN(erase_already_erased_key_returns_false);
}

REGISTER_TEST_SUITE();
