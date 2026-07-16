// HashMap vector of pairs example.
//
// Demonstrates:
// - building a HashMap from a std::vector of key-value pairs
// - reserving capacity based on the source vector's size
// - draining a HashMap's contents back into a std::vector

#include <common/framework.h>

#include <string>
#include <utility>
#include <vector>

using namespace HashMapPro;

static void run_examples() {
    // Start with a vector of key-value pairs.
    setTitle("Source Data");

    std::vector<std::pair<int, std::string>> source = {
        {1, "one"}, {2, "two"}, {3, "three"}, {4, "four"}, {5, "five"},
    };

    std::cout << "Source pairs : " << source.size() << "\n\n";

    // Reserve capacity up front, then populate the map from the vector.
    setTitle("Building the Map");

    HashMap<int, std::string> map{4};
    map.reserve(source.size());

    for (const auto& [key, value] : source) {
        map.insert(key, value);
    }

    std::cout << "Map size     : " << map.size() << "\n";
    std::cout << "Map capacity : " << map.capacity() << "\n\n";

    // Drain the map's contents back into a vector.
    setTitle("Extracting Back to a Vector");

    std::vector<std::pair<int, std::string>> extracted;
    extracted.reserve(map.size());

    for (const auto& node : map) {
        extracted.emplace_back(node.key, node.value);
    }

    std::cout << "Extracted pairs : " << extracted.size() << "\n";

    for (const auto& [key, value] : extracted)
        std::cout << key << " -> " << value << "\n";
}

REGISTER_EXAMPLE_SUITE();
