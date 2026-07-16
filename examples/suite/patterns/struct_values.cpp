// HashMap struct values example.
//
// Demonstrates:
// - storing an aggregate struct as the mapped value
// - updating individual fields in place through operator[]
// - reading back struct members after mutation

#include <common/framework.h>

#include <string>

using namespace HashMapPro;

// Tracks a player's in-game statistics.
struct PlayerStats {
    int wins = 0;
    int losses = 0;
    double winRate() const {
        int total = wins + losses;
        return total == 0 ? 0.0 : static_cast<double>(wins) / total;
    }
};

static void run_examples() {
    // Construct a map from player name to their stats.
    setTitle("Construction");

    HashMap<std::string, PlayerStats> stats{16};

    std::cout << "Size : " << stats.size() << "\n\n";

    // Insert initial struct values.
    setTitle("Insertion");

    stats.insert("alice", PlayerStats{5, 2});
    stats.insert("bob", PlayerStats{3, 3});

    std::cout << "alice wins : " << stats.at("alice").wins << "\n";
    std::cout << "bob wins   : " << stats.at("bob").wins << "\n\n";

    // Mutate individual struct fields in place via operator[].
    setTitle("Field Mutation");

    stats["alice"].wins += 1;
    stats["bob"].losses += 1;

    std::cout << "alice wins   : " << stats.at("alice").wins << "\n";
    std::cout << "bob losses   : " << stats.at("bob").losses << "\n\n";

    // Derive a value from struct fields.
    setTitle("Derived Value");

    std::cout << "alice win rate : " << stats.at("alice").winRate() << "\n";
    std::cout << "bob win rate   : " << stats.at("bob").winRate() << "\n";
}

REGISTER_EXAMPLE_SUITE();
