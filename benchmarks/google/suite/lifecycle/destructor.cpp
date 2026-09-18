// HashMap Destructor Benchmark Suite
// Measures destruction cost against std::unordered_map, isolated by
// comparing an empty map against an equally-sized populated map. Since
// destruction can only be observed by letting a map fall out of scope,
// each case necessarily times construction plus destruction together.
//
// Covers:
// - destruction of an empty map
// - destruction of a populated map

#include <HashMapPro/HashMap.h>

#include <benchmark/benchmark.h>
#include <unordered_map>

using namespace HashMapPro;

using Map = HashMap<int, int>;
using StdMap = std::unordered_map<int, int>;

// Measures construct-destroy cost for an empty map (HashMapPro).
static void destroy_empty_hmp(benchmark::State& state) {
    for (auto _ : state) {
        Map map{128};
        benchmark::DoNotOptimize(map);
    }
}
BENCHMARK(destroy_empty_hmp);

// Measures construct-destroy cost for an empty map (std::unordered_map).
static void destroy_empty_std(benchmark::State& state) {
    for (auto _ : state) {
        StdMap map;
        map.reserve(128);
        benchmark::DoNotOptimize(map);
    }
}
BENCHMARK(destroy_empty_std);

// Measures construct-populate-destroy cost for a populated map, isolating
// the marginal cost of destroying live nodes (HashMapPro).
static void destroy_populated_hmp(benchmark::State& state) {
    for (auto _ : state) {
        Map map{128};

        for (int i = 0; i < 100; ++i) {
            map.insert(i, i);
        }

        benchmark::DoNotOptimize(map);
    }
}
BENCHMARK(destroy_populated_hmp);

// Measures construct-populate-destroy cost for a populated map, isolating
// the marginal cost of destroying live nodes (std::unordered_map).
static void destroy_populated_std(benchmark::State& state) {
    for (auto _ : state) {
        StdMap map;
        map.reserve(128);

        for (int i = 0; i < 100; ++i) {
            map.insert({i, i});
        }

        benchmark::DoNotOptimize(map);
    }
}
BENCHMARK(destroy_populated_std);
