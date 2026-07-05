// HashMap Access Benchmark Suite
// Measures element access performance compared to std::unordered_map.
//
// Covers:
// - operator[] on existing keys
// - operator[] inserting missing keys
// - at() on existing keys

#include <common/framework.h>

#include <unordered_map>

using namespace HashMapPro;

using Map = HashMap<int, int>;
using StdMap = std::unordered_map<int, int>;

// Measures operator[] performance for an existing key.
static void bench_subscript_existing() {
    Map map{128};
    StdMap stdMap;
    stdMap.reserve(128);

    for (int i = 0; i < 64; ++i) {
        map.insert(i, i);
        stdMap.insert({i, i});
    }

    BENCH("hashmap_subscript_existing", LARGE, [&] {
        doNotOptimize(map[32]);
    });

    BENCH("std_subscript_existing", LARGE, [&] {
        doNotOptimize(stdMap[32]);
    });
}

// Measures operator[] performance when inserting a missing key.
static void bench_subscript_missing() {
    BENCH("hashmap_subscript_missing", LARGE, [&] {
        Map map{2};
        doNotOptimize(map[1]);
    });

    BENCH("std_subscript_missing", LARGE, [&] {
        StdMap map;
        map.reserve(2);
        doNotOptimize(map[1]);
    });
}

// Measures at() performance for an existing key.
static void bench_at_existing() {
    Map map{128};
    StdMap stdMap;
    stdMap.reserve(128);

    for (int i = 0; i < 64; ++i) {
        map.insert(i, i);
        stdMap.insert({i, i});
    }

    BENCH("hashmap_at_existing", LARGE, [&] {
        doNotOptimize(map.at(32));
    });

    BENCH("std_at_existing", LARGE, [&] {
        doNotOptimize(stdMap.at(32));
    });
}

// Executes all access benchmark cases.
static void run_benchmarks() {
    bench_subscript_existing();
    std::cout << "\n";

    bench_subscript_missing();
    std::cout << "\n";

    bench_at_existing();
}

REGISTER_BENCH_SUITE();