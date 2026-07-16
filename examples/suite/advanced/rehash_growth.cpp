// HashMap rehash growth example.
//
// Demonstrates:
// - automatic rehashing once the load factor threshold is exceeded
// - capacity doubling as elements are inserted
// - reading load_factor() and max_load_factor()

#include <common/framework.h>

#include <string>

using namespace HashMapPro;

static void run_examples() {
    // Start with a small bucket count to make growth easy to observe.
    setTitle("Initial State");

    HashMap<int, std::string> map{4};

    std::cout << "Capacity        : " << map.capacity() << "\n";
    std::cout << "Max load factor : " << map.max_load_factor() << "\n\n";

    // Insert elements one at a time, watching capacity double whenever
    // the load factor crosses the threshold.
    setTitle("Growth During Insertion");

    for (int i = 0; i < 10; ++i) {
        map.insert(i, "value");

        std::cout << "Size " << map.size() << " / Capacity " << map.capacity() << " / Load factor "
                  << map.load_factor() << "\n";
    }

    std::cout << "\n";

    // Capacity is always a power of two, regardless of the requested
    // initial size.
    setTitle("Power-of-Two Capacity");

    HashMap<int, std::string> odd{10};

    std::cout << "Requested 10, actual capacity : " << odd.capacity() << "\n";
}

REGISTER_EXAMPLE_SUITE();
