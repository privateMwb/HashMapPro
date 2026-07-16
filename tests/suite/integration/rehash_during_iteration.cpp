// HashMap Rehash During Iteration Test Suite
// Verifies how iteration interacts with automatic rehashing: safe
// patterns that survive a rehash, and confirmation that data itself
// stays correct across the growth even though bucket-array-backed
// iterators do not.
//
// Covers:
// - inserting without crossing the load factor keeps an outstanding
//   iterator usable
// - keys captured before a rehash are all still findable afterward
// - a freshly obtained iterator after a rehash visits every element
// - values read through at() are unchanged by a rehash
// - bidirectional traversal is consistent on a freshly obtained
//   iterator following a rehash

#include <common/framework.h>

#include <string>
#include <unordered_set>
#include <vector>

// Verifies that inserting an element which does not cross the load
// factor threshold (so no rehash occurs) leaves a previously obtained
// iterator safely usable, since the bucket array itself is untouched.
static void insert_without_rehash_keeps_iterator_valid() {
    HashMapPro::HashMap<int, std::string> map{16};
    map.insert(1, "one");

    auto it = map.begin();
    std::size_t capacityBefore = map.capacity();

    map.insert(2, "two");

    CHK(map.capacity() == capacityBefore);

    std::size_t counted = 0;

    for (; it != map.end(); ++it) {
        ++counted;
    }

    CHK(counted > 0);
}

// Note: a rehash reallocates the bucket array and frees the old one,
// so an iterator obtained before the rehash becomes dangling. The
// safe pattern is to capture whatever is needed *before* triggering
// growth, or to obtain a fresh iterator *after* it, as shown below.
// The following is never executed — it demonstrates what NOT to do:
//
//   auto it = map.begin();
//   for (int i = 0; i < 1000; ++i) map.insert(i, i); // may rehash
//   ++it; // UB if a rehash occurred — it's bucket array is stale

// Verifies that keys captured before a rehash-triggering burst of
// insertions are all still findable afterward.
static void keys_captured_before_rehash_remain_findable() {
    HashMapPro::HashMap<int, int> map{4};

    for (int i = 0; i < 3; ++i) {
        map.insert(i, i * 100);
    }

    std::vector<int> keysBefore;

    for (const auto& node : map) {
        keysBefore.push_back(node.key);
    }

    std::size_t capacityBefore = map.capacity();

    for (int i = 3; i < 20; ++i) {
        map.insert(i, i * 100);
    }

    CHK(map.capacity() > capacityBefore);

    for (int key : keysBefore) {
        CHK(map.contains(key) == true);
        CHK(map.at(key) == key * 100);
    }
}

// Verifies a freshly obtained iterator, taken after a rehash has
// already happened, correctly visits every element.
static void fresh_iterator_after_rehash_visits_all() {
    HashMapPro::HashMap<int, int> map{4};

    for (int i = 0; i < 20; ++i) {
        map.insert(i, i);
    }

    std::unordered_set<int> seen;

    for (auto it = map.begin(); it != map.end(); ++it) {
        seen.insert(it->key);
    }

    CHK(seen.size() == 20);
    CHK(seen.size() == map.size());
}

// Verifies values read through at() are unaffected by the rehash
// that happened while they were being inserted.
static void values_unchanged_after_rehash() {
    HashMapPro::HashMap<std::string, int> map{4};

    map.insert("alpha", 1);
    map.insert("beta", 2);

    std::size_t capacityBefore = map.capacity();

    for (int i = 0; i < 20; ++i) {
        map.insert("filler" + std::to_string(i), i);
    }

    CHK(map.capacity() > capacityBefore);
    CHK(map.at("alpha") == 1);
    CHK(map.at("beta") == 2);
}

// Verifies forward and backward traversal remain consistent with
// each other on a freshly obtained iterator following a rehash.
static void bidirectional_traversal_consistent_after_rehash() {
    HashMapPro::HashMap<int, int> map{4};

    for (int i = 0; i < 20; ++i) {
        map.insert(i, i);
    }

    auto it = map.begin();
    ++it;
    --it;

    CHK(it == map.begin());

    auto rit = map.rbegin();
    ++rit;
    --rit;

    CHK(rit == map.rbegin());
}

// Executes all rehash-during-iteration test cases.
static void run_tests() {
    RUN(insert_without_rehash_keeps_iterator_valid);
    RUN(keys_captured_before_rehash_remain_findable);
    RUN(fresh_iterator_after_rehash_visits_all);
    RUN(values_unchanged_after_rehash);
    RUN(bidirectional_traversal_consistent_after_rehash);
}

REGISTER_TEST_SUITE();
