// HashMap Iteration Benchmark Suite
// Measures iteration performance compared to std::unordered_map
// across different traversal patterns and map sizes.
//
// Covers:
// - forward traversal of a small map
// - forward traversal of a large map
// - reverse traversal

#include <common/framework.h>

#include <unordered_map>

using namespace HashMapPro;

using Map = HashMap<int, int>;
using StdMap = std::unordered_map<int, int>;

// Measures forward iteration performance over a small populated map.
static void bench_traverse_small() {
    Map map{64};
    StdMap stdMap;
    stdMap.reserve(64);

    for (int i = 0; i < 32; ++i) {
        map.insert(i, i);
        stdMap.insert({i, i});
    }

    BENCH("hashmap_traverse_small", MEDIUM, [&] {
        int sum = 0;

        for (auto it = map.begin(); it != map.end(); ++it) {
            sum += it->value;
        }

        doNotOptimize(sum);
    });

    BENCH("std_traverse_small", MEDIUM, [&] {
        int sum = 0;

        for (auto it = stdMap.begin(); it != stdMap.end(); ++it) {
            sum += it->second;
        }

        doNotOptimize(sum);
    });
}

// Measures forward iteration performance over a large populated map.
static void bench_traverse_large() {
    Map map{2048};
    StdMap stdMap;
    stdMap.reserve(2048);

    for (int i = 0; i < 1024; ++i) {
        map.insert(i, i);
        stdMap.insert({i, i});
    }

    BENCH("hashmap_traverse_large", SMALL, [&] {
        int sum = 0;

        for (auto it = map.begin(); it != map.end(); ++it) {
            sum += it->value;
        }

        doNotOptimize(sum);
    });

    BENCH("std_traverse_large", SMALL, [&] {
        int sum = 0;

        for (auto it = stdMap.begin(); it != stdMap.end(); ++it) {
            sum += it->second;
        }

        doNotOptimize(sum);
    });
}

// Measures reverse iteration performance.
static void bench_traverse_reverse() {
    Map map{64};
    StdMap stdMap;
    stdMap.reserve(64);

    for (int i = 0; i < 32; ++i) {
        map.insert(i, i);
        stdMap.insert({i, i});
    }

    BENCH("hashmap_traverse_reverse", MEDIUM, [&] {
        int sum = 0;

        for (auto it = map.rbegin(); it != map.rend(); ++it) {
            sum += it->value;
        }

        doNotOptimize(sum);
    });

    BENCH("std_traverse_forward_baseline", MEDIUM, [&] {
        int sum = 0;

        for (auto it = stdMap.begin(); it != stdMap.end(); ++it) {
            sum += it->second;
        }

        doNotOptimize(sum);
    });
}

// Executes all iteration benchmark cases.
static void run_benchmarks() {
    bench_traverse_small();
    std::cout << "\n";

    bench_traverse_large();
    std::cout << "\n";

    bench_traverse_reverse();
}

REGISTER_BENCH_SUITE();