// HashMap const correctness example.
//
// Demonstrates:
// - passing a HashMap by const reference into a function
// - the const-qualified overloads of at(), find(), begin(), and end()
// - const_iterator being used automatically for a const map

#include <common/framework.h>

#include <string>

using namespace HashMapPro;

// Only reads from the map; accepting it by const reference prevents
// accidental mutation and selects the const-qualified member overloads.
static void printSummary(const HashMap<int, std::string>& map) {
    std::cout << "Size  : " << map.size() << "\n";
    std::cout << "Empty : " << map.empty() << "\n";

    for (const auto& node : map)
        std::cout << node.key << " -> " << node.value << "\n";
}

// Reads through at() and find() on a const reference, exercising their
// const-qualified overloads.
static bool tryDescribe(const HashMap<int, std::string>& map, int key) {
    auto it = map.find(key);

    if (it == map.end())
        return false;

    std::cout << "key " << key << " -> " << map.at(key) << "\n";
    return true;
}

static void run_examples() {
    HashMap<int, std::string> map{16};
    map.insert(1, "one");
    map.insert(2, "two");
    map.insert(3, "three");

    // Pass the map into a function that only takes it by const reference.
    setTitle("Read-Only Function");

    printSummary(map);

    std::cout << "\n";

    // const find()/at() work identically to their non-const counterparts.
    setTitle("Const Lookup");

    tryDescribe(map, 2);

    if (!tryDescribe(map, 99))
        std::cout << "key 99 not found\n\n";

    // Binding a const reference to the map yields const_iterator from
    // begin()/end() automatically, without calling cbegin()/cend().
    setTitle("Const Iteration");

    const auto& constView = map;

    for (auto it = constView.begin(); it != constView.end(); ++it)
        std::cout << it->key << " -> " << it->value << "\n";
}

REGISTER_EXAMPLE_SUITE();
