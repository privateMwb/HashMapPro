// HashMap Lookup Benchmark Suite
// Measures find() and contains() performance against std::unordered_map
// for both successful and unsuccessful queries.
//
// Covers:
// - find on an existing key
// - find on a missing key
// - contains on an existing key
// - contains on a missing key

#include <HashMapPro/HashMap.h>

#include <benchmark/benchmark.h>
#include <unordered_map>

using namespace HashMapPro;

using Map = HashMap<int, int>;
using StdMap = std::unordered_map<int, int>;

// Measures find() performance for a key that exists in the map (HashMapPro).
static void find_hit_hmp(benchmark::State& state) {
    Map map{128};
    for (int i = 0; i < 64; ++i) {
        map.insert(i, i);
    }

    for (auto _ : state) {
        auto it = map.find(32);
        benchmark::DoNotOptimize(it);
    }
}
BENCHMARK(find_hit_hmp);

// Measures find() performance for a key that exists in the map (std::unordered_map).
static void find_hit_std(benchmark::State& state) {
    StdMap stdMap;
    stdMap.reserve(128);
    for (int i = 0; i < 64; ++i) {
        stdMap.insert({i, i});
    }

    for (auto _ : state) {
        auto it = stdMap.find(32);
        benchmark::DoNotOptimize(it);
    }
}
BENCHMARK(find_hit_std);

// Measures find() performance for a key that does not exist in the map (HashMapPro).
static void find_miss_hmp(benchmark::State& state) {
    Map map{128};
    for (int i = 0; i < 64; ++i) {
        map.insert(i, i);
    }

    for (auto _ : state) {
        auto it = map.find(9999);
        benchmark::DoNotOptimize(it);
    }
}
BENCHMARK(find_miss_hmp);

// Measures find() performance for a key that does not exist in the map (std::unordered_map).
static void find_miss_std(benchmark::State& state) {
    StdMap stdMap;
    stdMap.reserve(128);
    for (int i = 0; i < 64; ++i) {
        stdMap.insert({i, i});
    }

    for (auto _ : state) {
        auto it = stdMap.find(9999);
        benchmark::DoNotOptimize(it);
    }
}
BENCHMARK(find_miss_std);

// Measures contains() performance for a key that exists in the map (HashMapPro).
static void contains_hit_hmp(benchmark::State& state) {
    Map map{128};
    for (int i = 0; i < 64; ++i) {
        map.insert(i, i);
    }

    for (auto _ : state) {
        benchmark::DoNotOptimize(map.contains(32));
    }
}
BENCHMARK(contains_hit_hmp);

// Measures contains() performance for a key that exists in the map (std::unordered_map).
static void contains_hit_std(benchmark::State& state) {
    StdMap stdMap;
    stdMap.reserve(128);
    for (int i = 0; i < 64; ++i) {
        stdMap.insert({i, i});
    }

    for (auto _ : state) {
        benchmark::DoNotOptimize(stdMap.count(32));
    }
}
BENCHMARK(contains_hit_std);

// Measures contains() performance for a key that does not exist in the map (HashMapPro).
static void contains_miss_hmp(benchmark::State& state) {
    Map map{128};
    for (int i = 0; i < 64; ++i) {
        map.insert(i, i);
    }

    for (auto _ : state) {
        benchmark::DoNotOptimize(map.contains(9999));
    }
}
BENCHMARK(contains_miss_hmp);

// Measures contains() performance for a key that does not exist in the map (std::unordered_map).
static void contains_miss_std(benchmark::State& state) {
    StdMap stdMap;
    stdMap.reserve(128);
    for (int i = 0; i < 64; ++i) {
        stdMap.insert({i, i});
    }

    for (auto _ : state) {
        benchmark::DoNotOptimize(stdMap.count(9999));
    }
}
BENCHMARK(contains_miss_std);
