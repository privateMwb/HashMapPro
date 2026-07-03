// HashMap lookup example.
//
// Demonstrates:
// - contains
// - find
// - handling a missing key
// - at() with exception handling

#include "example_helper.h"

#include <stdexcept>
#include <string>

using namespace HashMapPro;

int main() {
    mainTitle("\nLookup Examples");
    borderLine();

    HashMap<int, std::string> map{16};
    map.insert(1, "one");
    map.insert(2, "two");

    // Check whether keys exist.
    setTitle("Contains");

    std::cout << "Contains 1  : " << map.contains(1) << "\n";
    std::cout << "Contains 99 : " << map.contains(99) << "\n\n";

    // Find an existing key.
    setTitle("Find");

    auto it = map.find(1);

    if (it != map.end())
        std::cout << "Found key 1 : " << it->value << "\n";
    else
        std::cout << "Key 1 not found\n";

    std::cout << "\n";

    // Search for a missing key without throwing.
    setTitle("Missing Key");

    auto missing = map.find(99);

    if (missing != map.end())
        std::cout << "Found key 99 : " << missing->value << "\n";
    else
        std::cout << "Key 99 not found\n";

    std::cout << "\n";

    // Access values with at(), handling missing keys.
    setTitle("at()");

    try {
        std::cout << "map.at(2)  : " << map.at(2) << "\n";
        std::cout << "map.at(50) : " << map.at(50) << "\n";
    }
    catch (const std::out_of_range& e) {
        std::cout << "Caught exception : " << e.what() << "\n";
    }

    borderLine();
    std::cout << "\n";
    return 0;
}