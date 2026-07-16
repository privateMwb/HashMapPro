// HashMap Destructor Benchmark Suite
// Measures destruction cost against std::unordered_map, isolated by
// comparing an empty map against an equally-sized populated map. Since
// destruction can only be observed by letting a map fall out of scope,
// each case necessarily times construction plus destruction together.
//
// Covers:
// - destruction of an empty map
// - destruction of a populated map

#include <common/framework.h>

#include <unordered_map>

using namespace HashMapPro;

using Map = HashMap<int, int>;
using StdMap = std::unordered_map<int, int>;

// Measures construct-destroy cost for an empty map.
static void bench_destroy_empty() {
    auto hmp = [&] {
        Map map{128};
        doNotOptimize(map);
    };

    auto soum = [&] {
        StdMap map;
        map.reserve(128);
        doNotOptimize(map);
    };

    BENCH("destroy empty", hmp, soum);
}

// Measures construct-populate-destroy cost for a populated map,
// isolating the marginal cost of destroying live nodes.
static void bench_destroy_populated() {
    auto hmp = [&] {
        Map map{128};

        for (int i = 0; i < 100; ++i) {
            map.insert(i, i);
        }

        doNotOptimize(map);
    };

    auto soum = [&] {
        StdMap map;
        map.reserve(128);

        for (int i = 0; i < 100; ++i) {
            map.insert({i, i});
        }

        doNotOptimize(map);
    };

    BENCH("destroy populated", hmp, soum);
}

// Executes all destructor benchmark cases.
static void run_benchmarks() {
    bench_destroy_empty();
    std::cout << "\n";

    bench_destroy_populated();
}

REGISTER_BENCH_SUITE();
