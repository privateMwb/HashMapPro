// HashMap STL algorithms example.
//
// Demonstrates:
// - using <algorithm> functions with HashMap's iterators
// - std::count_if and std::find_if over map elements
// - std::for_each for bulk processing
// - satisfying the bidirectional iterator requirements STL expects

#include <common/framework.h>

#include <algorithm>
#include <string>

using namespace HashMapPro;

static void run_examples() {
    HashMap<int, int> map{16};

    for (int i = 1; i <= 10; ++i) {
        map.insert(i, i * i);
    }

    // Count elements matching a predicate.
    setTitle("std::count_if");

    auto evenCount =
        std::count_if(map.begin(), map.end(), [](const auto& node) { return node.value % 2 == 0; });

    std::cout << "Elements with an even value : " << evenCount << "\n\n";

    // Find the first element matching a predicate.
    setTitle("std::find_if");

    auto it =
        std::find_if(map.begin(), map.end(), [](const auto& node) { return node.value > 50; });

    if (it != map.end())
        std::cout << "First value over 50 : key " << it->key << " -> " << it->value << "\n\n";
    else
        std::cout << "No value over 50 found\n\n";

    // Apply a function to every element.
    setTitle("std::for_each");

    int sum = 0;

    std::for_each(map.begin(), map.end(), [&sum](const auto& node) { sum += node.value; });

    std::cout << "Sum of all values : " << sum << "\n";
}

REGISTER_EXAMPLE_SUITE();
