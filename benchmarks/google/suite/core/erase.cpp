// HashMap Erase Benchmark Suite
// Measures erase() performance against std::unordered_map. Each case
// rebuilds the map fresh per iteration, since erasing mutates state
// that later iterations depend on.
//
// Covers:
// - erasing an existing key
// - erasing a missing key (no-op)

#include <HashMapPro/HashMap.h>

#include <benchmark/benchmark.h>
#include <unordered_map>

using namespace HashMapPro;

using Map = HashMap<int, int>;
using StdMap = std::unordered_map<int, int>;

// Measures erase() performance for a key that exists in the map (HashMapPro).
static void erase_existing_hmp(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        Map map{128};
        for (int i = 0; i < 64; ++i) {
            map.insert(i, i);
        }
        state.ResumeTiming();

        benchmark::DoNotOptimize(map.erase(32));
    }
}
BENCHMARK(erase_existing_hmp);

// Measures erase() performance for a key that exists in the map (std::unordered_map).
static void erase_existing_std(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        StdMap map;
        map.reserve(128);
        for (int i = 0; i < 64; ++i) {
            map.insert({i, i});
        }
        state.ResumeTiming();

        benchmark::DoNotOptimize(map.erase(32));
    }
}
BENCHMARK(erase_existing_std);

// Measures erase() performance for a key that does not exist in the map (HashMapPro).
static void erase_missing_hmp(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        Map map{128};
        for (int i = 0; i < 64; ++i) {
            map.insert(i, i);
        }
        state.ResumeTiming();

        benchmark::DoNotOptimize(map.erase(9999));
    }
}
BENCHMARK(erase_missing_hmp);

// Measures erase() performance for a key that does not exist in the map (std::unordered_map).
static void erase_missing_std(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        StdMap map;
        map.reserve(128);
        for (int i = 0; i < 64; ++i) {
            map.insert({i, i});
        }
        state.ResumeTiming();

        benchmark::DoNotOptimize(map.erase(9999));
    }
}
BENCHMARK(erase_missing_std);
