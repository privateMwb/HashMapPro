# HashMapPro

[![C++23](https://img.shields.io/badge/C%2B%2B-23-blue)](https://en.cppreference.com/w/cpp/23)
[![Status](https://img.shields.io/badge/status-learning%20project-green)](https://github.com/privateMwb/HashMapPro)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

A custom C++ hash map implementation built for learning associative container design, separate chaining, hashing strategy, and performance benchmarking against `std::unordered_map`.

---

## Table of Contents

- [Overview](#overview)
- [Motivation / Goals](#motivation--goals)
- [Features](#features)
- [Design Overview](#design-overview)
- [Complexity](#complexity)
- [Quick Example](#quick-example)
- [Core API](#core-api)
- [Benchmark Results](#benchmark-results)
- [Project Structure](#project-structure)
- [Build Instructions](#build-instructions)
- [Notes](#notes)
- [License](#license)

---

## Overview

HashMapPro (`HashMap`) is an associative container implemented from scratch in modern C++ (C++23), built as a `std::unordered_map`-compatible key/value store.
It focuses on understanding how hash tables work internally, including separate chaining, bucket indexing, load-factor-driven rehashing, and iterator design over a bucket-and-chain layout.

It also includes:

- Bidirectional iterators over buckets and chains, with const and reverse variants
- Pluggable hashing via a `Hash` template parameter, defaulting to `std::hash<K>`
- Automatic growth via load-factor-triggered rehashing
- Exception-safe copy construction and copy assignment
- Benchmark suite comparing against `std::unordered_map`
- Unit tests for correctness validation

---

## Motivation / Goals

This project was built to understand:

- Separate chaining as a collision resolution strategy
- Bucket indexing and load-factor-driven growth
- Iterator design over a two-level structure (buckets + linked chains)
- Exception safety during copy construction and assignment (commit/rollback)
- Move semantics for a heap-owning container
- Performance benchmarking vs `std::unordered_map`

---

## Features

- Separate chaining with singly-linked nodes per bucket
- O(1) average-case `find`, `insert`, `erase`, `contains`
- Automatic rehashing when the load factor exceeds `0.75`
- Bidirectional `iterator` / `const_iterator` over the full table
- `reverse_iterator` / `const_reverse_iterator` via `std::reverse_iterator`
- `operator[]` with default-value insertion, matching `std::unordered_map` semantics
- `at()` with `std::out_of_range` on a missing key, const and non-const overloads
- `update()` for explicit existing-key assignment, distinct from `insert()`
- Pluggable `Hash` template parameter for custom key types
- Deep-copying copy constructor/assignment with strong rollback on allocation failure
- Move constructor/assignment that leave the source in a valid empty state

---

## Design Overview

HashMap uses an array of bucket head pointers (`Node<K, V>**`), where each bucket is the head of a singly-linked chain of nodes sharing the same bucket index.

### Internal Structure

```
buckets (Node<K,V>**)
  ↓
[0] -> nullptr
[1] -> Node(k,v) -> Node(k,v) -> nullptr
[2] -> nullptr
[3] -> Node(k,v) -> nullptr
...
```

- `buckets`      → array of bucket head pointers
- `bucketCount`  → number of buckets in the array
- `elementCount` → number of stored key/value pairs
- `hasher`       → the `Hash` instance used to compute bucket indices

### Bucket Indexing

A key's bucket is computed by hashing and reducing modulo the bucket count:

```cpp
std::size_t index = hasher(key) % bucketCount;
```

### Collision Resolution

Collisions are resolved by chaining — colliding keys are linked within the same bucket via `Node::next`, and lookup walks the chain comparing keys with `operator==`.

### Growth Strategy

After each insertion, the load factor is checked against a fixed threshold:

```cpp
if (static_cast<double>(elementCount) / bucketCount > MAX_LOAD_FACTOR) {
    rehash(bucketCount * 2);
}
```

`rehash()` allocates a new bucket array and relinks every existing node into its new bucket — nodes are moved by pointer, not copied.

### Iterator Design

Iterators track four pieces of state: the current node, the bucket array, the current bucket index, and the bucket count. Forward iteration walks the current chain, then advances to the next non-empty bucket. Reverse iteration walks backward through prior buckets to find each chain's tail.

### Exception Safety Model

- `rehash()` builds the new bucket array before touching the old one — if allocation fails, the map is untouched
- Copy construction and copy assignment insert into a `try`/`catch` block — on failure mid-copy, the partially built state is released and the exception is rethrown
- Move construction and move assignment are `noexcept` — ownership transfer only, no allocation
- `at()` throws `std::out_of_range` on a missing key; all other lookup operations are `noexcept`

---

## Complexity

### Time Complexity

| Operation      | Complexity (avg) | Complexity (worst) | Notes                                   |
| -------------- | ----------------- | ------------------- | ---------------------------------------- |
| `insert`       | O(1)               | O(n)                 | Worst case under heavy collision         |
| `find`         | O(1)               | O(n)                 | Chain walk within one bucket             |
| `contains`     | O(1)               | O(n)                 | Delegates to `find`                      |
| `at`           | O(1)               | O(n)                 | Throws on miss                           |
| `operator[]`   | O(1)               | O(n)                 | Inserts default value on miss            |
| `update`       | O(1)               | O(n)                 | No insertion on miss                     |
| `erase`        | O(1)               | O(n)                 | Chain walk to unlink node                |
| `clear`        | O(n)               | O(n)                 | Deletes every node                       |
| `rehash`       | O(n)               | O(n)                 | Relinks every node into new buckets      |

### Space Complexity

- O(n) for stored elements, one heap-allocated `Node` per element
- O(b) for the bucket array, where `b` is the current bucket count
- O(1) additional metadata per `HashMap` instance

### Notes

- Each element incurs one heap allocation (`new Node<K, V>`) — no bulk/arena allocation
- Rehashing relinks existing nodes by pointer; it does not copy or reallocate key/value storage
- `capacity()` returns the current bucket count, not a byte capacity

---

## Quick Example

### Basic Usage

```cpp
#include <HashMapPro/HashMap.h>

using namespace HashMapPro;

int main() {
    HashMap<int, std::string> map{16};

    map.insert(1, "one");
    map.insert(2, "two");

    std::string& value = map[1];
    std::string  found = map.at(2);
}
```

### Safe Lookup

```cpp
#include <HashMapPro/HashMap.h>

using namespace HashMapPro;

int main() {
    HashMap<int, std::string> map{16};
    map.insert(1, "one");

    if (auto it = map.find(1); it != map.end()) {
        // key exists, use it->value
    }

    if (map.contains(1)) {
        // key exists
    }
}
```

### Iteration

```cpp
#include <HashMapPro/HashMap.h>

using namespace HashMapPro;

int main() {
    HashMap<int, std::string> map{16};
    map.insert(1, "one");
    map.insert(2, "two");

    for (const auto& node : map) {
        // node.key, node.value
    }

    for (auto it = map.rbegin(); it != map.rend(); ++it) {
        // reverse traversal
    }
}
```

### Custom Hash

```cpp
#include <HashMapPro/HashMap.h>

using namespace HashMapPro;

struct Point {
    int x, y;
    bool operator==(const Point& other) const { return x == other.x && y == other.y; }
};

struct PointHash {
    std::size_t operator()(const Point& p) const {
        std::size_t hx = std::hash<int>{}(p.x);
        std::size_t hy = std::hash<int>{}(p.y);
        return hx ^ (hy + 0x9e3779b9 + (hx << 6) + (hx >> 2));
    }
};

int main() {
    HashMap<Point, std::string, PointHash> map{16};
    map.insert({0, 0}, "origin");
}
```

---

## Core API

### Constructors

```cpp
explicit HashMap(std::size_t bucketCount = 16);

HashMap(const HashMap& other);              // deep copy
HashMap& operator=(const HashMap& other);   // deep copy

HashMap(HashMap&& other) noexcept;              // move
HashMap& operator=(HashMap&& other) noexcept;   // move
```

### Access

```cpp
[[nodiscard]] V& operator[](const K& key);

[[nodiscard]] V& at(const K& key);
[[nodiscard]] const V& at(const K& key) const;
```

### Modifiers

```cpp
void insert(const K& key, const V& value);
[[nodiscard]] bool update(const K& key, const V& value);
[[nodiscard]] bool erase(const K& key);
void clear();
```

### Lookup

```cpp
[[nodiscard]] iterator find(const K& key);
[[nodiscard]] const_iterator find(const K& key) const;

[[nodiscard]] bool contains(const K& key) const noexcept;
```

### Capacity / State

```cpp
[[nodiscard]] std::size_t capacity() const noexcept;
[[nodiscard]] std::size_t size() const noexcept;
[[nodiscard]] bool empty() const noexcept;
```

### Iterators

```cpp
iterator begin() noexcept;               const_iterator begin() const noexcept;
iterator end() noexcept;                 const_iterator end() const noexcept;
const_iterator cbegin() const noexcept;
const_iterator cend() const noexcept;

reverse_iterator rbegin() noexcept;              const_reverse_iterator rbegin() const noexcept;
reverse_iterator rend() noexcept;                const_reverse_iterator rend() const noexcept;
const_reverse_iterator crbegin() const noexcept;
const_reverse_iterator crend() const noexcept;
```

---

## Benchmark Results

Benchmarks compare `HashMap` against `std::unordered_map` across all operations.
All times are total elapsed time for the listed iteration count.

> Compiled with `-std=c++23`. Results may vary depending on hardware and compiler optimizations.

### Constructor

```
----------------------------------------------------------------------
Constructor Benchmarks                  Time           Iteration
----------------------------------------------------------------------
Hashmap Construct                       349.16 ms       1000000
Std Construct                           369.84 ms       1000000

Hashmap Copy Construct                  14.19 s         500000
Std Copy Construct                      5.38 s          500000

Hashmap Move Construct                  679.65 ms       1000000
Std Move Construct                      648.13 ms       1000000

Hashmap Move Assign                     679.44 ms       1000000
Std Move Assign                         649.83 ms       1000000
----------------------------------------------------------------------
```

### Modifiers

```
----------------------------------------------------------------------
Modifiers Benchmarks                    Time           Iteration
----------------------------------------------------------------------
Hashmap Insert                          606.08 ms       1000000
Std Insert                              638.01 ms       1000000

Hashmap Insert Growth                   12.63 s         500000
Std Insert Growth                       11.92 s         500000

Hashmap Update                          8.74 ms         1000000
Std Update                              25.49 ms        1000000

Hashmap Erase                           874.08 ms       1000000
Std Erase                               907.95 ms       1000000

Hashmap Clear                           5.49 s          500000
Std Clear                               5.41 s          500000
----------------------------------------------------------------------
```

### Lookup

```
----------------------------------------------------------------------
Lookup Benchmarks                       Time           Iteration
----------------------------------------------------------------------
Hashmap Find Hit                        12.68 ms        1000000
Std Find Hit                            13.44 ms        1000000

Hashmap Find Miss                       16.49 ms        1000000
Std Find Miss                           19.07 ms        1000000

Hashmap Contains Hit                    12.82 ms        1000000
Std Contains Hit                        14.14 ms        1000000

Hashmap Contains Miss                   16.40 ms        1000000
Std Contains Miss                       19.39 ms        1000000
----------------------------------------------------------------------
```

### Access

```
----------------------------------------------------------------------
Access Benchmarks                       Time           Iteration
----------------------------------------------------------------------
Hashmap Subscript Existing              12.82 ms        1000000
Std Subscript Existing                  29.75 ms        1000000

Hashmap Subscript Missing               621.37 ms       1000000
Std Subscript Missing                   643.52 ms       1000000

Hashmap At Existing                     12.46 ms        1000000
Std At Existing                         13.66 ms        1000000
----------------------------------------------------------------------
```

### Iteration

```
----------------------------------------------------------------------
Iteration Benchmarks                    Time           Iteration
----------------------------------------------------------------------
Hashmap Traverse Small                  166.93 ms       500000
Std Traverse Small                      41.56 ms        500000

Hashmap Traverse Large                  1.03 s          100000
Std Traverse Large                      493.04 ms       100000

Hashmap Traverse Reverse                300.19 ms       500000
Std Traverse Forward Baseline           41.60 ms        500000
----------------------------------------------------------------------
```

### Summary

Lookup operations are HashMap's strongest category. `Contains Hit` (12.82 ms vs 14.14 ms)
and `Find Hit` (12.68 ms vs 13.44 ms) are marginally faster than `std::unordered_map` — a
single hash and chain walk is competitive with libstdc++'s bucket-and-node design.

Subscript access shows the widest advantage: `Subscript Existing` (12.82 ms vs 29.75 ms)
is roughly 2.3x faster, since `operator[]` on an existing key resolves through a single
`findNode` call rather than the node-splicing path `std::unordered_map` takes internally.

Update is the other standout: `Update` (8.74 ms vs 25.49 ms) is roughly 2.9x faster —
a direct chain walk and in-place assignment, with no rehash bookkeeping.

Iteration is HashMap's weakest category. `Traverse Small` (166.93 ms vs 41.56 ms) and
`Traverse Large` (1.03 s vs 493.04 ms) are both roughly 2-4x slower — separate chaining
means iteration follows pointers through individually heap-allocated nodes, while
`std::unordered_map`'s implementation benefits from a more cache-friendly internal layout.
Reverse traversal (`Traverse Reverse`: 300.19 ms) carries additional cost from having to
walk each chain from its head to find the predecessor of the current node, since nodes
are singly-linked.

Copy construction (`Copy Construct`: 14.19 s vs 5.38 s) is roughly 2.6x slower — every
element triggers a full `insert()` including a duplicate-key check and a fresh heap
allocation, rather than a bulk node copy.

| Category                | Winner              | Notes                                                    |
| ------------------------ | -------------------- | ---------------------------------------------------------- |
| Find / Contains hit       | HashMap              | Marginally faster — single chain walk                     |
| Find / Contains miss      | HashMap              | Marginally faster across both hit and miss paths          |
| Subscript existing        | HashMap              | ~2.3x faster — single lookup path                          |
| Subscript missing         | HashMap              | Roughly comparable                                         |
| At existing                | HashMap              | Marginally faster                                          |
| Update                    | HashMap              | ~2.9x faster — direct in-place assignment                  |
| Insert                    | HashMap              | Marginally faster                                          |
| Insert growth              | std::unordered_map   | Roughly comparable, slight edge to std                     |
| Erase                      | HashMap              | Marginally faster                                          |
| Clear                      | std::unordered_map   | Roughly comparable                                         |
| Construct                  | HashMap              | Marginally faster                                          |
| Copy construct               | std::unordered_map   | ~2.6x faster — bulk copy vs per-element insert              |
| Move construct / assign      | std::unordered_map   | Roughly comparable, slight edge to std                      |
| Forward iteration             | std::unordered_map   | 2-4x faster — cache-friendlier internal layout               |
| Reverse iteration               | —                     | No native `std::unordered_map` reverse iterator to compare  |

**Use HashMap when:** lookup, update, and subscript access dominate the workload, and
iteration order and iteration speed are not critical.
**Use `std::unordered_map` when:** the workload is iteration-heavy or copy-heavy, or a
more mature, extensively-optimized implementation is preferred for production use.

---

## Project Structure

```
HashMap/
├── include/
│   └── HashMapPro/
│       ├── HashMap.h
│       ├── HashMap.tpp
│       ├── Iterator.h
│       └── Node.h
├── tests/
├── benchmarks/
├── examples/
├── cmake/
├── CMakeLists.txt
├── README.md
└── LICENSE
```

---

## Build Instructions

### Requirements

- GCC 16+ or Clang with C++23 support
- CMake 3.20+

### Build

```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

### Run Tests

```bash
./tests
```

### Run Benchmarks

```bash
./benchmarks
```

### Run Examples

```bash
./example_basic
./example_modifiers
./example_iteration
./example_lookup
./example_custom_hash
```

---

## Notes

- `insert()` on an existing key is a no-op — use `update()` to overwrite an existing value.
- `operator[]` inserts a default-constructed `V` for a missing key, matching `std::unordered_map` semantics.
- `at()` throws `std::out_of_range` on a missing key, for both the const and non-const overloads.
- `capacity()` returns the current bucket count, not a byte size.
- Rehashing occurs automatically once the load factor exceeds `0.75`; there is no manual `reserve()` or `rehash()` exposed in the public API.
- Iterators are invalidated by any operation that triggers a rehash (`insert()` past the load factor threshold).

---

## License

[MIT](LICENSE) — free to use, modify, and distribute for educational and personal purposes.
