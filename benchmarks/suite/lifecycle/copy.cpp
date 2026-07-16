// HashMap Copy Benchmark Suite
// Measures copy construction and copy assignment performance against std::unordered_map.
//
// Covers:
// - copy construction
// - copy assignment

#include <common/framework.h>

#include <unordered_map>

using namespace HashMapPro;

using Map = HashMap<int, int>;
using StdMap = std::unordered_map<int, int>;

// Measures copy construction from a populated map.
static void bench_copy_construct() {
    Map src{128};
    StdMap stdSrc;
    stdSrc.reserve(128);

    for (int i = 0; i < 100; ++i) {
        src.insert(i, i);
        stdSrc.insert({i, i});
    }

    auto hmp = [&] {
        Map c(src);
        doNotOptimize(c);
    };

    auto soum = [&] {
        StdMap c(stdSrc);
        doNotOptimize(c);
    };

    BENCH("copy construct", hmp, soum);
}

// Measures copy assignment with an already-populated destination.
static void bench_copy_assignment() {
    Map src{128};
    StdMap stdSrc;
    stdSrc.reserve(128);

    for (int i = 0; i < 100; ++i) {
        src.insert(i, i);
        stdSrc.insert({i, i});
    }

    Map dst{128};
    StdMap stdDst;
    stdDst.reserve(128);

    for (int i = 0; i < 100; ++i) {
        dst.insert(i, 0);
        stdDst.insert({i, 0});
    }

    auto hmp = [&] { dst = src; };

    auto soum = [&] { stdDst = stdSrc; };

    BENCH("copy assignment", hmp, soum);
}

// Executes all copy benchmark cases.
static void run_benchmarks() {
    bench_copy_construct();
    std::cout << "\n";

    bench_copy_assignment();
}

REGISTER_BENCH_SUITE();
