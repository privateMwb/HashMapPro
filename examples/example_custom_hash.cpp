// HashMap custom hash example.
//
// Demonstrates:
// - defining a custom key type
// - implementing a custom hash functor
// - constructing a HashMap with a custom hash
// - inserting and looking up custom keys

#include "example_helper.h"

#include <cstddef>
#include <string>

using namespace HashMapPro;

// Represents a 2D point used as a HashMap key.
struct Point {
    int x;
    int y;

    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

// Computes a hash value for Point.
struct PointHash {
    std::size_t operator()(const Point& p) const {
        std::size_t hx = std::hash<int>{}(p.x);
        std::size_t hy = std::hash<int>{}(p.y);

        return hx ^ (hy + 0x9e3779b9 + (hx << 6) + (hx >> 2));
    }
};

int main() {
    mainTitle("\nCustom Hash Examples");
    borderLine();

    // Construct a HashMap using PointHash.
    setTitle("Construction");

    HashMap<Point, std::string, PointHash> map{16};

    std::cout << "Capacity : " << map.capacity() << "\n";
    std::cout << "Size     : " << map.size() << "\n\n";

    // Insert values using Point keys.
    setTitle("Insertion");

    map.insert({0, 0}, "origin");
    map.insert({1, 1}, "diagonal");
    map.insert({-2, 5}, "off-axis");

    std::cout << "Size after insertions : " << map.size() << "\n\n";

    // Look up values using Point keys.
    setTitle("Lookup");

    Point origin{0, 0};
    Point missing{9, 9};

    std::cout << "Contains origin  : " << map.contains(origin) << "\n";
    std::cout << "Contains missing : " << map.contains(missing) << "\n";
    std::cout << "map.at(origin)   : " << map.at(origin) << "\n";

    borderLine();
    std::cout << "\n";
    return 0;
}