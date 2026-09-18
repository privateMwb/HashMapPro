// HashMap Copy Benchmark Suite
// Measures copy construction and copy assignment performance against std::unordered_map.
//
// Covers:
// - copy construction
// - copy assignment

#include <HashMapPro/HashMap.h>

#include <benchmark/benchmark.h>
#include <unordered_map>

using namespace HashMapPro;

using Map = HashMap<int, int>;
using StdMap = std::unordered_map<int, int>;

// Measures copy construction from a populated map (HashMapPro).
static void copy_construct_hmp(benchmark::State& state) {
    Map src{128};
    for (int i = 0; i < 100; ++i) {
        src.insert(i, i);
    }

    for (auto _ : state) {
        Map c(src);
        benchmark::DoNotOptimize(c);
    }
}
BENCHMARK(copy_construct_hmp);

// Measures copy construction from a populated map (std::unordered_map).
static void copy_construct_std(benchmark::State& state) {
    StdMap stdSrc;
    stdSrc.reserve(128);
    for (int i = 0; i < 100; ++i) {
        stdSrc.insert({i, i});
    }

    for (auto _ : state) {
        StdMap c(stdSrc);
        benchmark::DoNotOptimize(c);
    }
}
BENCHMARK(copy_construct_std);

// Measures copy assignment with an already-populated destination (HashMapPro).
static void copy_assignment_hmp(benchmark::State& state) {
    Map src{128};
    for (int i = 0; i < 100; ++i) {
        src.insert(i, i);
    }

    Map dst{128};
    for (int i = 0; i < 100; ++i) {
        dst.insert(i, 0);
    }

    for (auto _ : state) {
        dst = src;
    }
}
BENCHMARK(copy_assignment_hmp);

// Measures copy assignment with an already-populated destination (std::unordered_map).
static void copy_assignment_std(benchmark::State& state) {
    StdMap stdSrc;
    stdSrc.reserve(128);
    for (int i = 0; i < 100; ++i) {
        stdSrc.insert({i, i});
    }

    StdMap stdDst;
    stdDst.reserve(128);
    for (int i = 0; i < 100; ++i) {
        stdDst.insert({i, 0});
    }

    for (auto _ : state) {
        stdDst = stdSrc;
    }
}
BENCHMARK(copy_assignment_std);
