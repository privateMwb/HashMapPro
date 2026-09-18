// HashMap Clear Benchmark Suite
// Measures clear() performance against std::unordered_map, isolated by
// comparing an empty map against an equally-sized populated map.
//
// Covers:
// - clearing an empty map
// - clearing a populated map

#include <HashMapPro/HashMap.h>

#include <benchmark/benchmark.h>
#include <unordered_map>

using namespace HashMapPro;

using Map = HashMap<int, int>;
using StdMap = std::unordered_map<int, int>;

// Measures clear() performance on an empty map (HashMapPro).
static void clear_empty_hmp(benchmark::State& state) {
    Map map{128};

    for (auto _ : state) {
        map.clear();
    }
}
BENCHMARK(clear_empty_hmp);

// Measures clear() performance on an empty map (std::unordered_map).
static void clear_empty_std(benchmark::State& state) {
    StdMap stdMap;
    stdMap.reserve(128);

    for (auto _ : state) {
        stdMap.clear();
    }
}
BENCHMARK(clear_empty_std);

// Measures clear() performance on a populated map, rebuilt fresh per
// iteration since clearing mutates state that later iterations depend on
// (HashMapPro).
static void clear_populated_hmp(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        Map map{128};
        for (int i = 0; i < 64; ++i) {
            map.insert(i, i);
        }
        state.ResumeTiming();

        map.clear();
        benchmark::DoNotOptimize(map);
    }
}
BENCHMARK(clear_populated_hmp);

// Measures clear() performance on a populated map, rebuilt fresh per
// iteration since clearing mutates state that later iterations depend on
// (std::unordered_map).
static void clear_populated_std(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        StdMap map;
        map.reserve(128);
        for (int i = 0; i < 64; ++i) {
            map.insert({i, i});
        }
        state.ResumeTiming();

        map.clear();
        benchmark::DoNotOptimize(map);
    }
}
BENCHMARK(clear_populated_std);
