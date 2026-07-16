// HashMap Load Factor Benchmark Suite
// Measures find() lookup performance against std::unordered_map at a
// sparse vs. a dense load factor, with bucket count held fixed so the
// only variable between cases is chain length.
//
// Covers:
// - lookup at a sparse load factor
// - lookup at a dense load factor (just under the 0.75 threshold)

#include <common/framework.h>

#include <unordered_map>

using namespace HashMapPro;

using Map = HashMap<int, int>;
using StdMap = std::unordered_map<int, int>;

// Measures find() performance with the table sparsely populated.
static void bench_lookup_sparse() {
    Map map{1024};
    StdMap stdMap;
    stdMap.max_load_factor(0.75f);
    stdMap.rehash(1024);

    for (int i = 0; i < 100; ++i) {
        map.insert(i, i);
        stdMap.insert({i, i});
    }

    auto hmp = [&] {
        auto it = map.find(50);
        doNotOptimize(it);
    };

    auto soum = [&] {
        auto it = stdMap.find(50);
        doNotOptimize(it);
    };

    BENCH("lookup sparse", hmp, soum);
}

// Measures find() performance with the table densely populated, just
// under the 0.75 rehash threshold.
static void bench_lookup_dense() {
    Map map{1024};
    StdMap stdMap;
    stdMap.max_load_factor(0.75f);
    stdMap.rehash(1024);

    for (int i = 0; i < 750; ++i) {
        map.insert(i, i);
        stdMap.insert({i, i});
    }

    auto hmp = [&] {
        auto it = map.find(375);
        doNotOptimize(it);
    };

    auto soum = [&] {
        auto it = stdMap.find(375);
        doNotOptimize(it);
    };

    BENCH("lookup dense", hmp, soum);
}

// Executes all load factor benchmark cases.
static void run_benchmarks() {
    bench_lookup_sparse();
    std::cout << "\n";

    bench_lookup_dense();
}

REGISTER_BENCH_SUITE();
