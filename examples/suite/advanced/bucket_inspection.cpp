// HashMap bucket inspection example.
//
// Demonstrates:
// - reasoning about bucket placement using the same hash-and-mask
//   formula the map uses internally, without touching private storage
// - observing how capacity growth changes bucket assignment
// - counting how many keys logically land in the same bucket

#include <common/framework.h>

#include <array>
#include <string>

using namespace HashMapPro;

// Mirrors HashMap's internal bucket formula: hasher(key) & (capacity - 1).
// Only works here because the map uses the default std::hash<K>.
static std::size_t expectedBucket(int key, std::size_t capacity) {
    return std::hash<int>{}(key) & (capacity - 1);
}

static void run_examples() {
    // Insert a small set of keys into a small map.
    setTitle("Setup");

    HashMap<int, std::string> map{8};

    for (int i = 0; i < 8; ++i) {
        map.insert(i, "value");
    }

    std::cout << "Capacity : " << map.capacity() << "\n\n";

    // Compute the bucket each key would land in, using the map's own
    // hash-and-mask formula.
    setTitle("Bucket Assignment");

    for (int i = 0; i < 8; ++i) {
        std::cout << "key " << i << " -> bucket " << expectedBucket(i, map.capacity()) << "\n";
    }

    std::cout << "\n";

    // Tally how many keys share each bucket at this capacity.
    setTitle("Bucket Occupancy");

    std::array<int, 8> occupancy{};

    for (int i = 0; i < 8; ++i) {
        occupancy[expectedBucket(i, map.capacity())]++;
    }

    for (std::size_t b = 0; b < occupancy.size(); ++b)
        std::cout << "bucket " << b << " : " << occupancy[b] << " key(s)\n";

    std::cout << "\n";

    // Growing the map changes the mask, which redistributes keys across
    // a larger set of buckets.
    setTitle("Occupancy After Growth");

    map.reserve(64);

    std::cout << "New capacity : " << map.capacity() << "\n";

    for (int i = 0; i < 8; ++i) {
        std::cout << "key " << i << " -> bucket " << expectedBucket(i, map.capacity()) << "\n";
    }
}

REGISTER_EXAMPLE_SUITE();
