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

#include <HashMapPro/HashMap.h>

#include <gtest/gtest.h>

#include <atomic>
#include <string>
#include <thread>
#include <vector>

// Verifies each thread's own map instance produces correct results
// independent of what other threads are doing at the same time.
TEST(ThreadLocalInstances, IndependentInstancesCorrectResults) {
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

    EXPECT_EQ(mismatches.load(), 0);
}

// Verifies no cross-thread interference occurs: each thread's map
// only ever contains what that thread itself inserted.
TEST(ThreadLocalInstances, NoCrossThreadInterference) {
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

            EXPECT_EQ(local.size(), 1u);
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    EXPECT_EQ(unexpectedKeys.load(), 0);
}

// Verifies a thread-local map is safely constructed, used, and
// destroyed at thread exit without disturbing other concurrently
// running threads' own instances.
TEST(ThreadLocalInstances, ThreadLocalMapDestroyedSafely) {
    std::atomic<int> completedThreads{0};
    std::vector<std::thread> threads;

    for (int t = 0; t < 8; ++t) {
        threads.emplace_back([&completedThreads, t]() {
            {
                HashMapPro::HashMap<int, int> local{16};
                local.insert(t, t * t);

                EXPECT_EQ(local.at(t), t * t);
            } // local destroyed here, mid-thread, before the thread exits.

            HashMapPro::HashMap<int, int> another{16};
            another.insert(t, t + 1);

            EXPECT_EQ(another.at(t), t + 1);

            ++completedThreads;
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    EXPECT_EQ(completedThreads.load(), 8);
}

// Verifies many short-lived thread-local maps, created and destroyed
// across repeated thread launches, each produce correct, independent
// results.
TEST(ThreadLocalInstances, RepeatedThreadLaunchesCorrect) {
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

    EXPECT_EQ(mismatches.load(), 0);
}
