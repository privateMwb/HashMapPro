// HashMap use after move example.
//
// Demonstrates:
// - the mistake of expecting a moved-from map to still hold its data
// - what a moved-from map actually looks like (valid, but empty)
// - the correct pattern: treat a moved-from map as empty, not stale

#include <common/framework.h>

#include <string>

using namespace HashMapPro;

static void run_examples() {
    HashMap<int, std::string> source{16};
    source.insert(1, "one");
    source.insert(2, "two");

    HashMap<int, std::string> destination = std::move(source);

    // Misuse: assuming the moved-from map still contains its old
    // elements. It doesn't — its storage was transferred, not copied.
    setTitle("Misuse: Assuming Stale Data Survives");

    // NOLINTNEXTLINE(clang-analyzer-cplusplus.Move)
    std::cout << "source.size() after move : " << source.size() << "\n";
    std::cout << "source.contains(1)       : " << source.contains(1) << "\n\n";

    if (!source.contains(1)) {
        std::cout << "As expected: the moved-from map no longer has key 1\n\n";
    }

    // Correct: the data now lives in the destination.
    setTitle("Correct: Data Lives in the Destination");

    std::cout << "destination.contains(1) : " << destination.contains(1) << "\n";
    std::cout << "destination.at(1)       : " << destination.at(1) << "\n\n";

    // Correct: a moved-from map is valid and reusable, just empty. It is
    // safe to insert into it again rather than only destroying it.
    setTitle("Correct: Reuse Instead of Assuming Data");

    source.insert(99, "fresh");

    std::cout << "source.size() after reuse : " << source.size() << "\n";
}

REGISTER_EXAMPLE_SUITE();
