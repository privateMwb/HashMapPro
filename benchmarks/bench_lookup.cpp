// HashMap Lookup Benchmark Suite
// Measures lookup performance (find and contains) compared to std::unordered_map
// for both successful and unsuccessful queries.
//
// Covers:
// - find operation on existing keys
// - find operation on missing keys
// - contains/count operation on existing keys
// - contains/count operation on missing keys

#include "bench_helper.h"

#include <unordered_map>

using namespace HashMapPro;

using Map = HashMap<int, int>;
using StdMap = std::unordered_map<int, int>;

// Measures find() performance for a key that exists in the map.
static void bench_find_hit() {
    Map map{128};
    StdMap stdMap;
    stdMap.reserve(128);

    for (int i = 0; i < 64; ++i) {
        map.insert(i, i);
        stdMap.insert({i, i});
    }

    BENCH("hashmap_find_hit", LARGE, [&] {
        auto it = map.find(32);
        doNotOptimize(it);
    });

    BENCH("std_find_hit", LARGE, [&] {
        auto it = stdMap.find(32);
        doNotOptimize(it);
    });
}

// Measures find() performance for a key that does not exist in the map.
static void bench_find_miss() {
    Map map{128};
    StdMap stdMap;
    stdMap.reserve(128);

    for (int i = 0; i < 64; ++i) {
        map.insert(i, i);
        stdMap.insert({i, i});
    }

    BENCH("hashmap_find_miss", LARGE, [&] {
        auto it = map.find(9999);
        doNotOptimize(it);
    });

    BENCH("std_find_miss", LARGE, [&] {
        auto it = stdMap.find(9999);
        doNotOptimize(it);
    });
}

// Measures contains/count performance for a key that exists in the map.
static void bench_contains_hit() {
    Map map{128};
    StdMap stdMap;
    stdMap.reserve(128);

    for (int i = 0; i < 64; ++i) {
        map.insert(i, i);
        stdMap.insert({i, i});
    }

    BENCH("hashmap_contains_hit", LARGE, [&] {
        doNotOptimize(map.contains(32));
    });

    BENCH("std_contains_hit", LARGE, [&] {
        doNotOptimize(stdMap.count(32));
    });
}

// Measures contains/count performance for a key that does not exist in the map.
static void bench_contains_miss() {
    Map map{128};
    StdMap stdMap;
    stdMap.reserve(128);

    for (int i = 0; i < 64; ++i) {
        map.insert(i, i);
        stdMap.insert({i, i});
    }

    BENCH("hashmap_contains_miss", LARGE, [&] {
        doNotOptimize(map.contains(9999));
    });

    BENCH("std_contains_miss", LARGE, [&] {
        doNotOptimize(stdMap.count(9999));
    });
}

// Executes all lookup benchmark cases.
void run_lookup_benchmarks() {
    setHeader("Lookup Benchmarks");

    bench_find_hit();
    std::cout << "\n";

    bench_find_miss();
    std::cout << "\n";

    bench_contains_hit();
    std::cout << "\n";

    bench_contains_miss();
    borderLine();
    std::cout << "\n";
}