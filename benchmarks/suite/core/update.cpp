// HashMap Update Benchmark Suite
// Measures update() performance against std::unordered_map.
//
// Covers:
// - updating an existing key
// - updating a missing key (no-op)

#include <common/framework.h>

#include <unordered_map>

using namespace HashMapPro;

using Map = HashMap<int, int>;
using StdMap = std::unordered_map<int, int>;

// Measures update() performance for a key that exists in the map.
static void bench_update_existing() {
    Map map{128};
    StdMap stdMap;
    stdMap.reserve(128);

    for (int i = 0; i < 64; ++i) {
        map.insert(i, i);
        stdMap.insert({i, i});
    }

    auto hmp = [&] { doNotOptimize(map.update(32, 999)); };

    auto soum = [&] {
        auto it = stdMap.find(32);
        bool updated = it != stdMap.end();

        if (updated) {
            it->second = 999;
        }

        doNotOptimize(updated);
    };

    BENCH("update existing", hmp, soum);
}

// Measures update() performance for a key that does not exist in the map.
static void bench_update_missing() {
    Map map{128};
    StdMap stdMap;
    stdMap.reserve(128);

    for (int i = 0; i < 64; ++i) {
        map.insert(i, i);
        stdMap.insert({i, i});
    }

    auto hmp = [&] { doNotOptimize(map.update(9999, 999)); };

    auto soum = [&] {
        auto it = stdMap.find(9999);
        bool updated = it != stdMap.end();

        if (updated) {
            it->second = 999;
        }

        doNotOptimize(updated);
    };

    BENCH("update missing", hmp, soum);
}

// Executes all update benchmark cases.
static void run_benchmarks() {
    bench_update_existing();
    std::cout << "\n";

    bench_update_missing();
}

REGISTER_BENCH_SUITE();
