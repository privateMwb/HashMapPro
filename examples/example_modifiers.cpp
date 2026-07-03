// HashMap modifiers example.
//
// Demonstrates:
// - insert
// - duplicate insert behavior
// - update
// - erase
// - clear

#include "example_helper.h"

#include <string>

using namespace HashMapPro;

int main() {
    mainTitle("\nModifiers Examples");
    borderLine();

    HashMap<int, std::string> map{16};

    // Insert several key-value pairs.
    setTitle("Insert");

    map.insert(1, "one");
    map.insert(2, "two");

    std::cout << "map.at(1) : " << map.at(1) << "\n";
    std::cout << "map.at(2) : " << map.at(2) << "\n\n";

    // Attempt to insert an existing key.
    setTitle("Duplicate Insert");

    std::cout << "Before duplicate insert : " << map.at(1) << "\n";

    map.insert(1, "uno");

    std::cout << "After duplicate insert  : " << map.at(1) << "\n\n";

    // Update an existing key and attempt to update a missing one.
    setTitle("Update");

    bool updated = map.update(1, "uno");

    std::cout << "Update succeeded   : " << updated << "\n";
    std::cout << "map.at(1)          : " << map.at(1) << "\n";

    bool missingUpdate = map.update(99, "missing");

    std::cout << "Update missing key : " << missingUpdate << "\n\n";

    // Erase a key from the map.
    setTitle("Erase");

    std::cout << "Size before erase : " << map.size() << "\n";

    bool erased = map.erase(2);

    std::cout << "Erase succeeded   : " << erased << "\n";
    std::cout << "Size after erase  : " << map.size() << "\n\n";

    // Remove every element from the map.
    setTitle("Clear");

    map.insert(3, "three");
    map.insert(4, "four");

    std::cout << "Size before clear : " << map.size() << "\n";

    map.clear();

    std::cout << "Size after clear  : " << map.size() << "\n";
    std::cout << "Empty after clear : " << map.empty() << "\n";

    borderLine();
    std::cout << "\n";
    return 0;
}