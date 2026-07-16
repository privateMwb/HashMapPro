// HashMap Rehash Benchmark Suite
// Measures the cost of a triggered rehash against std::unordered_map,
// isolated by comparing against an equal-sized insert workload that
// doesn't cross the load factor threshold.
//
// Covers:
// - insertion that triggers a rehash
// - equal-sized insertion that does not trigger a rehash

#include <common/framework.h>

#include <unordered_map>

using namespace HashMapPro;

using Map = HashMap<int, int>;
using StdMap = std::unordered_map<int, int>;

// Measures insertion that crosses the load factor threshold, forcing
// exactly one rehash (128 -> 256 buckets).
static void bench_rehash_trigger() {
    auto hmp = [&] {
        Map map{128};

        for (int i = 0; i < 100; ++i) {
            map.insert(i, i);
        }

        doNotOptimize(map);
    };

    auto soum = [&] {
        StdMap map;
        map.max_load_factor(0.75f);
        map.rehash(128);

        for (int i = 0; i < 100; ++i) {
            map.insert({i, i});
        }

        doNotOptimize(map);
    };

    BENCH("rehash trigger", hmp, soum);
}

// Measures the same insertion count into a map already sized large
// enough that no rehash occurs, isolating the rehash's marginal cost.
static void bench_no_rehash_baseline() {
    auto hmp = [&] {
        Map map{256};

        for (int i = 0; i < 100; ++i) {
            map.insert(i, i);
        }

        doNotOptimize(map);
    };

    auto soum = [&] {
        StdMap map;
        map.max_load_factor(0.75f);
        map.rehash(256);

        for (int i = 0; i < 100; ++i) {
            map.insert({i, i});
        }

        doNotOptimize(map);
    };

    BENCH("no rehash baseline", hmp, soum);
}

// Executes all rehash benchmark cases.
static void run_benchmarks() {
    bench_rehash_trigger();
    std::cout << "\n";

    bench_no_rehash_baseline();
}

REGISTER_BENCH_SUITE();
