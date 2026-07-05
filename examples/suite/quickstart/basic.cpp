// Basic HashMap example.
//
// Demonstrates:
// - construction with an initial capacity
// - insert
// - operator[] and at()
// - contains
// - size, capacity, and empty

#include <common/framework.h>

#include <string>

using namespace HashMapPro;

static void run_examples() {
    // Construct a HashMap with an initial capacity.
    setTitle("Construction");

    HashMap<int, std::string> map{16};

    std::cout << "Capacity : " << map.capacity() << "\n";
    std::cout << "Size     : " << map.size() << "\n";
    std::cout << "Empty    : " << map.empty() << "\n\n";

    // Insert several key-value pairs.
    setTitle("Insertion");

    map.insert(1, "one");
    map.insert(2, "two");
    map.insert(3, "three");

    std::cout << "Size after insertions : " << map.size() << "\n\n";

    // Access and modify values.
    setTitle("Access");

    std::cout << "map[1]    : " << map[1] << "\n";
    std::cout << "map.at(2) : " << map.at(2) << "\n";

    map[4] = "four";

    std::cout << "map[4]    : " << map[4] << "\n\n";

    // Check whether specific keys exist.
    setTitle("Contains");

    std::cout << "Contains 3  : " << map.contains(3) << "\n";
    std::cout << "Contains 99 : " << map.contains(99) << "\n\n";

    // Inspect the container state.
    setTitle("Capacity");

    std::cout << "Size     : " << map.size() << "\n";
    std::cout << "Capacity : " << map.capacity() << "\n";
    std::cout << "Empty    : " << map.empty() << "\n";
}

REGISTER_EXAMPLE_SUITE();