// HashMap Modifiers Benchmark Suite
// Measures insert, update, erase, and clear performance
// compared against std::unordered_map.
//
// Covers:
// - insertion of new keys
// - insertion triggering growth/rehash
// - updating existing keys
// - erasing elements
// - clearing populated maps

#include <common/framework.h>

#include <unordered_map>

using namespace HashMapPro;

using Map = HashMap<int, int>;
using StdMap = std::unordered_map<int, int>;

// Measures insertion of a single new key into a small pre-sized map.
static void bench_insert() {
    BENCH("hashmap_insert", LARGE, [&] {
        Map map{2};
        map.insert(1, 1);
        doNotOptimize(map);
    });

    BENCH("std_insert", LARGE, [&] {
        StdMap map;
        map.reserve(2);
        map.insert({1, 1});
        doNotOptimize(map);
    });
}

// Measures insertion workload that triggers rehash/growth.
static void bench_insert_growth() {
    BENCH("hashmap_insert_growth", MEDIUM, [&] {
        Map map{4};

        for (int i = 0; i < 64; ++i) {
            map.insert(i, i);
        }

        doNotOptimize(map);
    });

    BENCH("std_insert_growth", MEDIUM, [&] {
        StdMap map;
        map.reserve(4);

        for (int i = 0; i < 64; ++i) {
            map.insert({i, i});
        }

        doNotOptimize(map);
    });
}

// Measures updates of existing keys in both implementations.
static void bench_update() {
    Map map{8};
    map.insert(1, 0);

    StdMap stdMap;
    stdMap[1] = 0;

    BENCH("hashmap_update", LARGE, [&] {
        (void)map.update(1, 1);
        doNotOptimize(map);
    });

    BENCH("std_update", LARGE, [&] {
        stdMap[1] = 1;
        doNotOptimize(stdMap);
    });
}

// Measures erase performance for a single existing element.
static void bench_erase() {
    BENCH("hashmap_erase", LARGE, [&] {
        Map map{8};
        map.insert(1, 1);
        doNotOptimize(map.erase(1));
    });

    BENCH("std_erase", LARGE, [&] {
        StdMap map;
        map.reserve(8);
        map.insert({1, 1});
        doNotOptimize(map.erase(1));
    });
}

// Measures clearing a populated map.
static void bench_clear() {
    BENCH("hashmap_clear", MEDIUM, [&] {
        Map map{64};

        for (int i = 0; i < 32; ++i) {
            map.insert(i, i);
        }

        map.clear();
        doNotOptimize(map);
    });

    BENCH("std_clear", MEDIUM, [&] {
        StdMap map;
        map.reserve(64);

        for (int i = 0; i < 32; ++i) {
            map.insert({i, i});
        }

        map.clear();
        doNotOptimize(map);
    });
}

// Executes all modifier benchmark cases.
static void run_benchmarks() {
    bench_insert();
    std::cout << "\n";

    bench_insert_growth();
    std::cout << "\n";

    bench_update();
    std::cout << "\n";

    bench_erase();
    std::cout << "\n";

    bench_clear();
}

REGISTER_BENCH_SUITE();