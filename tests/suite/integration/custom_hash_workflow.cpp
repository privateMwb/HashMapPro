// HashMap Custom Hash Workflow Test Suite
// Verifies a HashMap configured with a user-supplied key type and
// hash functor behaves correctly across a full combined workflow:
// insertion, lookup, update, erase, iteration, copying, and growth.
//
// Covers:
// - insertion and lookup with a custom key type and hash functor
// - update and erase operate correctly on custom keys
// - iteration visits every element keyed by a custom type
// - copying a map preserves custom-keyed elements independently
// - rehashing redistributes custom-keyed elements without data loss

#include <common/framework.h>

#include <cstddef>
#include <functional>
#include <string>
#include <unordered_set>

// A 2D grid coordinate used as a HashMap key.
struct Point {
    int x;
    int y;

    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

// Hash functor for Point, combining the hashes of its two members.
struct PointHash {
    std::size_t operator()(const Point& p) const {
        std::size_t hx = std::hash<int>{}(p.x);
        std::size_t hy = std::hash<int>{}(p.y);

        return hx ^ (hy + 0x9e3779b9 + (hx << 6) + (hx >> 2));
    }
};

using PointMap = HashMapPro::HashMap<Point, std::string, PointHash>;

// Verifies insertion and lookup work correctly with a custom key
// type and hash functor.
static void custom_key_insert_and_lookup() {
    PointMap map{8};

    map.insert({0, 0}, "origin");
    map.insert({1, 1}, "diagonal");
    map.insert({-2, 5}, "off-axis");

    CHK(map.size() == 3);
    CHK(map.contains({0, 0}) == true);
    CHK(map.contains({9, 9}) == false);
    CHK(map.at({1, 1}) == "diagonal");
    CHK(map.at({-2, 5}) == "off-axis");
}

// Verifies update and erase correctly target custom-keyed elements.
static void custom_key_update_and_erase() {
    PointMap map{8};
    map.insert({3, 4}, "point-a");
    map.insert({5, 6}, "point-b");

    bool updated = map.update({3, 4}, "point-a-updated");
    CHK(updated == true);
    CHK(map.at({3, 4}) == "point-a-updated");

    bool erased = map.erase({5, 6});
    CHK(erased == true);
    CHK(map.contains({5, 6}) == false);
    CHK(map.size() == 1);
}

// Verifies iteration visits every element keyed by a custom type
// exactly once.
static void custom_key_iteration_visits_all() {
    PointMap map{8};
    map.insert({0, 0}, "a");
    map.insert({1, 0}, "b");
    map.insert({0, 1}, "c");

    std::unordered_set<std::string> seen;

    for (const auto& node : map) {
        seen.insert(node.value);
    }

    CHK(seen.size() == 3);
    CHK(seen.count("a") == 1);
    CHK(seen.count("b") == 1);
    CHK(seen.count("c") == 1);
}

// Verifies copying a map with custom-keyed elements produces an
// independent deep copy.
static void custom_key_copy_is_independent() {
    PointMap original{8};
    original.insert({1, 1}, "shared");

    PointMap copy = original;
    copy.insert({2, 2}, "copy-only");

    CHK(original.size() == 1);
    CHK(copy.size() == 2);
    CHK(original.contains({2, 2}) == false);
}

// Verifies rehashing redistributes custom-keyed elements without
// losing or corrupting any of them.
static void custom_key_survives_rehash() {
    PointMap map{4};

    for (int i = 0; i < 20; ++i) {
        map.insert({i, -i}, "point-" + std::to_string(i));
    }

    CHK(map.capacity() > 4);
    CHK(map.size() == 20);

    for (int i = 0; i < 20; ++i) {
        CHK(map.at({i, -i}) == "point-" + std::to_string(i));
    }
}

// Executes all custom hash workflow test cases.
static void run_tests() {
    RUN(custom_key_insert_and_lookup);
    RUN(custom_key_update_and_erase);
    RUN(custom_key_iteration_visits_all);
    RUN(custom_key_copy_is_independent);
    RUN(custom_key_survives_rehash);
}

REGISTER_TEST_SUITE();
