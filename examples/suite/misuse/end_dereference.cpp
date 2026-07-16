// HashMap end iterator dereference example.
//
// Demonstrates:
// - why end() must never be dereferenced (undefined behavior)
// - the correct pattern: always compare against end() before use
// - the same rule applied to a failed find()

#include <common/framework.h>

#include <string>

using namespace HashMapPro;

static void run_examples() {
    HashMap<int, std::string> map{16};
    map.insert(1, "one");

    // Misuse (not executed): dereferencing end() is undefined behavior,
    // since end() does not refer to a real element.
    //
    //   auto it = map.end();
    //   std::cout << it->value; // UB — never do this
    setTitle("Misuse: Dereferencing end()");

    std::cout << "The code above is never executed in this example,\n";
    std::cout << "because dereferencing end() is undefined behavior.\n\n";

    // Correct: always guard a dereference with a comparison against end().
    setTitle("Correct: Guard With end() Comparison");

    auto it = map.end();

    if (it != map.end())
        std::cout << "it->value : " << it->value << "\n";
    else
        std::cout << "Iterator is at end(), skipping dereference\n\n";

    // The same rule applies to find(): a failed lookup returns end(),
    // which must be checked before use.
    setTitle("Correct: Guard After find()");

    auto missing = map.find(42);

    if (missing != map.end())
        std::cout << "missing->value : " << missing->value << "\n";
    else
        std::cout << "Key 42 not found, iterator equals end()\n";
}

REGISTER_EXAMPLE_SUITE();
