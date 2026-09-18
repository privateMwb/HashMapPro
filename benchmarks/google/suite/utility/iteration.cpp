// HashMap Iteration Benchmark Suite
// Measures traversal performance against std::unordered_map.
//
// Covers:
// - forward traversal
// - reverse traversal

#include <HashMapPro/HashMap.h>

#include <benchmark/benchmark.h>
#include <unordered_map>

using namespace HashMapPro;

using Map = HashMap<int, int>;
using StdMap = std::unordered_map<int, int>;

// Measures forward iteration performance over a populated map (HashMapPro).
static void forward_traverse_hmp(benchmark::State& state) {
    Map map{128};
    for (int i = 0; i < 64; ++i) {
        map.insert(i, i);
    }

    for (auto _ : state) {
        int sum = 0;

        for (auto it = map.begin(); it != map.end(); ++it) {
            sum += it->value;
        }

        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(forward_traverse_hmp);

// Measures forward iteration performance over a populated map (std::unordered_map).
static void forward_traverse_std(benchmark::State& state) {
    StdMap stdMap;
    stdMap.reserve(128);
    for (int i = 0; i < 64; ++i) {
        stdMap.insert({i, i});
    }

    for (auto _ : state) {
        int sum = 0;

        for (auto it = stdMap.begin(); it != stdMap.end(); ++it) {
            sum += it->second;
        }

        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(forward_traverse_std);

// Measures reverse iteration performance over a populated map (HashMapPro).
// Compared against std::unordered_map's forward traversal, since it has no
// reverse iterators -- this is the closest available baseline, not a
// like-for-like comparison.
static void reverse_traverse_hmp(benchmark::State& state) {
    Map map{128};
    for (int i = 0; i < 64; ++i) {
        map.insert(i, i);
    }

    for (auto _ : state) {
        int sum = 0;

        for (auto it = map.rbegin(); it != map.rend(); ++it) {
            sum += it->value;
        }

        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(reverse_traverse_hmp);

// Baseline for reverse_traverse_hmp: std::unordered_map's forward
// traversal, since it has no reverse iterators.
static void reverse_traverse_std(benchmark::State& state) {
    StdMap stdMap;
    stdMap.reserve(128);
    for (int i = 0; i < 64; ++i) {
        stdMap.insert({i, i});
    }

    for (auto _ : state) {
        int sum = 0;

        for (auto it = stdMap.begin(); it != stdMap.end(); ++it) {
            sum += it->second;
        }

        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(reverse_traverse_std);
