// HashMap Iteration Benchmark Suite
// Measures traversal performance against std::unordered_map.
//
// Covers:
// - forward traversal
// - reverse traversal

#include <common/framework.h>

#include <unordered_map>

using namespace HashMapPro;

using Map = HashMap<int, int>;
using StdMap = std::unordered_map<int, int>;

// Measures forward iteration performance over a populated map.
static void bench_forward_traverse() {
    Map map{128};
    StdMap stdMap;
    stdMap.reserve(128);

    for (int i = 0; i < 64; ++i) {
        map.insert(i, i);
        stdMap.insert({i, i});
    }

    auto hmp = [&] {
        int sum = 0;

        for (auto it = map.begin(); it != map.end(); ++it) {
            sum += it->value;
        }

        doNotOptimize(sum);
    };

    auto soum = [&] {
        int sum = 0;

        for (auto it = stdMap.begin(); it != stdMap.end(); ++it) {
            sum += it->second;
        }

        doNotOptimize(sum);
    };

    BENCH("forward traverse", hmp, soum);
}

// Measures reverse iteration performance over a populated map. Compared
// against std::unordered_map's forward traversal, since it has no
// reverse iterators — this is the closest available baseline, not a
// like-for-like comparison.
static void bench_reverse_traverse() {
    Map map{128};
    StdMap stdMap;
    stdMap.reserve(128);

    for (int i = 0; i < 64; ++i) {
        map.insert(i, i);
        stdMap.insert({i, i});
    }

    auto hmp = [&] {
        int sum = 0;

        for (auto it = map.rbegin(); it != map.rend(); ++it) {
            sum += it->value;
        }

        doNotOptimize(sum);
    };

    auto soum = [&] {
        int sum = 0;

        for (auto it = stdMap.begin(); it != stdMap.end(); ++it) {
            sum += it->second;
        }

        doNotOptimize(sum);
    };

    BENCH("reverse traverse", hmp, soum);
}

// Executes all iteration benchmark cases.
static void run_benchmarks() {
    bench_forward_traverse();
    std::cout << "\n";

    bench_reverse_traverse();
}

REGISTER_BENCH_SUITE();
