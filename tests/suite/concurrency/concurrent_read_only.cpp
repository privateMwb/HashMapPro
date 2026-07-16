// HashMap Concurrent Read-Only Test Suite
// Verifies that concurrent, read-only access to an already-populated
// map from multiple threads is safe and consistent, since none of
// find(), at(), contains(), or const iteration mutate map state.
//
// Covers:
// - concurrent find() calls from multiple threads all return correct
//   results
// - concurrent contains() calls from multiple threads all agree
// - concurrent at() calls from multiple threads read consistent values
// - concurrent const iteration from multiple threads each visit every
//   element
// - the map's size is unchanged after concurrent read-only access

#include <common/framework.h>

#include <atomic>
#include <string>
#include <thread>
#include <vector>

// Verifies concurrent find() calls from multiple threads each
// correctly locate their assigned key.
static void concurrent_find_returns_correct_results() {
    HashMapPro::HashMap<int, int> map{256};

    for (int i = 0; i < 200; ++i) {
        map.insert(i, i * 2);
    }

    std::atomic<int> mismatches{0};
    std::vector<std::thread> threads;

    for (int t = 0; t < 8; ++t) {
        threads.emplace_back([&map, &mismatches, t]() {
            for (int i = t; i < 200; i += 8) {
                auto it = map.find(i);

                if (it == map.end() || it->value != i * 2) {
                    ++mismatches;
                }
            }
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    CHK(mismatches.load() == 0);
}

// Verifies concurrent contains() calls from multiple threads all
// agree about which keys are present.
static void concurrent_contains_agrees_across_threads() {
    HashMapPro::HashMap<int, int> map{256};

    for (int i = 0; i < 100; ++i) {
        map.insert(i, i);
    }

    std::atomic<int> falseNegatives{0};
    std::atomic<int> falsePositives{0};
    std::vector<std::thread> threads;

    for (int t = 0; t < 8; ++t) {
        threads.emplace_back([&map, &falseNegatives, &falsePositives]() {
            for (int i = 0; i < 200; ++i) {
                bool shouldExist = i < 100;
                bool exists = map.contains(i);

                if (shouldExist && !exists)
                    ++falseNegatives;
                if (!shouldExist && exists)
                    ++falsePositives;
            }
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    CHK(falseNegatives.load() == 0);
    CHK(falsePositives.load() == 0);
}

// Verifies concurrent at() calls from multiple threads all read the
// same, correct value for a shared key.
static void concurrent_at_reads_consistent_values() {
    HashMapPro::HashMap<std::string, int> map{64};
    map.insert("shared", 42);

    std::atomic<int> mismatches{0};
    std::vector<std::thread> threads;

    for (int t = 0; t < 8; ++t) {
        threads.emplace_back([&map, &mismatches]() {
            for (int i = 0; i < 1000; ++i) {
                if (map.at("shared") != 42) {
                    ++mismatches;
                }
            }
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    CHK(mismatches.load() == 0);
}

// Verifies concurrent const iteration from multiple threads each
// visit every element of the map exactly once.
static void concurrent_const_iteration_visits_all() {
    const HashMapPro::HashMap<int, int> map = [] {
        HashMapPro::HashMap<int, int> m{64};
        for (int i = 0; i < 50; ++i) {
            m.insert(i, i);
        }
        return m;
    }();

    std::atomic<int> incorrectCounts{0};
    std::vector<std::thread> threads;

    for (int t = 0; t < 8; ++t) {
        threads.emplace_back([&map, &incorrectCounts]() {
            std::size_t counted = 0;

            for (auto it = map.cbegin(); it != map.cend(); ++it) {
                ++counted;
            }

            if (counted != 50) {
                ++incorrectCounts;
            }
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    CHK(incorrectCounts.load() == 0);
}

// Verifies the map's size is unchanged after a burst of concurrent
// read-only access, confirming no read operation mutated state.
static void size_unchanged_after_concurrent_reads() {
    HashMapPro::HashMap<int, int> map{128};

    for (int i = 0; i < 80; ++i) {
        map.insert(i, i);
    }

    std::size_t sizeBefore = map.size();
    std::vector<std::thread> threads;

    for (int t = 0; t < 8; ++t) {
        threads.emplace_back([&map]() {
            for (int i = 0; i < 80; ++i) {
                (void)map.contains(i);
                (void)map.find(i);
            }
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    CHK(map.size() == sizeBefore);
}

// Executes all concurrent read-only test cases.
static void run_tests() {
    RUN(concurrent_find_returns_correct_results);
    RUN(concurrent_contains_agrees_across_threads);
    RUN(concurrent_at_reads_consistent_values);
    RUN(concurrent_const_iteration_visits_all);
    RUN(size_unchanged_after_concurrent_reads);
}

REGISTER_TEST_SUITE();
