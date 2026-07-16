// HashMap missing key example.
//
// Demonstrates:
// - at() throwing std::out_of_range on a missing key
// - the risk of calling at() without checking contains() first
// - the correct pattern: check before accessing, or catch the exception

#include <common/framework.h>

#include <stdexcept>
#include <string>

using namespace HashMapPro;

static void run_examples() {
    HashMap<int, std::string> map{16};
    map.insert(1, "one");

    // Misuse: calling at() on a key that was never inserted throws.
    // Left uncaught, this exception would propagate out and terminate
    // the program, so it is caught here purely to show what happens.
    setTitle("Misuse: Unchecked at()");

    try {
        std::cout << "map.at(99) : " << map.at(99) << "\n";
    } catch (const std::out_of_range& e) {
        std::cout << "Caught exception : " << e.what() << "\n\n";
    }

    // Correct: check contains() before accessing when the key's
    // presence isn't already guaranteed.
    setTitle("Correct: Check Before Access");

    if (map.contains(99)) {
        std::cout << "map.at(99) : " << map.at(99) << "\n";
    } else {
        std::cout << "Key 99 is not present, skipping access\n\n";
    }

    // Correct: prefer find() when a missing key is an expected outcome
    // rather than an error.
    setTitle("Correct: Use find() Instead");

    auto it = map.find(99);

    if (it != map.end())
        std::cout << "Found : " << it->value << "\n";
    else
        std::cout << "Key 99 not found, no exception involved\n";
}

REGISTER_EXAMPLE_SUITE();
