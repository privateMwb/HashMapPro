// HashMap self-assignment example.
//
// Demonstrates:
// - assigning a map to itself via copy assignment
// - assigning a map to itself via move assignment
// - that both are safe no-ops rather than data-corrupting mistakes

#include <common/framework.h>

#include <string>

using namespace HashMapPro;

static void run_examples() {
    HashMap<int, std::string> map{16};
    map.insert(1, "one");
    map.insert(2, "two");

    // Self copy-assignment might look like an obvious mistake, but the
    // operator explicitly checks for it and is a safe no-op.
    setTitle("Copy Self-Assignment");

    std::cout << "Size before : " << map.size() << "\n";

    map = map;

    std::cout << "Size after  : " << map.size() << "\n";
    std::cout << "map.at(1)   : " << map.at(1) << "\n\n";

    // Self move-assignment is a subtler trap: without a self-check, an
    // implementation could release its own storage before "moving" from
    // it. This operator also checks for self-assignment and is safe.
    setTitle("Move Self-Assignment");

    std::cout << "Size before : " << map.size() << "\n";

    map = std::move(map);

    std::cout << "Size after  : " << map.size() << "\n";
    std::cout << "map.at(2)   : " << map.at(2) << "\n";
}

REGISTER_EXAMPLE_SUITE();
