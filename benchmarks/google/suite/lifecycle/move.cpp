// HashMap Move Benchmark Suite
// Measures move construction and move assignment performance against std::unordered_map.
//
// Covers:
// - move construction
// - move assignment

#include <HashMapPro/HashMap.h>

#include <benchmark/benchmark.h>
#include <unordered_map>
#include <utility>

using namespace HashMapPro;

using Map = HashMap<int, int>;
using StdMap = std::unordered_map<int, int>;

// Measures move construction (HashMapPro).
static void move_construct_hmp(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        Map src{128};
        for (int i = 0; i < 100; ++i) {
            src.insert(i, 7);
        }
        state.ResumeTiming();

        Map dst(std::move(src));
        benchmark::DoNotOptimize(dst);
    }
}
BENCHMARK(move_construct_hmp);

// Measures move construction (std::unordered_map).
static void move_construct_std(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        StdMap src;
        src.reserve(128);
        for (int i = 0; i < 100; ++i) {
            src.insert({i, 7});
        }
        state.ResumeTiming();

        StdMap dst(std::move(src));
        benchmark::DoNotOptimize(dst);
    }
}
BENCHMARK(move_construct_std);

// Measures move assignment (HashMapPro).
static void move_assignment_hmp(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        Map src{128};
        for (int i = 0; i < 100; ++i) {
            src.insert(i, 7);
        }
        Map dst;
        state.ResumeTiming();

        dst = std::move(src);
        benchmark::DoNotOptimize(dst);
    }
}
BENCHMARK(move_assignment_hmp);

// Measures move assignment (std::unordered_map).
static void move_assignment_std(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        StdMap src;
        src.reserve(128);
        for (int i = 0; i < 100; ++i) {
            src.insert({i, 7});
        }
        StdMap dst;
        state.ResumeTiming();

        dst = std::move(src);
        benchmark::DoNotOptimize(dst);
    }
}
BENCHMARK(move_assignment_std);
