// HashMap iteration example.
//
// Demonstrates:
// - forward iteration
// - range-based iteration
// - mutating values through iteration
// - reverse iteration
// - const iteration

#include <common/framework.h>

#include <string>

using namespace HashMapPro;

static void run_examples() {
    HashMap<int, std::string> map{16};
    map.insert(1, "one");
    map.insert(2, "two");
    map.insert(3, "three");

    // Iterate from begin() to end().
    setTitle("Forward Iteration");

    for (auto it = map.begin(); it != map.end(); ++it)
        std::cout << it->key << " -> " << it->value << "\n";

    std::cout << "\n";

    // Iterate using a range-based for loop.
    setTitle("Range-Based For");

    for (const auto& node : map)
        std::cout << node.key << " -> " << node.value << "\n";

    std::cout << "\n";

    // Modify values through mutable iteration.
    setTitle("Mutating Values");

    for (auto& node : map)
        node.value += "!";

    std::cout << "map.at(1) : " << map.at(1) << "\n";
    std::cout << "map.at(2) : " << map.at(2) << "\n\n";

    // Iterate in reverse order.
    setTitle("Reverse Iteration");

    for (auto it = map.rbegin(); it != map.rend(); ++it)
        std::cout << it->key << " -> " << it->value << "\n";

    std::cout << "\n";

    // Iterate over a const HashMap.
    setTitle("Const Iteration");

    const auto& constMap = map;

    for (auto it = constMap.cbegin(); it != constMap.cend(); ++it)
        std::cout << it->key << " -> " << it->value << "\n";
}

REGISTER_EXAMPLE_SUITE();