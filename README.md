# HashMapPro

<p align="center">
  <img src="https://img.shields.io/github/v/release/privateMwb/HashMapPro?style=for-the-badge&logo=github&color=yellow" alt="Version">
  <img src="https://img.shields.io/badge/License-MIT-orange?style=for-the-badge" alt="License - MIT">
  <img src="https://img.shields.io/badge/C%2B%2B-23-blue?style=for-the-badge&logo=c%2B%2B" alt="C++ - 23">
</p>

<p align="center">
  <a href="https://github.com/privateMwb/HashMapPro/actions/workflows/build.yml">
    <img src="https://github.com/privateMwb/HashMapPro/actions/workflows/build.yml/badge.svg" alt="Build and Test">
  </a>
  <a href="https://github.com/privateMwb/HashMapPro/actions/workflows/benchmark.yml">
    <img src="https://github.com/privateMwb/HashMapPro/actions/workflows/benchmark.yml/badge.svg" alt="Benchmarks">
  </a>
  <a href="https://github.com/privateMwb/HashMapPro/actions/workflows/coverage.yml">
    <img src="https://github.com/privateMwb/HashMapPro/actions/workflows/coverage.yml/badge.svg" alt="Coverage">
  </a>
  <a href="https://github.com/privateMwb/HashMapPro/actions/workflows/sanitizers.yml">
    <img src="https://github.com/privateMwb/HashMapPro/actions/workflows/sanitizers.yml/badge.svg" alt="Sanitizers">
  </a>
  <a href="https://github.com/privateMwb/HashMapPro/actions/workflows/clang-tidy.yml">
    <img src="https://github.com/privateMwb/HashMapPro/actions/workflows/clang-tidy.yml/badge.svg" alt="Clang Tidy">
  </a>
  <a href="https://github.com/privateMwb/HashMapPro/actions/workflows/clang-format.yml">
    <img src="https://github.com/privateMwb/HashMapPro/actions/workflows/clang-format.yml/badge.svg" alt="Clang Format">
  </a>
  <a href="https://github.com/privateMwb/HashMapPro/actions/workflows/docs.yml">
    <img src="https://github.com/privateMwb/HashMapPro/actions/workflows/docs.yml/badge.svg" alt="Documentation">
  </a>
  <a href="https://github.com/privateMwb/HashMapPro/actions/workflows/release.yml">
    <img src="https://github.com/privateMwb/HashMapPro/actions/workflows/release.yml/badge.svg" alt="Release">
  </a>
</p>

<p align="center">
  <img src="https://img.shields.io/badge/GCC-support-B46F1B?style=flat&logo=gnu" alt="GCC - support">
  <img src="https://img.shields.io/badge/Clang-support-045891?style=flat&logo=llvm" alt="Clang - support">
  <img src="https://img.shields.io/badge/MSVC-support-5C2D91?style=flat" alt="MSVC - support">
  <img src="https://img.shields.io/badge/AppleClang-support-000000?style=flat&logo=apple" alt="AppleClang - support">
</p>

A header-only, C++23, `std::unordered_map`-compatible separate-chaining hash
map, built from scratch with a power-of-two bucket count and an explicit
`reserve()`/`rehash()` API for controlling allocation up front.

## 📑 Table of Contents

- [Features](#-features)
- [Requirements](#-requirements)
- [Installation](#-installation)
- [Quick Start](#-quick-start)
- [Project Structure](#project-structure)
- [Development](#development)
- [Benchmarks](#-benchmarks)
- [Documentation](#-documentation)
- [Contributing](#-contributing)
- [Changelog](#-changelog)
- [License](#-license)

## <a id="features"></a>✨ Features

- **Familiar `std::unordered_map` API** — `operator[]`, `at()`, `insert()`,
  `update()`, `erase()`, `contains()`, `find()`, `clear()`, forward/reverse
  iterators, and more.
- **Separate chaining** with a power-of-two bucket count, so lookups can
  mask instead of dividing when locating a bucket.
- **Custom hash support** — the hash function is a template parameter
  (`Hash = std::hash<K>` by default), so any type satisfying the standard
  hash interface works.
- **Explicit capacity control** — `reserve(desiredCapacity)` computes the
  smallest bucket count that keeps the load factor under the configured
  maximum and rehashes up front, useful before a bulk-insert loop.
- **`update()` distinct from `insert()`** — `insert()` is a no-op if the
  key already exists; `update()` only replaces an existing key's value and
  reports whether it found one, so intent is explicit at the call site.
- **Copy and move semantics** — deep-copying copy constructor/assignment,
  and move construction/assignment that leaves the source in a valid,
  reusable empty state.
- **C++23 throughout** the header and iterator implementation.

## <a id="requirements"></a>📋 Requirements

- A C++23-conformant compiler (tested: GCC, Clang, MSVC, AppleClang)
- CMake 3.20+

## <a id="installation"></a>📦 Installation

**From source:**

```bash
git clone https://github.com/privateMwb/HashMapPro.git
cd HashMapPro
cmake -B build \
  -DHASHMAPPRO_BUILD_TESTS=OFF \
  -DHASHMAPPRO_BUILD_BENCHMARKS=OFF \
  -DHASHMAPPRO_BUILD_TOOLS=OFF \
  -DHASHMAPPRO_BUILD_EXAMPLES=OFF
cmake --install build
```

Then, in your own `CMakeLists.txt`:

```cmake
find_package(HashMapPro CONFIG REQUIRED)
target_link_libraries(your_target PRIVATE HashMapPro::HashMapPro)
```

> vcpkg and Conan packages are built and verified, but not yet published to
> the public registries. This section will be updated once they are.

## <a id="quick-start"></a>🚀 Quick Start

**Basic usage:**

```cpp
#include <HashMapPro/HashMap.h>
#include <iostream>

int main() {
    HashMapPro::HashMap<int, std::string> map;
    map.insert(1, "one");
    map.insert(2, "two");

    std::cout << map.at(1) << " " << map.at(2) << "\n"; // one two
}
```

**Reserving capacity before a bulk insert:**

```cpp
#include <HashMapPro/HashMap.h>

int main() {
    HashMapPro::HashMap<int, int> map;
    map.reserve(10'000); // rehash once, up front, instead of repeatedly

    for (int i = 0; i < 10'000; ++i) {
        map.insert(i, i * i);
    }
}
```

**Using a custom hash function:**

```cpp
#include <HashMapPro/HashMap.h>
#include <string>

struct CaseInsensitiveHash {
    std::size_t operator()(const std::string& key) const {
        std::string lower = key;
        for (auto& c : lower) c = std::tolower(static_cast<unsigned char>(c));
        return std::hash<std::string>{}(lower);
    }
};

int main() {
    HashMapPro::HashMap<std::string, int, CaseInsensitiveHash> map;
    map.insert("Hello", 1);
}
```

**`update()` vs. `insert()`:**

```cpp
#include <HashMapPro/HashMap.h>

int main() {
    HashMapPro::HashMap<int, std::string> map;
    map.insert(1, "one");

    map.insert(1, "ONE");        // no-op — key already exists
    bool updated = map.update(1, "ONE"); // true — replaces the value
    bool missed  = map.update(2, "two"); // false — key 2 doesn't exist
}
```

## <a id="project-structure"></a>🗂️ Project Structure

```
HashMapPro/
├── include/
│   └── HashMapPro/
│       ├── HashMap.h
│       ├── HashMap.tpp
│       ├── Iterator.h
│       └── Node.h
│
├── tests/
│   ├── common/
│   ├── suite/
│   ├── test_all.cpp
│   └── CMakeLists.txt
│
├── benchmarks/
│   ├── common/
│   ├── suite/
│   ├── baselines/
│   ├── bench_all.cpp
│   └── CMakeLists.txt
│
├── examples/
│   ├── common/
│   ├── suite/
│   ├── example_all.cpp
│   └── CMakeLists.txt
│
├── tools/
│   ├── regression/
│   └── CMakeLists.txt
│
├── recipes/
│   └── hashmappro/          # Conan recipe
│
├── vcpkg/
│   └── ports/                # vcpkg port
│
├── cmake/
│   └── HashMapProConfig.cmake.in
│
├── docs/
│   ├── Doxyfile
│   └── PACKAGING.md
│
├── .gitignore
├── CMakeLists.txt
├── README.md
└── LICENSE
```

## <a id="development"></a>🛠️ Development

The from-source install above builds the library only. To work on
HashMapPro itself — running tests, benchmarks, or the regression tool —
build with everything enabled (the default):

```bash
cmake -B build
cmake --build build
```

**Run the test suite:**

```bash
ctest --test-dir build
```

**Run benchmarks and check for regressions:**

```bash
./build/benchmarks
./build/regression   # current results vs. benchmarks/baselines/v1.0.0.json
```

**Sanitizers, static analysis, and formatting** are covered by the
`Sanitizers`, `Clang Tidy`, and `Clang Format` CI workflows — see
`.github/workflows/` for the exact invocations if you want to reproduce
them locally.

**Build the docs locally:**

```bash
doxygen docs/Doxyfile
```

See `docs/PACKAGING.md` for notes on verifying the vcpkg port and Conan
recipe locally.

## <a id="benchmarks"></a>📊 Benchmarks

Measured against `std::unordered_map`, same build, at 10K / 100K / 1M
elements (`benchmarks/baselines/v1.0.0.json` has the full dataset).
Numbers below are the 100K row unless noted.

**At parity or faster:**

| Operation | HashMapPro | std::unordered_map | Difference |
|---|---|---|---|
| `At existing` | 4.19 ms | 19.76 ms | ~78% faster |
| `Update existing` | 8.81 ms | 34.86 ms | ~75% faster |
| `Contains miss` | 4.43 ms | 15.62 ms | ~72% faster |
| `Insert existing` | 8.38 ms | 28.44 ms | ~71% faster |
| `Rehash trigger` | 5.89 s | 13.39 s | ~56% faster |
| `Move assignment` | 5.94 s | 12.25 s | ~52% faster |
| `Copy assignment` | 4.90 s | 4.90 s | parity |

**Slower, and consistent across scale — worth fixing:**

| Operation | HashMapPro | std::unordered_map | Difference |
|---|---|---|---|
| `Clear` (empty map) | 106.33 ms | 1.64 ms | ~65x slower |
| `Default construct` | 87.41 ms | 12.06 ms | ~86% slower |
| `Reserve` (explicit call) | 1.33 s | 503.40 ms | ~62% slower |
| `Reserved construct` | 377.69 ms | 251.14 ms | ~34% slower |
| `Reverse traverse` | 1.70 s | 774.98 ms | ~55% slower |

<details>
<summary>Why the gap on these specific operations</summary>

`clear()` unconditionally walks every bucket (`bucketCount_` of them) and
frees any chained nodes, even on an empty map — there's no early exit for
the zero-element case. That's the source of the ~65x gap: it's paying
O(bucket count) instead of O(element count), and an empty map still has
16+ allocated buckets to walk. The default constructor has a matching
cost: it eagerly allocates and null-initializes the full bucket array
(minimum 16 buckets) rather than deferring allocation until the first
insert, which is why `Default construct` shows a consistent ~85%+ gap at
every scale rather than narrowing as element count grows.

`reserve()` and reserved-capacity construction pay a similar up-front
cost — computing the target bucket count and allocating/rehashing into it
immediately, rather than lazily on first use. This is a deliberate
trade-off (predictable one-time cost before a bulk-insert loop, matching
the "reserve before a bulk insert" usage pattern in Quick Start) but it
does mean an isolated `reserve()` call, measured on its own, looks
worse than `std::unordered_map`'s more incremental growth.

The `Reverse traverse` gap and the `Erase existing` anomaly at the 1M row
specifically (present in the full dataset but not shown above, since it's
inconsistent with the 10K/100K rows for the same operation) are still
under investigation — see `benchmarks/baselines/v1.0.0.json` for the raw
numbers if you want to look yourself.

</details>

## <a id="documentation"></a>📖 Documentation

Full API reference (generated with Doxygen, updated on every push to
`main`):

**https://privateMwb.github.io/HashMapPro/**

## <a id="contributing"></a>🤝 Contributing

Issues and pull requests are welcome. Before submitting a PR:

- Run the test suite (`ctest --test-dir build`)
- Format with `clang-format` (or let the `Clang Format` CI check catch it)
- If you're changing a hot path, run `./build/regression` and mention the
  results in your PR description

## <a id="changelog"></a>📝 Changelog

See the [Releases](https://github.com/privateMwb/HashMapPro/releases) page
for version history and release notes.

## <a id="license"></a>📄 License

MIT — see [LICENSE](LICENSE) for details.
