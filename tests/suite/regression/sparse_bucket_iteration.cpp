// HashMap Sparse Bucket Iteration Regression Test Suite
// Verifies iteration correctly skips over empty buckets when
// elements are sparsely distributed across a large bucket array,
// using an identity hash to place keys at known, deliberately
// spaced-out bucket indices.
//
// Covers:
// - begin() finds the first non-empty bucket, skipping leading gaps
// - forward iteration skips interior gaps between occupied buckets
// - iteration correctly reaches end() after the last occupied bucket
// - reverse iteration finds the last occupied bucket first
// - reverse iteration skips interior gaps symmetric to forward iteration

#include <common/framework.h>

#include <cstddef>
#include <string>
#include <vector>

// Maps each int key directly to its own bucket index, so placing key
// K in a map of capacity C puts it at bucket (K & (C - 1)).
struct IdentityHash {
    std::size_t operator()(int key) const {
        return static_cast<std::size_t>(key);
    }
};

using SparseMap = HashMapPro::HashMap<int, std::string, IdentityHash>;

// Verifies begin() lands on the first occupied bucket, correctly
// skipping past empty leading buckets.
static void begin_skips_leading_empty_buckets() {
    SparseMap map{64};

    // Bucket 0 is left empty; the first occupied bucket is 20.
    map.insert(20, "at-20");
    map.insert(45, "at-45");

    auto it = map.begin();

    CHK(it != map.end());
    CHK(it->key == 20);
}

// Verifies forward iteration correctly skips over interior gaps of
// empty buckets between occupied ones.
static void forward_iteration_skips_interior_gaps() {
    SparseMap map{64};

    map.insert(3, "a");
    map.insert(20, "b");
    map.insert(45, "c");
    map.insert(63, "d");

    std::vector<int> visited;

    for (auto it = map.begin(); it != map.end(); ++it) {
        visited.push_back(it->key);
    }

    CHK(visited.size() == 4);
    CHK(visited[0] == 3);
    CHK(visited[1] == 20);
    CHK(visited[2] == 45);
    CHK(visited[3] == 63);
}

// Verifies iteration correctly reaches end() once past the last
// occupied bucket, without skipping past it or looping.
static void iteration_reaches_end_after_last_bucket() {
    SparseMap map{64};

    map.insert(5, "a");
    map.insert(63, "last");

    auto it = map.begin();
    ++it;

    CHK(it->key == 63);

    ++it;

    CHK(it == map.end());
}

// Verifies reverse iteration starts at the last occupied bucket,
// correctly skipping trailing empty buckets.
static void reverse_iteration_finds_last_occupied_bucket() {
    SparseMap map{64};

    map.insert(10, "a");
    map.insert(50, "b");

    auto it = map.rbegin();

    CHK(it != map.rend());
    CHK(it->key == 50);
}

// Verifies reverse iteration skips interior gaps symmetrically to
// forward iteration, visiting the same elements in reverse order.
static void reverse_iteration_skips_interior_gaps() {
    SparseMap map{64};

    map.insert(3, "a");
    map.insert(20, "b");
    map.insert(45, "c");
    map.insert(63, "d");

    std::vector<int> visited;

    for (auto it = map.rbegin(); it != map.rend(); ++it) {
        visited.push_back(it->key);
    }

    CHK(visited.size() == 4);
    CHK(visited[0] == 63);
    CHK(visited[1] == 45);
    CHK(visited[2] == 20);
    CHK(visited[3] == 3);
}

// Executes all sparse bucket iteration test cases.
static void run_tests() {
    RUN(begin_skips_leading_empty_buckets);
    RUN(forward_iteration_skips_interior_gaps);
    RUN(iteration_reaches_end_after_last_bucket);
    RUN(reverse_iteration_finds_last_occupied_bucket);
    RUN(reverse_iteration_skips_interior_gaps);
}

REGISTER_TEST_SUITE();
