// HashMap stale iterator example.
//
// Demonstrates:
// - why an iterator to an erased element must not be used afterward
// - the correct pattern: re-lookup or re-iterate after an erase
// - that other, non-erased iterators remain valid

#include <common/framework.h>

#include <string>

using namespace HashMapPro;

static void run_examples() {
    HashMap<int, std::string> map{16};
    map.insert(1, "one");
    map.insert(2, "two");

    [[maybe_unused]] auto staleIt = map.find(1);

    // Misuse (not executed): once the node an iterator refers to is
    // erased, that iterator is dangling. Dereferencing it afterward is
    // undefined behavior.
    //
    //   map.erase(1);
    //   std::cout << staleIt->value; // UB — staleIt is dangling
    setTitle("Misuse: Using an Iterator After Its Element Is Erased");

    (void)map.erase(1);

    std::cout << "The commented dereference above is never executed,\n";
    std::cout << "because staleIt now refers to a destroyed node.\n\n";

    // Correct: obtain a fresh iterator after modifying the map, rather
    // than continuing to use one taken beforehand.
    setTitle("Correct: Re-Lookup After Erase");

    auto freshIt = map.find(1);

    if (freshIt != map.end())
        std::cout << "Unexpectedly still present\n\n";
    else
        std::cout << "Key 1 correctly reported as absent\n\n";

    // Iterators to elements that were not erased remain valid; erase()
    // only invalidates the iterator to the removed element.
    setTitle("Correct: Other Iterators Remain Valid");

    auto stillValid = map.find(2);

    if (stillValid != map.end())
        std::cout << "key 2 -> " << stillValid->value << "\n";
}

REGISTER_EXAMPLE_SUITE();
