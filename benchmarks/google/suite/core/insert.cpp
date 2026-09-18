// HashMap Insert Benchmark Suite
// Measures insert() performance against std::unordered_map.
//
// Covers:
// - inserting a new key
// - inserting an already-present key (no-op)

#include <HashMapPro/HashMap.h>

#include <benchmark/benchmark.h>
#include <unordered_map>

using namespace HashMapPro;

using Map = HashMap<int, int>;
using StdMap = std::unordered_map<int, int>;

// Measures insert() performance for a key not yet in the map (HashMapPro).
static void insert_new_hmp(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        Map map{128};
        state.ResumeTiming();

        for (int i = 0; i < 64; ++i) {
            map.insert(i, i);
        }

        benchmark::DoNotOptimize(map);
    }
}
BENCHMARK(insert_new_hmp);

// Measures insert() performance for a key not yet in the map (std::unordered_map).
static void insert_new_std(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        StdMap map;
        map.reserve(128);
        state.ResumeTiming();

        for (int i = 0; i < 64; ++i) {
            map.insert({i, i});
        }

        benchmark::DoNotOptimize(map);
    }
}
BENCHMARK(insert_new_std);

// Measures insert() performance for a key already present in the map (HashMapPro).
static void insert_existing_hmp(benchmark::State& state) {
    Map map{128};
    for (int i = 0; i < 64; ++i) {
        map.insert(i, i);
    }

    for (auto _ : state) {
        map.insert(32, 999);
    }
}
BENCHMARK(insert_existing_hmp);

// Measures insert() performance for a key already present in the map (std::unordered_map).
static void insert_existing_std(benchmark::State& state) {
    StdMap stdMap;
    stdMap.reserve(128);
    for (int i = 0; i < 64; ++i) {
        stdMap.insert({i, i});
    }

    for (auto _ : state) {
        stdMap.insert({32, 999});
    }
}
BENCHMARK(insert_existing_std);
