// HashMap Load Factor Benchmark Suite
// Measures find() lookup performance against std::unordered_map at a
// sparse vs. a dense load factor, with bucket count held fixed so the
// only variable between cases is chain length.
//
// Covers:
// - lookup at a sparse load factor
// - lookup at a dense load factor (just under the 0.75 threshold)

#include <HashMapPro/HashMap.h>

#include <benchmark/benchmark.h>
#include <unordered_map>

using namespace HashMapPro;

using Map = HashMap<int, int>;
using StdMap = std::unordered_map<int, int>;

// Measures find() performance with the table sparsely populated (HashMapPro).
static void lookup_sparse_hmp(benchmark::State& state) {
    Map map{1024};
    for (int i = 0; i < 100; ++i) {
        map.insert(i, i);
    }

    for (auto _ : state) {
        auto it = map.find(50);
        benchmark::DoNotOptimize(it);
    }
}
BENCHMARK(lookup_sparse_hmp);

// Measures find() performance with the table sparsely populated (std::unordered_map).
static void lookup_sparse_std(benchmark::State& state) {
    StdMap stdMap;
    stdMap.max_load_factor(0.75f);
    stdMap.rehash(1024);
    for (int i = 0; i < 100; ++i) {
        stdMap.insert({i, i});
    }

    for (auto _ : state) {
        auto it = stdMap.find(50);
        benchmark::DoNotOptimize(it);
    }
}
BENCHMARK(lookup_sparse_std);

// Measures find() performance with the table densely populated, just under
// the 0.75 rehash threshold (HashMapPro).
static void lookup_dense_hmp(benchmark::State& state) {
    Map map{1024};
    for (int i = 0; i < 750; ++i) {
        map.insert(i, i);
    }

    for (auto _ : state) {
        auto it = map.find(375);
        benchmark::DoNotOptimize(it);
    }
}
BENCHMARK(lookup_dense_hmp);

// Measures find() performance with the table densely populated, just under
// the 0.75 rehash threshold (std::unordered_map).
static void lookup_dense_std(benchmark::State& state) {
    StdMap stdMap;
    stdMap.max_load_factor(0.75f);
    stdMap.rehash(1024);
    for (int i = 0; i < 750; ++i) {
        stdMap.insert({i, i});
    }

    for (auto _ : state) {
        auto it = stdMap.find(375);
        benchmark::DoNotOptimize(it);
    }
}
BENCHMARK(lookup_dense_std);
