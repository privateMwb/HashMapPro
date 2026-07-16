// HashMap reuse after move example.
//
// Demonstrates:
// - a moved-from map left in a valid, empty state
// - safely reusing a moved-from map instead of only destroying it
// - lazily-allocated storage recovering on the next insertion
// - move assignment into an already-populated map

#include <common/framework.h>

#include <string>

using namespace HashMapPro;

static void run_examples() {
    // Move a map's contents out, leaving it valid but empty.
    setTitle("After Move");

    HashMap<int, std::string> source{16};
    source.insert(1, "one");

    HashMap<int, std::string> destination = std::move(source);

    // NOLINTNEXTLINE(clang-analyzer-cplusplus.Move)
    std::cout << "Source size  : " << source.size() << "\n";
    std::cout << "Source empty : " << source.empty() << "\n\n";

    // The moved-from map has no bucket storage, but reuse is still safe;
    // storage is lazily allocated again on the next insertion.
    setTitle("Reusing the Moved-From Map");

    source.insert(42, "reused");

    std::cout << "Source size after reuse : " << source.size() << "\n";
    std::cout << "source.at(42)           : " << source.at(42) << "\n\n";

    // Move-assigning into a map that already holds elements replaces its
    // contents entirely rather than merging.
    setTitle("Move Assignment Into Populated Map");

    HashMap<int, std::string> target{16};
    target.insert(100, "old");

    HashMap<int, std::string> replacement{16};
    replacement.insert(200, "new");

    target = std::move(replacement);

    std::cout << "target.contains(100) : " << target.contains(100) << "\n";
    std::cout << "target.contains(200) : " << target.contains(200) << "\n";
}

REGISTER_EXAMPLE_SUITE();
