// HashMap Move Benchmark Suite
// Measures move construction and move assignment performance against std::unordered_map.
//
// Covers:
// - move construction
// - move assignment

#include <common/framework.h>

#include <unordered_map>
#include <utility>

using namespace HashMapPro;

using Map = HashMap<int, int>;
using StdMap = std::unordered_map<int, int>;

// Measures move construction.
static void bench_move_construct() {
    auto hmp = [&] {
        Map src{128};

        for (int i = 0; i < 100; ++i) {
            src.insert(i, 7);
        }

        Map dst(std::move(src));
        doNotOptimize(dst);
    };

    auto soum = [&] {
        StdMap src;
        src.reserve(128);

        for (int i = 0; i < 100; ++i) {
            src.insert({i, 7});
        }

        StdMap dst(std::move(src));
        doNotOptimize(dst);
    };

    BENCH("move construct", hmp, soum);
}

// Measures move assignment.
static void bench_move_assignment() {
    auto hmp = [&] {
        Map src{128};

        for (int i = 0; i < 100; ++i) {
            src.insert(i, 7);
        }

        Map dst;
        dst = std::move(src);
        doNotOptimize(dst);
    };

    auto soum = [&] {
        StdMap src;
        src.reserve(128);

        for (int i = 0; i < 100; ++i) {
            src.insert({i, 7});
        }

        StdMap dst;
        dst = std::move(src);
        doNotOptimize(dst);
    };

    BENCH("move assignment", hmp, soum);
}

// Executes all move benchmark cases.
static void run_benchmarks() {
    bench_move_construct();
    std::cout << "\n";

    bench_move_assignment();
}

REGISTER_BENCH_SUITE();
