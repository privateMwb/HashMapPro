# Changelog

All notable changes to HashMapPro are documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added
- Nothing yet.

## [1.0.0] - 2026-07-16

The first stable release of HashMapPro, a high-performance separate-chaining
hash map for modern C++.

### Added
- Separate chaining with O(1) average-case lookup, insertion, and removal.
- Power-of-two bucket sizing with bitmask indexing instead of modulo.
- Automatic rehashing once the load factor exceeds 0.75.
- Lazily-allocated storage, so a moved-from map can be safely reused.
- `insert()` and `update()` for element insertion and value replacement.
- `find()`, `contains()`, `operator[]`, and `at()` lookups.
- `erase()` and `clear()` for element removal.
- `reserve()` for explicit capacity management.
- Bidirectional forward and reverse iteration, with const and non-const variants.
- Copy and move construction and assignment support.
- Custom key types via a user-supplied `Hash` functor.
- Exception-safe cloning on copy construction/assignment.
- `rain::` namespace alias for `HashMap`.

### Performance
- Bitmask indexing (`hash & (capacity - 1)`) replaces division/modulo on
  the hot path.
- Bucket-chain prepend insertion keeps single-key insertion O(1).
- `findNode()` reuses a single hash computation across insert,
  `operator[]`, and `find()`.
- Bucket count is always kept a power of two, so growth and indexing
  stay cheap.
- `cloneFrom()` copies elements directly during copy, skipping the
  per-element existence check `insert()` would otherwise perform.
- Rehashing relinks existing nodes in place rather than reallocating or
  copying elements.
- Benchmarked against `std::unordered_map` at 10K / 100K / 1M
  iterations; largest wins on no-op and lookup-heavy workloads
  (`Insert() Existing`, `Contains()`, `Update()`, `At()`) at the 1M
  scale, with `Insert() New`, `Erase() Missing`, `Clear()`, and `Move
  Construct` also ahead; full-reallocation paths like `Reserved
  Construct` and `Copy Assignment` trail `std::unordered_map` instead.
  Full results in `benchmarks/results/v1_0_0.md`.

### Testing
- Comprehensive test suite covering unit, integration, lifecycle,
  move semantics, regression, and concurrency tests; exception safety;
  custom hash functor workflows; rehash and load-factor boundary
  behavior; power-of-two bucket rounding; hash collision chains;
  sparse bucket iteration; and external synchronization contracts.
- 93.0% line coverage and 99.0% function coverage, excluding test
  infrastructure.

### CI
- Automated builds and tests across GCC, Clang, MSVC, and AppleClang,
  each in Debug and Release configurations.

[Unreleased]: https://github.com/privateMwb/HashMapPro/compare/v1.0.0...HEAD
[1.0.0]: https://github.com/privateMwb/HashMapPro/releases/tag/v1.0.0
