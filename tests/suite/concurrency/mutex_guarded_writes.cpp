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

#include <common/framework.h>

#include <mutex>
#include <string>
#include <thread>
#include <vector>

// Verifies concurrent inserts of distinct keys, each guarded by a
// shared mutex, all end up present in the map.
static void guarded_concurrent_inserts_all_land() {
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

    CHK(map.size() == 400);

    for (int i = 0; i < 400; ++i) {
        CHK(map.contains(i) == true);
    }
}

// Verifies concurrent guarded updates to a shared counter-like value
// produce the correct final accumulated result.
static void guarded_concurrent_updates_sum_correctly() {
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

    CHK(map.at("counter") == 8000);
}

// Verifies concurrent guarded erases of distinct keys leave exactly
// the correct remainder of elements.
static void guarded_concurrent_erases_leave_correct_remainder() {
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

    CHK(map.size() == 200);

    for (int i = 0; i < 400; ++i) {
        bool shouldRemain = (i % 2 != 0);
        CHK(map.contains(i) == shouldRemain);
    }
}

// Verifies a mix of guarded insert and erase operations issued from
// multiple threads leaves the map's size consistent with the net
// number of successful operations performed.
static void guarded_mixed_insert_and_erase_consistent_size() {
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

    CHK(map.size() == 200);
}

// Executes all mutex-guarded writes test cases.
static void run_tests() {
    RUN(guarded_concurrent_inserts_all_land);
    RUN(guarded_concurrent_updates_sum_correctly);
    RUN(guarded_concurrent_erases_leave_correct_remainder);
    RUN(guarded_mixed_insert_and_erase_consistent_size);
}

REGISTER_TEST_SUITE();
