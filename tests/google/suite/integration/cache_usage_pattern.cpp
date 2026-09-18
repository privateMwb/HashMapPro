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

#include <HashMapPro/HashMap.h>

#include <gtest/gtest.h>

#include <string>

// Simulates an expensive computation whose result should be cached.
static int expensiveComputation(int key, int& computeCount) {
    ++computeCount;
    return key * key;
}

// Verifies a cache miss falls back to a default value instead of
// throwing or returning garbage.
TEST(CacheUsagePattern, CacheMissFallsBackToDefault) {
    HashMapPro::HashMap<int, std::string> cache{16};
    cache.insert(1, "cached-value");

    auto it = cache.find(42);

    std::string result = (it != cache.end()) ? it->value : "default";

    EXPECT_EQ(result, "default");
}

// Verifies a cache hit via operator[] returns the value stored on a
// previous put.
TEST(CacheUsagePattern, CacheHitReturnsStoredValue) {
    HashMapPro::HashMap<int, std::string> cache{16};

    cache[1] = "first-value";

    EXPECT_EQ(cache[1], "first-value");
    EXPECT_EQ(cache.size(), 1u);
}

// Verifies overwriting a cached entry via operator[] replaces the
// previously stored value rather than duplicating the key.
TEST(CacheUsagePattern, OverwriteReplacesCachedValue) {
    HashMapPro::HashMap<int, std::string> cache{16};

    cache[1] = "stale";
    cache[1] = "fresh";

    EXPECT_EQ(cache[1], "fresh");
    EXPECT_EQ(cache.size(), 1u);
}

// Verifies clearing the cache evicts every entry and leaves it
// immediately reusable for new entries.
TEST(CacheUsagePattern, ClearEvictsAllEntries) {
    HashMapPro::HashMap<int, std::string> cache{16};
    cache[1] = "a";
    cache[2] = "b";
    cache[3] = "c";

    EXPECT_EQ(cache.size(), 3u);

    cache.clear();

    EXPECT_EQ(cache.size(), 0u);
    EXPECT_TRUE(cache.empty());

    cache[4] = "fresh-after-clear";

    EXPECT_EQ(cache.size(), 1u);
    EXPECT_EQ(cache[4], "fresh-after-clear");
}

// Verifies a get-or-compute pattern using find() and insert() only
// performs the expensive computation once per key, on subsequent
// accesses reusing the cached result.
TEST(CacheUsagePattern, GetOrComputeOnlyComputesOnce) {
    HashMapPro::HashMap<int, int> cache{16};
    int computeCount = 0;

    for (int round = 0; round < 5; ++round) {
        auto it = cache.find(7);

        if (it == cache.end()) {
            int value = expensiveComputation(7, computeCount);
            cache.insert(7, value);
        }
    }

    EXPECT_EQ(computeCount, 1);
    EXPECT_EQ(cache.at(7), 49);
}
