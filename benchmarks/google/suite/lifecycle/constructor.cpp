// HashMap Construction Benchmark Suite
// Measures construction performance against std::unordered_map.
//
// Covers:
// - default construction
// - reserved construction
// - populate construction

#include <HashMapPro/HashMap.h>

#include <benchmark/benchmark.h>
#include <unordered_map>

using namespace HashMapPro;

using Map = HashMap<int, int>;
using StdMap = std::unordered_map<int, int>;

// Measures default construction (HashMapPro).
static void default_construct_hmp(benchmark::State& state) {
    for (auto _ : state) {
        Map map;
        benchmark::DoNotOptimize(map);
    }
}
BENCHMARK(default_construct_hmp);

// Measures default construction (std::unordered_map).
static void default_construct_std(benchmark::State& state) {
    for (auto _ : state) {
        StdMap map;
        benchmark::DoNotOptimize(map);
    }
}
BENCHMARK(default_construct_std);

// Measures construction with a reserved bucket count (HashMapPro).
static void reserved_construct_hmp(benchmark::State& state) {
    for (auto _ : state) {
        Map map{100};
        benchmark::DoNotOptimize(map);
    }
}
BENCHMARK(reserved_construct_hmp);

// Measures construction with a reserved bucket count (std::unordered_map).
static void reserved_construct_std(benchmark::State& state) {
    for (auto _ : state) {
        StdMap map;
        map.reserve(100);
        benchmark::DoNotOptimize(map);
    }
}
BENCHMARK(reserved_construct_std);

// Measures construction followed by populating a handful of elements (HashMapPro).
static void populate_construct_hmp(benchmark::State& state) {
    for (auto _ : state) {
        Map map;

        for (int i = 0; i < 5; ++i) {
            map.insert(i, i);
        }

        benchmark::DoNotOptimize(map);
    }
}
BENCHMARK(populate_construct_hmp);

// Measures construction followed by populating a handful of elements (std::unordered_map).
static void populate_construct_std(benchmark::State& state) {
    for (auto _ : state) {
        StdMap map;

        for (int i = 0; i < 5; ++i) {
            map.insert({i, i});
        }

        benchmark::DoNotOptimize(map);
    }
}
BENCHMARK(populate_construct_std);
