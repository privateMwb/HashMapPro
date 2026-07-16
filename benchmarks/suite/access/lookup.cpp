// HashMap Lookup Benchmark Suite
// Measures find() and contains() performance against std::unordered_map
// for both successful and unsuccessful queries.
//
// Covers:
// - find on an existing key
// - find on a missing key
// - contains on an existing key
// - contains on a missing key

#include <common/framework.h>

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

    auto hmp = [&] {
        auto it = map.find(32);
        doNotOptimize(it);
    };

    auto soum = [&] {
        auto it = stdMap.find(32);
        doNotOptimize(it);
    };

    BENCH("find hit", hmp, soum);
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

    auto hmp = [&] {
        auto it = map.find(9999);
        doNotOptimize(it);
    };

    auto soum = [&] {
        auto it = stdMap.find(9999);
        doNotOptimize(it);
    };

    BENCH("find miss", hmp, soum);
}

// Measures contains() performance for a key that exists in the map.
static void bench_contains_hit() {
    Map map{128};
    StdMap stdMap;
    stdMap.reserve(128);

    for (int i = 0; i < 64; ++i) {
        map.insert(i, i);
        stdMap.insert({i, i});
    }

    auto hmp = [&] { doNotOptimize(map.contains(32)); };

    auto soum = [&] { doNotOptimize(stdMap.count(32)); };

    BENCH("contains hit", hmp, soum);
}

// Measures contains() performance for a key that does not exist in the map.
static void bench_contains_miss() {
    Map map{128};
    StdMap stdMap;
    stdMap.reserve(128);

    for (int i = 0; i < 64; ++i) {
        map.insert(i, i);
        stdMap.insert({i, i});
    }

    auto hmp = [&] { doNotOptimize(map.contains(9999)); };

    auto soum = [&] { doNotOptimize(stdMap.count(9999)); };

    BENCH("contains miss", hmp, soum);
}

// Executes all lookup benchmark cases.
static void run_benchmarks() {
    bench_find_hit();
    std::cout << "\n";

    bench_find_miss();
    std::cout << "\n";

    bench_contains_hit();
    std::cout << "\n";

    bench_contains_miss();
}

REGISTER_BENCH_SUITE();
