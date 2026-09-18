// HashMap Rehash Benchmark Suite
// Measures the cost of a triggered rehash against std::unordered_map,
// isolated by comparing against an equal-sized insert workload that
// doesn't cross the load factor threshold.
//
// Covers:
// - insertion that triggers a rehash
// - equal-sized insertion that does not trigger a rehash

#include <HashMapPro/HashMap.h>

#include <benchmark/benchmark.h>
#include <unordered_map>

using namespace HashMapPro;

using Map = HashMap<int, int>;
using StdMap = std::unordered_map<int, int>;

// Measures insertion that crosses the load factor threshold, forcing
// exactly one rehash (128 -> 256 buckets) (HashMapPro).
static void rehash_trigger_hmp(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        Map map{128};
        state.ResumeTiming();

        for (int i = 0; i < 100; ++i) {
            map.insert(i, i);
        }

        benchmark::DoNotOptimize(map);
    }
}
BENCHMARK(rehash_trigger_hmp);

// Measures insertion that crosses the load factor threshold, forcing
// exactly one rehash (128 -> 256 buckets) (std::unordered_map).
static void rehash_trigger_std(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        StdMap map;
        map.max_load_factor(0.75f);
        map.rehash(128);
        state.ResumeTiming();

        for (int i = 0; i < 100; ++i) {
            map.insert({i, i});
        }

        benchmark::DoNotOptimize(map);
    }
}
BENCHMARK(rehash_trigger_std);

// Measures the same insertion count into a map already sized large enough
// that no rehash occurs, isolating the rehash's marginal cost (HashMapPro).
static void no_rehash_baseline_hmp(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        Map map{256};
        state.ResumeTiming();

        for (int i = 0; i < 100; ++i) {
            map.insert(i, i);
        }

        benchmark::DoNotOptimize(map);
    }
}
BENCHMARK(no_rehash_baseline_hmp);

// Measures the same insertion count into a map already sized large enough
// that no rehash occurs, isolating the rehash's marginal cost (std::unordered_map).
static void no_rehash_baseline_std(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        StdMap map;
        map.max_load_factor(0.75f);
        map.rehash(256);
        state.ResumeTiming();

        for (int i = 0; i < 100; ++i) {
            map.insert({i, i});
        }

        benchmark::DoNotOptimize(map);
    }
}
BENCHMARK(no_rehash_baseline_std);
