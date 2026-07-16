// HashMap Access Benchmark Suite
// Measures element access performance against std::unordered_map.
//
// Covers:
// - operator[] on an existing key
// - operator[] inserting a missing key
// - at() on an existing key

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

    auto hmp = [&] { doNotOptimize(map[32]); };

    auto soum = [&] { doNotOptimize(stdMap[32]); };

    BENCH("subscript existing", hmp, soum);
}

// Measures operator[] performance when inserting a missing key.
static void bench_subscript_missing() {
    auto hmp = [&] {
        Map map{2};
        doNotOptimize(map[1]);
    };

    auto soum = [&] {
        StdMap map;
        map.reserve(2);
        doNotOptimize(map[1]);
    };

    BENCH("subscript missing", hmp, soum);
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

    auto hmp = [&] { doNotOptimize(map.at(32)); };

    auto soum = [&] { doNotOptimize(stdMap.at(32)); };

    BENCH("at existing", hmp, soum);
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
