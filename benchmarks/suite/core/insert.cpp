// HashMap Insert Benchmark Suite
// Measures insert() performance against std::unordered_map.
//
// Covers:
// - inserting a new key
// - inserting an already-present key (no-op)

#include <common/framework.h>

#include <unordered_map>

using namespace HashMapPro;

using Map = HashMap<int, int>;
using StdMap = std::unordered_map<int, int>;

// Measures insert() performance for a key not yet in the map.
static void bench_insert_new() {
    auto hmp = [&] {
        Map map{128};

        for (int i = 0; i < 64; ++i) {
            map.insert(i, i);
        }

        doNotOptimize(map);
    };

    auto soum = [&] {
        StdMap map;
        map.reserve(128);

        for (int i = 0; i < 64; ++i) {
            map.insert({i, i});
        }

        doNotOptimize(map);
    };

    BENCH("insert new", hmp, soum);
}

// Measures insert() performance for a key already present in the map.
static void bench_insert_existing() {
    Map map{128};
    StdMap stdMap;
    stdMap.reserve(128);

    for (int i = 0; i < 64; ++i) {
        map.insert(i, i);
        stdMap.insert({i, i});
    }

    auto hmp = [&] { map.insert(32, 999); };

    auto soum = [&] { stdMap.insert({32, 999}); };

    BENCH("insert existing", hmp, soum);
}

// Executes all insert benchmark cases.
static void run_benchmarks() {
    bench_insert_new();
    std::cout << "\n";

    bench_insert_existing();
}

REGISTER_BENCH_SUITE();
