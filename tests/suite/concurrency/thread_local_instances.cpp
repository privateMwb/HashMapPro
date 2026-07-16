// HashMap Thread-Local Instances Test Suite
// Verifies that giving each thread its own independent HashMap
// instance avoids synchronization entirely, and that concurrently
// running threads never observe each other's instance state.
//
// Covers:
// - each thread's independently constructed map computes correct
//   results, unaffected by other threads running concurrently
// - no cross-thread interference occurs between separate instances
// - a thread-local map is safely destroyed at thread exit without
//   affecting other threads' instances
// - many short-lived thread-local maps across thread lifetimes all
//   produce correct, independent results

#include <common/framework.h>

#include <atomic>
#include <string>
#include <thread>
#include <vector>

// Verifies each thread's own map instance produces correct results
// independent of what other threads are doing at the same time.
static void independent_instances_compute_correct_results() {
    std::atomic<int> mismatches{0};
    std::vector<std::thread> threads;

    for (int t = 0; t < 8; ++t) {
        threads.emplace_back([&mismatches, t]() {
            HashMapPro::HashMap<int, int> local{32};

            for (int i = 0; i < 100; ++i) {
                local.insert(i, i + t);
            }

            for (int i = 0; i < 100; ++i) {
                if (local.at(i) != i + t) {
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

// Verifies no cross-thread interference occurs: each thread's map
// only ever contains what that thread itself inserted.
static void no_cross_thread_interference() {
    std::atomic<int> unexpectedKeys{0};
    std::vector<std::thread> threads;

    for (int t = 0; t < 8; ++t) {
        threads.emplace_back([&unexpectedKeys, t]() {
            HashMapPro::HashMap<int, std::string> local{16};

            local.insert(t, "owned-by-" + std::to_string(t));

            for (int other = 0; other < 8; ++other) {
                if (other != t && local.contains(other)) {
                    ++unexpectedKeys;
                }
            }

            CHK(local.size() == 1);
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    CHK(unexpectedKeys.load() == 0);
}

// Verifies a thread-local map is safely constructed, used, and
// destroyed at thread exit without disturbing other concurrently
// running threads' own instances.
static void thread_local_map_destroyed_safely_at_exit() {
    std::atomic<int> completedThreads{0};
    std::vector<std::thread> threads;

    for (int t = 0; t < 8; ++t) {
        threads.emplace_back([&completedThreads, t]() {
            {
                HashMapPro::HashMap<int, int> local{16};
                local.insert(t, t * t);

                CHK(local.at(t) == t * t);
            } // local destroyed here, mid-thread, before the thread exits.

            HashMapPro::HashMap<int, int> another{16};
            another.insert(t, t + 1);

            CHK(another.at(t) == t + 1);

            ++completedThreads;
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    CHK(completedThreads.load() == 8);
}

// Verifies many short-lived thread-local maps, created and destroyed
// across repeated thread launches, each produce correct, independent
// results.
static void repeated_thread_launches_produce_correct_results() {
    std::atomic<int> mismatches{0};

    for (int round = 0; round < 5; ++round) {
        std::vector<std::thread> threads;

        for (int t = 0; t < 4; ++t) {
            threads.emplace_back([&mismatches, round, t]() {
                HashMapPro::HashMap<int, int> local{8};
                int key = round * 10 + t;

                local.insert(key, key * 2);

                if (local.at(key) != key * 2 || local.size() != 1) {
                    ++mismatches;
                }
            });
        }

        for (auto& thread : threads) {
            thread.join();
        }
    }

    CHK(mismatches.load() == 0);
}

// Executes all thread-local instances test cases.
static void run_tests() {
    RUN(independent_instances_compute_correct_results);
    RUN(no_cross_thread_interference);
    RUN(thread_local_map_destroyed_safely_at_exit);
    RUN(repeated_thread_launches_produce_correct_results);
}

REGISTER_TEST_SUITE();
