// HashMap Reserve Benchmark Suite
// Measures reserve() cost and its effect on bulk-insert throughput
// compared against std::unordered_map.
//
// Covers:
// - cost of reserve() itself on an empty map
// - bulk insertion without pre-reserving (organic growth/rehashing)
// - bulk insertion with pre-reserved capacity (no rehashing)

#include <HashMapPro/HashMap.h>

#include <benchmark/benchmark.h>
#include <unordered_map>

using namespace HashMapPro;

using Map = HashMap<int, int>;
using StdMap = std::unordered_map<int, int>;

// Measures the cost of reserve() alone on an empty map (HashMapPro).
static void reserve_call_hmp(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        Map map{2};
        state.ResumeTiming();

        map.reserve(256);
        benchmark::DoNotOptimize(map);
    }
}
BENCHMARK(reserve_call_hmp);

// Measures the cost of reserve() alone on an empty map (std::unordered_map).
static void reserve_call_std(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        StdMap map;
        state.ResumeTiming();

        map.reserve(256);
        benchmark::DoNotOptimize(map);
    }
}
BENCHMARK(reserve_call_std);

// Measures bulk-insert throughput without pre-reserving, so capacity
// grows organically and rehashes are triggered along the way (HashMapPro).
static void insert_no_reserve_hmp(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        Map map{2};
        state.ResumeTiming();

        for (int i = 0; i < 50; ++i) {
            map.insert(i, i);
        }

        benchmark::DoNotOptimize(map);
    }
}
BENCHMARK(insert_no_reserve_hmp);

// Measures bulk-insert throughput without pre-reserving, so capacity
// grows organically and rehashes are triggered along the way (std::unordered_map).
static void insert_no_reserve_std(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        StdMap map;
        state.ResumeTiming();

        for (int i = 0; i < 50; ++i) {
            map.insert({i, i});
        }

        benchmark::DoNotOptimize(map);
    }
}
BENCHMARK(insert_no_reserve_std);

// Measures bulk-insert throughput into a map pre-sized with reserve(), so
// no rehash occurs during the insertion loop (HashMapPro).
static void insert_with_reserve_hmp(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        Map map{2};
        map.reserve(50);
        state.ResumeTiming();

        for (int i = 0; i < 50; ++i) {
            map.insert(i, i);
        }

        benchmark::DoNotOptimize(map);
    }
}
BENCHMARK(insert_with_reserve_hmp);

// Measures bulk-insert throughput into a map pre-sized with reserve(), so
// no rehash occurs during the insertion loop (std::unordered_map).
static void insert_with_reserve_std(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        StdMap map;
        map.reserve(50);
        state.ResumeTiming();

        for (int i = 0; i < 50; ++i) {
            map.insert({i, i});
        }

        benchmark::DoNotOptimize(map);
    }
}
BENCHMARK(insert_with_reserve_std);
