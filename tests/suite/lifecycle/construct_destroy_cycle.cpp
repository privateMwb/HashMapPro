// Verifies repeated construction and destruction leaves no residual
// state, and that nested and container-owned instances are destroyed
// correctly.
//
// Covers:
// - repeated construct/destroy cycles each start empty
// - a map destroyed at the end of a nested scope does not affect an
//   outer map
// - a vector of maps is safely destroyed when cleared
// - a map destroyed mid-scope does not affect a sibling map

#include <common/framework.h>

#include <string>
#include <vector>

// Verifies repeatedly constructing and destroying a map in a loop
// never leaves behind state that affects the next instance.
static void repeated_construct_destroy() {
    for (int i = 0; i < 100; ++i) {
        HashMapPro::HashMap<int, std::string> map{8};

        CHK(map.empty() == true);
        CHK(map.size() == 0);
        CHK(map.capacity() == 8);

        map.insert(i, "value");
        CHK(map.size() == 1);
    }
}

// Verifies a map destroyed at the end of a nested scope does not
// affect an outer map that remains alive.
static void destroy_in_nested_scope() {
    HashMapPro::HashMap<int, std::string> outer{8};
    outer.insert(1, "outer");

    {
        HashMapPro::HashMap<int, std::string> inner{8};
        inner.insert(2, "inner");

        CHK(inner.size() == 1);
    }

    CHK(outer.size() == 1);
    CHK(outer.at(1) == "outer");
    CHK(outer.contains(2) == false);
}

// Verifies a std::vector of maps is destroyed safely when cleared.
static void vector_of_maps_destroyed_on_clear() {
    std::vector<HashMapPro::HashMap<int, std::string>> maps;

    for (int i = 0; i < 5; ++i) {
        HashMapPro::HashMap<int, std::string> map{4};
        map.insert(i, "value");
        maps.push_back(std::move(map));
    }

    CHK(maps.size() == 5);

    for (const auto& map : maps) {
        CHK(map.size() == 1);
    }

    maps.clear();

    CHK(maps.size() == 0);
    CHK(maps.empty() == true);
}

// Verifies one map going out of scope mid-function does not affect a
// sibling map constructed alongside it.
static void sibling_map_unaffected_by_destruction() {
    HashMapPro::HashMap<int, std::string> survivor{8};
    survivor.insert(1, "alive");

    {
        HashMapPro::HashMap<int, std::string> temporary{8};
        temporary.insert(1, "temporary");
        temporary.insert(2, "also temporary");
    }

    CHK(survivor.size() == 1);
    CHK(survivor.at(1) == "alive");
    CHK(survivor.contains(2) == false);
}

// Executes all construct/destroy cycle test cases.
static void run_tests() {
    RUN(repeated_construct_destroy);
    RUN(destroy_in_nested_scope);
    RUN(vector_of_maps_destroyed_on_clear);
    RUN(sibling_map_unaffected_by_destruction);
}

REGISTER_TEST_SUITE();
