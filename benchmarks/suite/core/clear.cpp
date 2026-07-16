// HashMap Clear Benchmark Suite
// Measures clear() performance against std::unordered_map, isolated by
// comparing an empty map against an equally-sized populated map.
//
// Covers:
// - clearing an empty map
// - clearing a populated map

#include <common/framework.h>

#include <unordered_map>

using namespace HashMapPro;

using Map = HashMap<int, int>;
using StdMap = std::unordered_map<int, int>;

// Measures clear() performance on an empty map.
static void bench_clear_empty() {
    Map map{128};
    StdMap stdMap;
    stdMap.reserve(128);

    auto hmp = [&] { map.clear(); };

    auto soum = [&] { stdMap.clear(); };

    BENCH("clear empty", hmp, soum);
}

// Measures clear() performance on a populated map, rebuilt fresh per
// iteration since clearing mutates state that later iterations depend on.
static void bench_clear_populated() {
    auto hmp = [&] {
        Map map{128};

        for (int i = 0; i < 64; ++i) {
            map.insert(i, i);
        }

        map.clear();
        doNotOptimize(map);
    };

    auto soum = [&] {
        StdMap map;
        map.reserve(128);

        for (int i = 0; i < 64; ++i) {
            map.insert({i, i});
        }

        map.clear();
        doNotOptimize(map);
    };

    BENCH("clear populated", hmp, soum);
}

// Executes all clear benchmark cases.
static void run_benchmarks() {
    bench_clear_empty();
    std::cout << "\n";

    bench_clear_populated();
}

REGISTER_BENCH_SUITE();
