// HashMap Access Benchmark Suite
// Measures element access performance against std::unordered_map.
//
// Covers:
// - operator[] on an existing key
// - operator[] inserting a missing key
// - at() on an existing key

#include <HashMapPro/HashMap.h>

#include <benchmark/benchmark.h>
#include <unordered_map>

using namespace HashMapPro;

using Map = HashMap<int, int>;
using StdMap = std::unordered_map<int, int>;

// Measures operator[] performance for an existing key (HashMapPro).
static void subscript_existing_hmp(benchmark::State& state) {
    Map map{128};
    for (int i = 0; i < 64; ++i) {
        map.insert(i, i);
    }

    for (auto _ : state) {
        benchmark::DoNotOptimize(map[32]);
    }
}
BENCHMARK(subscript_existing_hmp);

// Measures operator[] performance for an existing key (std::unordered_map).
static void subscript_existing_std(benchmark::State& state) {
    StdMap stdMap;
    stdMap.reserve(128);
    for (int i = 0; i < 64; ++i) {
        stdMap.insert({i, i});
    }

    for (auto _ : state) {
        benchmark::DoNotOptimize(stdMap[32]);
    }
}
BENCHMARK(subscript_existing_std);

// Measures operator[] performance when inserting a missing key (HashMapPro).
static void subscript_missing_hmp(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        Map map{2};
        state.ResumeTiming();

        benchmark::DoNotOptimize(map[1]);
    }
}
BENCHMARK(subscript_missing_hmp);

// Measures operator[] performance when inserting a missing key (std::unordered_map).
static void subscript_missing_std(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        StdMap map;
        map.reserve(2);
        state.ResumeTiming();

        benchmark::DoNotOptimize(map[1]);
    }
}
BENCHMARK(subscript_missing_std);

// Measures at() performance for an existing key (HashMapPro).
static void at_existing_hmp(benchmark::State& state) {
    Map map{128};
    for (int i = 0; i < 64; ++i) {
        map.insert(i, i);
    }

    for (auto _ : state) {
        benchmark::DoNotOptimize(map.at(32));
    }
}
BENCHMARK(at_existing_hmp);

// Measures at() performance for an existing key (std::unordered_map).
static void at_existing_std(benchmark::State& state) {
    StdMap stdMap;
    stdMap.reserve(128);
    for (int i = 0; i < 64; ++i) {
        stdMap.insert({i, i});
    }

    for (auto _ : state) {
        benchmark::DoNotOptimize(stdMap.at(32));
    }
}
BENCHMARK(at_existing_std);
