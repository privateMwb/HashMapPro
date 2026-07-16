// HashMap copy vs move example.
//
// Demonstrates:
// - deep-copying a map with the copy constructor
// - independence of the original and the copy
// - cheap ownership transfer with the move constructor
// - the source map's state immediately after a move

#include <common/framework.h>

#include <string>

using namespace HashMapPro;

static void run_examples() {
    // Deep-copy a map and confirm both are independent.
    setTitle("Copy Construction");

    HashMap<int, std::string> original{16};
    original.insert(1, "one");
    original.insert(2, "two");

    HashMap<int, std::string> copy = original;

    std::cout << "Original size : " << original.size() << "\n";
    std::cout << "Copy size     : " << copy.size() << "\n\n";

    // Mutating the copy must not affect the original.
    setTitle("Independence After Copy");

    copy.insert(3, "three");

    std::cout << "Original size : " << original.size() << "\n";
    std::cout << "Copy size     : " << copy.size() << "\n\n";

    // Move-construct a new map, transferring ownership of storage.
    setTitle("Move Construction");

    HashMap<int, std::string> moved = std::move(original);

    std::cout << "Moved-to size    : " << moved.size() << "\n";

    // NOLINTNEXTLINE(clang-analyzer-cplusplus.Move)
    std::cout << "Moved-from size  : " << original.size() << "\n";
    std::cout << "Moved-from empty : " << original.empty() << "\n\n";

    // Values transferred by move are the same objects, not copies.
    setTitle("Transferred Values");

    std::cout << "moved.at(1) : " << moved.at(1) << "\n";
    std::cout << "moved.at(2) : " << moved.at(2) << "\n";
}

REGISTER_EXAMPLE_SUITE();
