// HashMap Reserve Benchmark Suite
// Measures reserve() cost and its effect on bulk-insert throughput
// compared against std::unordered_map.
//
// Covers:
// - cost of reserve() itself on an empty map
// - bulk insertion without pre-reserving (organic growth/rehashing)
// - bulk insertion with pre-reserved capacity (no rehashing)

#include <common/framework.h>

#include <unordered_map>

using namespace HashMapPro;

using Map = HashMap<int, int>;
using StdMap = std::unordered_map<int, int>;

// Measures the cost of reserve() alone on an empty map.
static void bench_reserve_call() {
    auto hmp = [&] {
        Map map{2};
        map.reserve(256);
        doNotOptimize(map);
    };

    auto soum = [&] {
        StdMap map;
        map.reserve(256);
        doNotOptimize(map);
    };

    BENCH("reserve_call", hmp, soum);
}

// Measures bulk-insert throughput without pre-reserving, so capacity
// grows organically and rehashes are triggered along the way.
static void bench_insert_no_reserve() {
    auto hmp = [&] {
        Map map{2};

        for (int i = 0; i < 50; ++i) {
            map.insert(i, i);
        }

        doNotOptimize(map);
    };

    auto soum = [&] {
        StdMap map;

        for (int i = 0; i < 50; ++i) {
            map.insert({i, i});
        }

        doNotOptimize(map);
    };

    BENCH("insert_no_reserve", hmp, soum);
}

// Measures bulk-insert throughput into a map pre-sized with reserve(),
// so no rehash occurs during the insertion loop.
static void bench_insert_with_reserve() {
    auto hmp = [&] {
        Map map{2};
        map.reserve(50);

        for (int i = 0; i < 50; ++i) {
            map.insert(i, i);
        }

        doNotOptimize(map);
    };

    auto soum = [&] {
        StdMap map;
        map.reserve(50);

        for (int i = 0; i < 50; ++i) {
            map.insert({i, i});
        }

        doNotOptimize(map);
    };

    BENCH("insert_with_reserve", hmp, soum);
}

// Executes all reserve benchmark cases.
static void run_benchmarks() {
    bench_reserve_call();
    std::cout << "\n";

    bench_insert_no_reserve();
    std::cout << "\n";

    bench_insert_with_reserve();
}

REGISTER_BENCH_SUITE();
