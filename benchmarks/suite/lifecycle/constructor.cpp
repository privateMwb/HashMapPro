// HashMap Construction Benchmark Suite
// Measures construction performance against std::unordered_map.
//
// Covers:
// - default construction
// - reserved construction
// - populate construction

#include <common/framework.h>

#include <unordered_map>

using namespace HashMapPro;

using Map = HashMap<int, int>;
using StdMap = std::unordered_map<int, int>;

// Measures default construction.
static void bench_default_construct() {
    auto hmp = [&] {
        Map map;
        doNotOptimize(map);
    };

    auto soum = [&] {
        StdMap map;
        doNotOptimize(map);
    };

    BENCH("default construct", hmp, soum);
}

// Measures construction with a reserved bucket count.
static void bench_reserved_construct() {
    auto hmp = [&] {
        Map map{100};
        doNotOptimize(map);
    };

    auto soum = [&] {
        StdMap map;
        map.reserve(100);
        doNotOptimize(map);
    };

    BENCH("reserved construct", hmp, soum);
}

// Measures construction followed by populating a handful of elements.
static void bench_populate_construct() {
    auto hmp = [&] {
        Map map;

        for (int i = 0; i < 5; ++i) {
            map.insert(i, i);
        }

        doNotOptimize(map);
    };

    auto soum = [&] {
        StdMap map;

        for (int i = 0; i < 5; ++i) {
            map.insert({i, i});
        }

        doNotOptimize(map);
    };

    BENCH("populate construct", hmp, soum);
}

// Executes all construction benchmark cases.
static void run_benchmarks() {
    bench_default_construct();
    std::cout << "\n";

    bench_reserved_construct();
    std::cout << "\n";

    bench_populate_construct();
}

REGISTER_BENCH_SUITE();
