// HashMap Capacity Benchmark Suite
// Measures capacity introspection performance against std::unordered_map.
//
// Covers:
// - size
// - capacity (bucket count)
// - empty

#include <common/framework.h>

#include <unordered_map>

using namespace HashMapPro;

using Map = HashMap<int, int>;
using StdMap = std::unordered_map<int, int>;

// Measures size() performance on a populated map.
static void bench_size() {
    Map map{128};
    StdMap stdMap;
    stdMap.reserve(128);

    for (int i = 0; i < 64; ++i) {
        map.insert(i, i);
        stdMap.insert({i, i});
    }

    auto hmp = [&] { doNotOptimize(map.size()); };

    auto soum = [&] { doNotOptimize(stdMap.size()); };

    BENCH("size", hmp, soum);
}

// Measures capacity()/bucket_count() performance on a populated map.
static void bench_capacity() {
    Map map{128};
    StdMap stdMap;
    stdMap.reserve(128);

    for (int i = 0; i < 64; ++i) {
        map.insert(i, i);
        stdMap.insert({i, i});
    }

    auto hmp = [&] { doNotOptimize(map.capacity()); };

    auto soum = [&] { doNotOptimize(stdMap.bucket_count()); };

    BENCH("capacity", hmp, soum);
}

// Measures empty() performance on a populated map.
static void bench_empty() {
    Map map{128};
    StdMap stdMap;
    stdMap.reserve(128);

    for (int i = 0; i < 64; ++i) {
        map.insert(i, i);
        stdMap.insert({i, i});
    }

    auto hmp = [&] { doNotOptimize(map.empty()); };

    auto soum = [&] { doNotOptimize(stdMap.empty()); };

    BENCH("empty", hmp, soum);
}

// Executes all capacity benchmark cases.
static void run_benchmarks() {
    bench_size();
    std::cout << "\n";

    bench_capacity();
    std::cout << "\n";

    bench_empty();
}

REGISTER_BENCH_SUITE();
