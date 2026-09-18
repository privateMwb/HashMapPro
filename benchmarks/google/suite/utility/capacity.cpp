// HashMap Capacity Benchmark Suite
// Measures capacity introspection performance against std::unordered_map.
//
// Covers:
// - size
// - capacity (bucket count)
// - empty

#include <HashMapPro/HashMap.h>

#include <benchmark/benchmark.h>
#include <unordered_map>

using namespace HashMapPro;

using Map = HashMap<int, int>;
using StdMap = std::unordered_map<int, int>;

// Measures size() performance on a populated map (HashMapPro).
static void size_hmp(benchmark::State& state) {
    Map map{128};
    for (int i = 0; i < 64; ++i) {
        map.insert(i, i);
    }

    for (auto _ : state) {
        benchmark::DoNotOptimize(map.size());
    }
}
BENCHMARK(size_hmp);

// Measures size() performance on a populated map (std::unordered_map).
static void size_std(benchmark::State& state) {
    StdMap stdMap;
    stdMap.reserve(128);
    for (int i = 0; i < 64; ++i) {
        stdMap.insert({i, i});
    }

    for (auto _ : state) {
        benchmark::DoNotOptimize(stdMap.size());
    }
}
BENCHMARK(size_std);

// Measures capacity()/bucket_count() performance on a populated map (HashMapPro).
static void capacity_hmp(benchmark::State& state) {
    Map map{128};
    for (int i = 0; i < 64; ++i) {
        map.insert(i, i);
    }

    for (auto _ : state) {
        benchmark::DoNotOptimize(map.capacity());
    }
}
BENCHMARK(capacity_hmp);

// Measures capacity()/bucket_count() performance on a populated map (std::unordered_map).
static void capacity_std(benchmark::State& state) {
    StdMap stdMap;
    stdMap.reserve(128);
    for (int i = 0; i < 64; ++i) {
        stdMap.insert({i, i});
    }

    for (auto _ : state) {
        benchmark::DoNotOptimize(stdMap.bucket_count());
    }
}
BENCHMARK(capacity_std);

// Measures empty() performance on a populated map (HashMapPro).
static void empty_hmp(benchmark::State& state) {
    Map map{128};
    for (int i = 0; i < 64; ++i) {
        map.insert(i, i);
    }

    for (auto _ : state) {
        benchmark::DoNotOptimize(map.empty());
    }
}
BENCHMARK(empty_hmp);

// Measures empty() performance on a populated map (std::unordered_map).
static void empty_std(benchmark::State& state) {
    StdMap stdMap;
    stdMap.reserve(128);
    for (int i = 0; i < 64; ++i) {
        stdMap.insert({i, i});
    }

    for (auto _ : state) {
        benchmark::DoNotOptimize(stdMap.empty());
    }
}
BENCHMARK(empty_std);
