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

#include <common/framework.h>

#include <string>

// Verifies insert adds a new key/value pair.
static void insert_new_key() {
    HashMapPro::HashMap<int, std::string> map{8};

    map.insert(1, "one");

    CHK(map.size() == 1);
    CHK(map.contains(1) == true);
    CHK(map.at(1) == "one");
}

// Verifies insert does not overwrite an existing key.
static void insert_existing_key_no_op() {
    HashMapPro::HashMap<int, std::string> map{8};

    map.insert(1, "one");
    map.insert(1, "uno");

    CHK(map.size() == 1);
    CHK(map.at(1) == "one");
}

// Verifies update modifies the value of an existing key.
static void update_existing_key() {
    HashMapPro::HashMap<int, std::string> map{8};
    map.insert(1, "one");

    bool updated = map.update(1, "uno");

    CHK(updated == true);
    CHK(map.at(1) == "uno");
    CHK(map.size() == 1);
}

// Verifies update returns false for a missing key.
static void update_missing_key() {
    HashMapPro::HashMap<int, std::string> map{8};

    bool updated = map.update(1, "one");

    CHK(updated == false);
    CHK(map.contains(1) == false);
    CHK(map.size() == 0);
}

// Verifies erase removes an existing key.
static void erase_existing_key() {
    HashMapPro::HashMap<int, std::string> map{8};
    map.insert(1, "one");
    map.insert(2, "two");

    bool erased = map.erase(1);

    CHK(erased == true);
    CHK(map.contains(1) == false);
    CHK(map.contains(2) == true);
    CHK(map.size() == 1);
}

// Verifies erase returns false for a missing key.
static void erase_missing_key() {
    HashMapPro::HashMap<int, std::string> map{8};
    map.insert(1, "one");

    bool erased = map.erase(2);

    CHK(erased == false);
    CHK(map.size() == 1);
}

// Verifies clear removes all elements while preserving capacity.
static void clear_resets_map() {
    HashMapPro::HashMap<int, std::string> map{8};
    map.insert(1, "one");
    map.insert(2, "two");
    map.insert(3, "three");

    map.clear();

    CHK(map.size() == 0);
    CHK(map.empty() == true);
    CHK(map.contains(1) == false);
    CHK(map.capacity() == 8);
}

// Verifies the map remains usable after clear.
static void clear_then_reinsert() {
    HashMapPro::HashMap<int, std::string> map{8};
    map.insert(1, "one");
    map.clear();
    map.insert(2, "two");

    CHK(map.size() == 1);
    CHK(map.contains(1) == false);
    CHK(map.contains(2) == true);
}

// Verifies insertion beyond the load factor triggers rehashing.
static void insert_triggers_rehash() {
    HashMapPro::HashMap<int, int> map{4};

    for (int i = 0; i < 10; ++i) {
        map.insert(i, i * 10);
    }

    CHK(map.size() == 10);
    CHK(map.capacity() > 4);

    for (int i = 0; i < 10; ++i) {
        CHK(map.at(i) == i * 10);
    }
}

// Executes all modifier test cases.
static void run_tests() {
    RUN(insert_new_key);
    RUN(insert_existing_key_no_op);
    RUN(update_existing_key);
    RUN(update_missing_key);
    RUN(erase_existing_key);
    RUN(erase_missing_key);
    RUN(clear_resets_map);
    RUN(clear_then_reinsert);
    RUN(insert_triggers_rehash);
}

REGISTER_TEST_SUITE();