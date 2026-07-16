// HashMap word counter example.
//
// Demonstrates:
// - using std::string keys
// - a common word-frequency counting pattern
// - combining operator[] with default-constructed values
// - iterating results after processing

#include <common/framework.h>

#include <sstream>
#include <string>

using namespace HashMapPro;

static void run_examples() {
    // Split a sentence into individual words.
    setTitle("Input Text");

    std::string text = "the quick brown fox jumps over the lazy dog the fox runs";

    std::cout << "Text : " << text << "\n\n";

    // Count occurrences of each word using operator[].
    setTitle("Counting Words");

    HashMap<std::string, int> counts{16};

    std::istringstream stream{text};
    std::string word;

    while (stream >> word) {
        // operator[] default-constructs a 0 for a new key, then we
        // increment it, giving a compact one-line counting idiom.
        counts[word]++;
    }

    std::cout << "Unique words : " << counts.size() << "\n\n";

    // Report the frequency of a few specific words.
    setTitle("Lookup Frequencies");

    std::cout << "\"the\"   : " << counts.at("the") << "\n";
    std::cout << "\"fox\"   : " << counts.at("fox") << "\n";
    std::cout << "\"lazy\"  : " << counts.at("lazy") << "\n";

    std::cout << "\n";

    // Iterate all collected word counts.
    setTitle("All Counts");

    for (const auto& node : counts)
        std::cout << node.key << " : " << node.value << "\n";
}

REGISTER_EXAMPLE_SUITE();
