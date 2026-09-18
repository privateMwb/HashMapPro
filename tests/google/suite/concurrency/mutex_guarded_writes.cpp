// HashMap Mutex-Guarded Writes Test Suite
// Verifies that concurrent writes from multiple threads produce a
// correct final state when properly serialized through an external
// mutex, since HashMap itself provides no internal synchronization.
//
// Covers:
// - concurrent inserts of distinct keys, guarded by a mutex, all land
// - concurrent guarded updates from multiple threads sum correctly
// - concurrent guarded erases of distinct keys leave the correct
//   remainder
// - a mix of guarded insert and erase across threads leaves size
//   consistent with the net number of operations

#include <HashMapPro/HashMap.h>

#include <gtest/gtest.h>

#include <mutex>
#include <string>
#include <thread>
#include <vector>

// Verifies concurrent inserts of distinct keys, each guarded by a
// shared mutex, all end up present in the map.
TEST(MutexGuardedWrites, GuardedConcurrentInsertsLand) {
    HashMapPro::HashMap<int, int> map{256};
    std::mutex mutex;
    std::vector<std::thread> threads;

    for (int t = 0; t < 8; ++t) {
        threads.emplace_back([&map, &mutex, t]() {
            for (int i = 0; i < 50; ++i) {
                int key = t * 50 + i;

                std::lock_guard<std::mutex> lock(mutex);
                map.insert(key, key);
            }
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    EXPECT_EQ(map.size(), 400u);

    for (int i = 0; i < 400; ++i) {
        EXPECT_TRUE(map.contains(i));
    }
}

// Verifies concurrent guarded updates to a shared counter-like value
// produce the correct final accumulated result.
TEST(MutexGuardedWrites, GuardedConcurrentUpdatesSumCorrect) {
    HashMapPro::HashMap<std::string, int> map{16};
    map.insert("counter", 0);

    std::mutex mutex;
    std::vector<std::thread> threads;

    for (int t = 0; t < 8; ++t) {
        threads.emplace_back([&map, &mutex]() {
            for (int i = 0; i < 1000; ++i) {
                std::lock_guard<std::mutex> lock(mutex);
                int current = map.at("counter");
                (void)map.update("counter", current + 1);
            }
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    EXPECT_EQ(map.at("counter"), 8000);
}

// Verifies concurrent guarded erases of distinct keys leave exactly
// the correct remainder of elements.
TEST(MutexGuardedWrites, GuardedErasesCorrectRemainder) {
    HashMapPro::HashMap<int, int> map{256};

    for (int i = 0; i < 400; ++i) {
        map.insert(i, i);
    }

    std::mutex mutex;
    std::vector<std::thread> threads;

    for (int t = 0; t < 8; ++t) {
        threads.emplace_back([&map, &mutex, t]() {
            for (int i = 0; i < 50; ++i) {
                int key = t * 50 + i;

                if (key % 2 == 0) {
                    std::lock_guard<std::mutex> lock(mutex);
                    (void)map.erase(key);
                }
            }
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    EXPECT_EQ(map.size(), 200u);

    for (int i = 0; i < 400; ++i) {
        bool shouldRemain = (i % 2 != 0);
        EXPECT_EQ(map.contains(i), shouldRemain);
    }
}

// Verifies a mix of guarded insert and erase operations issued from
// multiple threads leaves the map's size consistent with the net
// number of successful operations performed.
TEST(MutexGuardedWrites, GuardedMixedInsertEraseSize) {
    HashMapPro::HashMap<int, int> map{256};
    std::mutex mutex;
    std::vector<std::thread> threads;

    for (int i = 0; i < 200; ++i) {
        map.insert(i, i);
    }

    for (int t = 0; t < 4; ++t) {
        threads.emplace_back([&map, &mutex, t]() {
            for (int i = 0; i < 50; ++i) {
                int key = t * 50 + i;

                std::lock_guard<std::mutex> lock(mutex);
                (void)map.erase(key);
            }
        });
    }

    for (int t = 4; t < 8; ++t) {
        threads.emplace_back([&map, &mutex, t]() {
            for (int i = 0; i < 50; ++i) {
                int key = 200 + (t - 4) * 50 + i;

                std::lock_guard<std::mutex> lock(mutex);
                map.insert(key, key);
            }
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    EXPECT_EQ(map.size(), 200u);
}
