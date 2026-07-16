// HashMap Erase Benchmark Suite
// Measures erase() performance against std::unordered_map. Each case
// rebuilds the map fresh per iteration, since erasing mutates state
// that later iterations depend on.
//
// Covers:
// - erasing an existing key
// - erasing a missing key (no-op)

#include <common/framework.h>

#include <unordered_map>

using namespace HashMapPro;

using Map = HashMap<int, int>;
using StdMap = std::unordered_map<int, int>;

// Measures erase() performance for a key that exists in the map.
static void bench_erase_existing() {
    auto hmp = [&] {
        Map map{128};

        for (int i = 0; i < 64; ++i) {
            map.insert(i, i);
        }

        doNotOptimize(map.erase(32));
    };

    auto soum = [&] {
        StdMap map;
        map.reserve(128);

        for (int i = 0; i < 64; ++i) {
            map.insert({i, i});
        }

        doNotOptimize(map.erase(32));
    };

    BENCH("erase existing", hmp, soum);
}

// Measures erase() performance for a key that does not exist in the map.
static void bench_erase_missing() {
    auto hmp = [&] {
        Map map{128};

        for (int i = 0; i < 64; ++i) {
            map.insert(i, i);
        }

        doNotOptimize(map.erase(9999));
    };

    auto soum = [&] {
        StdMap map;
        map.reserve(128);

        for (int i = 0; i < 64; ++i) {
            map.insert({i, i});
        }

        doNotOptimize(map.erase(9999));
    };

    BENCH("erase missing", hmp, soum);
}

// Executes all erase benchmark cases.
static void run_benchmarks() {
    bench_erase_existing();
    std::cout << "\n";

    bench_erase_missing();
}

REGISTER_BENCH_SUITE();
