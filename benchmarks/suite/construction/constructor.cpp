// HashMap Constructor Benchmark Suite
// Measures construction, copy, and move performance
// compared against std::unordered_map.
//
// Covers:
// - default construction comparison
// - copy construction of populated maps
// - move construction
// - move assignment

#include <common/framework.h>

#include <unordered_map>
#include <utility>

using namespace HashMapPro;

using Map = HashMap<int, int>;
using StdMap = std::unordered_map<int, int>;

// Measures default construction performance against std::unordered_map.
static void bench_construct() {
    BENCH("hashmap_construct", LARGE, [&] {
        Map map{16};
        doNotOptimize(map);
    });

    BENCH("std_construct", LARGE, [&] {
        StdMap map;
        map.reserve(16);
        doNotOptimize(map);
    });
}

// Measures copy construction performance for populated maps.
static void bench_copy_construct() {
    Map source{64};
    StdMap stdSource;

    for (int i = 0; i < 32; ++i) {
        source.insert(i, i);
        stdSource[i] = i;
    }

    BENCH("hashmap_copy_construct", MEDIUM, [&] {
        Map copy{source};
        doNotOptimize(copy);
    });

    BENCH("std_copy_construct", MEDIUM, [&] {
        StdMap copy{stdSource};
        doNotOptimize(copy);
    });
}

// Measures move construction performance against std::unordered_map.
static void bench_move_construct() {
    BENCH("hashmap_move_construct", LARGE, [&] {
        Map a{16};
        a.insert(1, 1);

        Map b{std::move(a)};
        doNotOptimize(b);
    });

    BENCH("std_move_construct", LARGE, [&] {
        StdMap a;
        a[1] = 1;

        StdMap b{std::move(a)};
        doNotOptimize(b);
    });
}

// Measures move assignment performance against std::unordered_map.
static void bench_move_assign() {
    BENCH("hashmap_move_assign", LARGE, [&] {
        Map a{16};
        a.insert(1, 1);

        Map b{8};
        b = std::move(a);
        doNotOptimize(b);
    });

    BENCH("std_move_assign", LARGE, [&] {
        StdMap a;
        a[1] = 1;

        StdMap b;
        b = std::move(a);
        doNotOptimize(b);
    });
}

// Executes all constructor benchmark cases.
static void run_benchmarks() {
    bench_construct();
    std::cout << "\n";

    bench_copy_construct();
    std::cout << "\n";

    bench_move_construct();
    std::cout << "\n";

    bench_move_assign();
}

REGISTER_BENCH_SUITE();