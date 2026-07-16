// HashMap Cache Usage Pattern Test Suite
// Verifies HashMap correctly supports a realistic cache-like usage
// pattern: lookups on a miss falling back to a default, capacity
// eviction via clear, and repeated get/put cycles.
//
// Covers:
// - a cache miss via find() falls back to a default value
// - a cache hit via operator[] returns the previously stored value
// - overwriting a cached entry replaces the stored value
// - clearing the cache evicts all entries and allows fresh reuse
// - a repeated get-or-compute pattern only computes once per key

#include <common/framework.h>

#include <string>

// Simulates an expensive computation whose result should be cached.
static int expensiveComputation(int key, int& computeCount) {
    ++computeCount;
    return key * key;
}

// Verifies a cache miss falls back to a default value instead of
// throwing or returning garbage.
static void cache_miss_falls_back_to_default() {
    HashMapPro::HashMap<int, std::string> cache{16};
    cache.insert(1, "cached-value");

    auto it = cache.find(42);

    std::string result = (it != cache.end()) ? it->value : "default";

    CHK(result == "default");
}

// Verifies a cache hit via operator[] returns the value stored on a
// previous put.
static void cache_hit_returns_stored_value() {
    HashMapPro::HashMap<int, std::string> cache{16};

    cache[1] = "first-value";

    CHK(cache[1] == "first-value");
    CHK(cache.size() == 1);
}

// Verifies overwriting a cached entry via operator[] replaces the
// previously stored value rather than duplicating the key.
static void overwrite_replaces_cached_value() {
    HashMapPro::HashMap<int, std::string> cache{16};

    cache[1] = "stale";
    cache[1] = "fresh";

    CHK(cache[1] == "fresh");
    CHK(cache.size() == 1);
}

// Verifies clearing the cache evicts every entry and leaves it
// immediately reusable for new entries.
static void clear_evicts_all_entries() {
    HashMapPro::HashMap<int, std::string> cache{16};
    cache[1] = "a";
    cache[2] = "b";
    cache[3] = "c";

    CHK(cache.size() == 3);

    cache.clear();

    CHK(cache.size() == 0);
    CHK(cache.empty() == true);

    cache[4] = "fresh-after-clear";

    CHK(cache.size() == 1);
    CHK(cache[4] == "fresh-after-clear");
}

// Verifies a get-or-compute pattern using find() and insert() only
// performs the expensive computation once per key, on subsequent
// accesses reusing the cached result.
static void get_or_compute_only_computes_once() {
    HashMapPro::HashMap<int, int> cache{16};
    int computeCount = 0;

    for (int round = 0; round < 5; ++round) {
        auto it = cache.find(7);

        if (it == cache.end()) {
            int value = expensiveComputation(7, computeCount);
            cache.insert(7, value);
        }
    }

    CHK(computeCount == 1);
    CHK(cache.at(7) == 49);
}

// Executes all cache usage pattern test cases.
static void run_tests() {
    RUN(cache_miss_falls_back_to_default);
    RUN(cache_hit_returns_stored_value);
    RUN(overwrite_replaces_cached_value);
    RUN(clear_evicts_all_entries);
    RUN(get_or_compute_only_computes_once);
}

REGISTER_TEST_SUITE();
