// HashMap Repeated Move Reuse Test Suite
// Verifies a map can be moved from and reused repeatedly across many
// cycles without corruption, relying on its lazily-allocated storage.
//
// Covers:
// - a map is safely reusable immediately after being moved from
// - repeated move-out/reuse cycles each behave independently
// - a map alternately used as move source and move destination
// - reuse after move works correctly following a rehash

#include <common/framework.h>

#include <string>
#include <utility>

// Verifies a map is safely reusable immediately after being moved from.
static void reuse_immediately_after_move() {
    HashMapPro::HashMap<int, std::string> source{8};
    source.insert(1, "one");

    HashMapPro::HashMap<int, std::string> destination{std::move(source)};

    CHK(source.size() == 0);

    source.insert(2, "two");

    CHK(source.size() == 1);
    CHK(source.at(2) == "two");
    CHK(destination.size() == 1);
    CHK(destination.at(1) == "one");
}

// Verifies many move-out/reuse cycles each behave independently,
// without state leaking from one cycle into the next.
static void repeated_move_reuse_cycles() {
    HashMapPro::HashMap<int, int> map{8};

    for (int cycle = 0; cycle < 20; ++cycle) {
        map.insert(cycle, cycle * 10);

        CHK(map.size() == 1);

        HashMapPro::HashMap<int, int> drained{std::move(map)};

        CHK(map.size() == 0);
        CHK(drained.size() == 1);
        CHK(drained.at(cycle) == cycle * 10);
    }
}

// Verifies the same map variable can alternate between being a move
// source and a move destination across successive operations.
static void alternating_source_and_destination() {
    HashMapPro::HashMap<int, std::string> a{8};
    HashMapPro::HashMap<int, std::string> b{8};

    a.insert(1, "from-a");

    b = std::move(a);
    CHK(b.size() == 1);
    CHK(a.size() == 0);

    a.insert(2, "from-a-again");
    a = std::move(b);

    CHK(a.size() == 1);
    CHK(a.at(1) == "from-a");
    CHK(b.size() == 0);
}

// Verifies a map remains correctly reusable after being moved from
// following a rehash, confirming lazy storage recovers properly.
static void reuse_after_move_following_rehash() {
    HashMapPro::HashMap<int, int> map{4};

    for (int i = 0; i < 10; ++i) {
        map.insert(i, i);
    }

    CHK(map.capacity() > 4);

    HashMapPro::HashMap<int, int> moved{std::move(map)};

    CHK(map.size() == 0);
    CHK(moved.size() == 10);

    map.insert(100, 100);

    CHK(map.size() == 1);
    CHK(map.at(100) == 100);
}

// Executes all repeated move reuse test cases.
static void run_tests() {
    RUN(reuse_immediately_after_move);
    RUN(repeated_move_reuse_cycles);
    RUN(alternating_source_and_destination);
    RUN(reuse_after_move_following_rehash);
}

REGISTER_TEST_SUITE();
