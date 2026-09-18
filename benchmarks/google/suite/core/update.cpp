// HashMap Update Benchmark Suite
// Measures update() performance against std::unordered_map.
//
// Covers:
// - updating an existing key
// - updating a missing key (no-op)

#include <HashMapPro/HashMap.h>

#include <benchmark/benchmark.h>
#include <unordered_map>

using namespace HashMapPro;

using Map = HashMap<int, int>;
using StdMap = std::unordered_map<int, int>;

// Measures update() performance for a key that exists in the map (HashMapPro).
static void update_existing_hmp(benchmark::State& state) {
    Map map{128};
    for (int i = 0; i < 64; ++i) {
        map.insert(i, i);
    }

    for (auto _ : state) {
        benchmark::DoNotOptimize(map.update(32, 999));
    }
}
BENCHMARK(update_existing_hmp);

// Measures update() performance for a key that exists in the map (std::unordered_map).
static void update_existing_std(benchmark::State& state) {
    StdMap stdMap;
    stdMap.reserve(128);
    for (int i = 0; i < 64; ++i) {
        stdMap.insert({i, i});
    }

    for (auto _ : state) {
        auto it = stdMap.find(32);
        bool updated = it != stdMap.end();

        if (updated) {
            it->second = 999;
        }

        benchmark::DoNotOptimize(updated);
    }
}
BENCHMARK(update_existing_std);

// Measures update() performance for a key that does not exist in the map (HashMapPro).
static void update_missing_hmp(benchmark::State& state) {
    Map map{128};
    for (int i = 0; i < 64; ++i) {
        map.insert(i, i);
    }

    for (auto _ : state) {
        benchmark::DoNotOptimize(map.update(9999, 999));
    }
}
BENCHMARK(update_missing_hmp);

// Measures update() performance for a key that does not exist in the map (std::unordered_map).
static void update_missing_std(benchmark::State& state) {
    StdMap stdMap;
    stdMap.reserve(128);
    for (int i = 0; i < 64; ++i) {
        stdMap.insert({i, i});
    }

    for (auto _ : state) {
        auto it = stdMap.find(9999);
        bool updated = it != stdMap.end();

        if (updated) {
            it->second = 999;
        }

        benchmark::DoNotOptimize(updated);
    }
}
BENCHMARK(update_missing_std);
