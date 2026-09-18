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

#include <HashMapPro/HashMap.h>

#include <gtest/gtest.h>

#include <string>
#include <vector>

// Verifies repeatedly constructing and destroying a map in a loop
// never leaves behind state that affects the next instance.
TEST(ConstructDestroyCycle, RepeatedConstructDestroy) {
    for (int i = 0; i < 100; ++i) {
        HashMapPro::HashMap<int, std::string> map{8};

        EXPECT_TRUE(map.empty());
        EXPECT_EQ(map.size(), 0u);
        EXPECT_EQ(map.capacity(), 8u);

        map.insert(i, "value");
        EXPECT_EQ(map.size(), 1u);
    }
}

// Verifies a map destroyed at the end of a nested scope does not
// affect an outer map that remains alive.
TEST(ConstructDestroyCycle, DestroyInNestedScope) {
    HashMapPro::HashMap<int, std::string> outer{8};
    outer.insert(1, "outer");

    {
        HashMapPro::HashMap<int, std::string> inner{8};
        inner.insert(2, "inner");

        EXPECT_EQ(inner.size(), 1u);
    }

    EXPECT_EQ(outer.size(), 1u);
    EXPECT_EQ(outer.at(1), "outer");
    EXPECT_FALSE(outer.contains(2));
}

// Verifies a std::vector of maps is destroyed safely when cleared.
TEST(ConstructDestroyCycle, VectorOfMapsDestroyedOnClear) {
    std::vector<HashMapPro::HashMap<int, std::string>> maps;

    for (int i = 0; i < 5; ++i) {
        HashMapPro::HashMap<int, std::string> map{4};
        map.insert(i, "value");
        maps.push_back(std::move(map));
    }

    EXPECT_EQ(maps.size(), 5u);

    for (const auto& map : maps) {
        EXPECT_EQ(map.size(), 1u);
    }

    maps.clear();

    EXPECT_EQ(maps.size(), 0u);
    EXPECT_TRUE(maps.empty());
}

// Verifies one map going out of scope mid-function does not affect a
// sibling map constructed alongside it.
TEST(ConstructDestroyCycle, SiblingMapUnaffectedByDestruction) {
    HashMapPro::HashMap<int, std::string> survivor{8};
    survivor.insert(1, "alive");

    {
        HashMapPro::HashMap<int, std::string> temporary{8};
        temporary.insert(1, "temporary");
        temporary.insert(2, "also temporary");
    }

    EXPECT_EQ(survivor.size(), 1u);
    EXPECT_EQ(survivor.at(1), "alive");
    EXPECT_FALSE(survivor.contains(2));
}
