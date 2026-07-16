// HashMap reserve capacity example.
//
// Demonstrates:
// - reserving capacity before a bulk insertion loop
// - avoiding repeated rehashing during large inserts
// - capacity growth with and without reserve

#include <common/framework.h>

#include <string>

using namespace HashMapPro;

static void run_examples() {
    // Insert many elements without reserving ahead of time.
    setTitle("Without Reserve");

    HashMap<int, std::string> unreserved{4};

    std::cout << "Initial capacity : " << unreserved.capacity() << "\n";

    for (int i = 0; i < 100; ++i) {
        unreserved.insert(i, "value");
    }

    std::cout << "Size after inserts     : " << unreserved.size() << "\n";
    std::cout << "Capacity after inserts : " << unreserved.capacity() << "\n\n";

    // Reserve enough capacity up front to avoid rehashing mid-loop.
    setTitle("With Reserve");

    HashMap<int, std::string> reserved{4};

    reserved.reserve(100);

    std::cout << "Capacity after reserve : " << reserved.capacity() << "\n";

    for (int i = 0; i < 100; ++i) {
        reserved.insert(i, "value");
    }

    std::cout << "Size after inserts     : " << reserved.size() << "\n";
    std::cout << "Capacity after inserts : " << reserved.capacity() << "\n\n";

    // Reserve is a no-op if the map already has enough capacity.
    setTitle("Redundant Reserve");

    std::size_t before = reserved.capacity();

    reserved.reserve(50);

    std::cout << "Capacity before : " << before << "\n";
    std::cout << "Capacity after  : " << reserved.capacity() << "\n";
}

REGISTER_EXAMPLE_SUITE();
