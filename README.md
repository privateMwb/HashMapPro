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
  <a href="https://github.com/privateMwb/HashMapPro/actions/workflows/packaging.yml">
    <img src="https://github.com/privateMwb/HashMapPro/actions/workflows/packaging.yml/badge.svg" alt="Packaging">
  </a>
</p>

<p align="center">
  <img src="https://img.shields.io/badge/GCC-support-B46F1B?style=flat&logo=gnu" alt="GCC - support">
  <img src="https://img.shields.io/badge/Clang-support-045891?style=flat&logo=llvm" alt="Clang - support">
  <img src="https://img.shields.io/badge/MSVC-support-5C2D91?style=flat" alt="MSVC - support">
  <img src="https://img.shields.io/badge/AppleClang-support-000000?style=flat&logo=apple" alt="AppleClang - support">
</p>

HashMapPro is a header-only separate-chaining hash map for modern C++ — O(1) average-case lookup/insert/erase, power-of-two bucket sizing with bitmask indexing instead of modulo, and automatic rehashing that relinks existing nodes in place rather than reallocating or copying elements.

## 📑 Table of Contents

- [Features](#features)
- [Requirements](#requirements)
- [Installation](#installation)
- [Quick Start](#quick-start)
- [Project Structure](#project-structure)
- [Development](#development)
- [Benchmarks](#benchmarks)
- [Documentation](#documentation)
- [Contributing](#contributing)
- [Changelog](#changelog)
- [License](#license)

## <a id="features"></a>✨ Features

- **Separate chaining with power-of-two bucket sizing** — bucket count is always a power of two, so `bucketIndex()` masks (`hash & (capacity - 1)`) instead of dividing/modulo-ing on every lookup, insert, and erase.
- **Automatic rehashing without touching elements** — once the load factor exceeds 0.75, `rehash()` relinks every existing node into a new bucket array; nodes are never reallocated or copied, so outstanding values stay at the same address.
- **Lazily-allocated, reusable storage** — a moved-from map holds no bucket array at all rather than a dangling or zeroed-out one, and quietly reallocates on the next insertion, so it's safe to keep using instead of only destroying or reassigning.
- **Single hash computation per operation** — `findNode()` returns both the matching node (if any) and its bucket index, so `insert()`, `operator[]`, and `find()` never hash the same key twice.
- **Strong exception guarantee on copy** — `cloneFrom()` clones elements directly (skipping the per-element existence check `insert()` would otherwise do), and rolls back cleanly, releasing everything already cloned, if a `K`/`V` copy constructor throws partway through.
- **Full bidirectional iteration** — `begin()`/`end()`, `rbegin()`/`rend()`, and their `c`-prefixed const counterparts all traverse bucket-chain order correctly, skipping empty buckets in either direction.

## <a id="requirements"></a>📋 Requirements

- A C++23-conformant compiler (tested: GCC, Clang, MSVC, AppleClang)
- CMake 3.20+

## <a id="installation"></a>📦 Installation

**From source:**

```bash
git clone https://github.com/privateMwb/HashMapPro.git
cd HashMapPro
cmake -B build \
  -DBUILD_TESTS=OFF \
  -DBUILD_BENCHMARKS=OFF \
  -DBUILD_REGRESSION=OFF \
  -DBUILD_EXAMPLES=OFF
cmake --install build
```

Then, in your own `CMakeLists.txt`:

```cmake
find_package(HashMapPro CONFIG REQUIRED)
target_link_libraries(your_target PRIVATE HashMapPro::HashMapPro)
```

> vcpkg and Conan packages are built and verified (recipe in
> `packaging/recipes/hashmappro/`, port in `packaging/vcpkg/ports/hashmappro/`),
> but not yet published to the public registries. This section will be
> updated once they are.

## <a id="quick-start"></a>🚀 Quick Start

```cpp
#include <HashMapPro/HashMap.h>

int main() {
    HashMapPro::HashMap<std::string, int> map{16};

    map.insert("alpha", 1);
    map.insert("beta", 2);
    map.insert("gamma", 3);

    if (map.contains("alpha")) {
        // present
    }

    auto it = map.find("beta");
    if (it != map.end()) {
        // it->key == "beta", it->value == 2
    }
}
```

`operator[]` and reading back load factor:

```cpp
HashMapPro::HashMap<std::string, int> counts{16};

counts["fox"]++; // default-constructs 0, then increments
counts["fox"]++;

std::cout << counts.at("fox") << " occurrences\n";
std::cout << counts.load_factor() << " / " << counts.max_load_factor() << " max\n";
```

A moved-from map is left valid and empty, safe to reuse rather than only destroy:

```cpp
HashMapPro::HashMap<int, std::string> source{16};
source.insert(1, "one");

HashMapPro::HashMap<int, std::string> destination = std::move(source);

// source.size() == 0 here — safe to keep using, storage reallocates lazily
source.insert(2, "two");
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
│   ├── support/
│   ├── suite/
│   ├── test_main.cpp
│   └── CMakeLists.txt
│
├── benchmarks/
│   ├── support/
│   ├── suite/
│   ├── baselines/
│   ├── bench_main.cpp
│   └── CMakeLists.txt
│
├── examples/
│   ├── support/
│   ├── suite/
│   ├── example_main.cpp
│   └── CMakeLists.txt
│
├── regression/
│   ├── support/
│   ├── regression_main.cpp
│   └── CMakeLists.txt
│
├── packaging/
│   ├── README.md
│   ├── recipes/
│   │   └── hashmappro/
│   ├── vcpkg/
│   │   └── ports/
│   │       └── hashmappro/
│   └── vcpkg-smoke-test/
│
├── scripts/
│   └── update_package_files.py
│
├── .github/
│   ├── releases/
│   └── workflows/
│
├── cmake/
│   └── HashMapProConfig.cmake.in
│
├── docs/
│   ├── Doxyfile
│   └── README.md
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
./build/regression                  # latest baseline vs. benchmarks/results/benchmark_results.json
./build/regression v1.2.0           # a specific baseline vs. current
./build/regression v1.2.0 v1.4.0    # two baselines against each other
```

`regression` picks the latest baseline by semantic version (`v1.10.0`
correctly outranks `v1.9.0`), not alphabetical filename order, and
auto-names its output (`regression_v1.2.0_vs_current.md`/`.json`, etc.).

See [packaging/README.md](packaging/README.md) for notes on verifying the vcpkg
port and Conan recipe locally.

## <a id="benchmarks"></a>📊 Benchmarks

Measured against `std::unordered_map`, same build, at 10K / 100K / 1M
iterations (`benchmarks/results/v1_0_0.md` has the full dataset).

| Operation | HashMapPro (1M) | std::unordered_map (1M) | Δ |
|---|---|---|---|
| `Insert() Existing` | 2.29 ms | 14.63 ms | +538.3% |
| `Contains() Hit` | 934.05 us | 2.17 ms | +132.7% |
| `Update() Existing` | 988.29 us | 2.30 ms | +132.3% |
| `At() Existing` | 939.44 us | 2.17 ms | +131.4% |
| `Insert() New` | 1.36 s | 2.82 s | +107.0% |
| `Erase() Missing` | 1.34 s | 2.68 s | +99.6% |
| `Clear() Populated` | 1.40 s | 2.64 s | +88.5% |
| `Move Construct` | 2.30 s | 4.26 s | +85.6% |
| `Find() Hit` | 1.25 ms | 2.17 ms | +73.4% |
| `Reserved Construct` | 150.24 ms | 37.85 ms | -74.8% |
| `Copy Assignment` | 2.61 s | 256.05 ms | -90.2% |

HashMapPro's bitmask-indexed, separate-chaining design pays off most on
no-op paths that skip real work (`Insert() Existing` short-circuits on
the first matching node without allocating), and on the ordinary
hit/miss lookups (`Contains()`, `At()`, `Update()`, `Find()`) that
dominate typical usage.

The trade-off: bucket storage is allocated lazily but grows by full
reallocation, so operations that touch every element under a fresh
allocation — `Reserved Construct`, and especially `Copy Assignment`,
which releases the destination before cloning — pay that cost directly
rather than amortizing it the way `std::unordered_map`'s node-based
allocator does.

## <a id="documentation"></a>📖 Documentation

Full API reference, generated with Doxygen from `docs/Doxyfile`:

**https://privateMwb.github.io/HashMapPro/**

## <a id="contributing"></a>🤝 Contributing

Issues and pull requests are welcome. Before submitting a PR:

- Run the test suite (`ctest --test-dir build`)
- If you're changing a hot path, run `./build/regression` and mention
  the results in your PR description

## <a id="changelog"></a>📝 Changelog

See the [Releases](https://github.com/privateMwb/HashMapPro/releases)
page for version history and release notes.

## <a id="license"></a>📄 License

MIT — see [LICENSE](LICENSE) for details.
