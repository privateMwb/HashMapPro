# Benchmark Suite

This document describes the benchmark categories under `suite/` — what
each one measures, and the individual benchmarks it contains.

Every benchmark compares HashMap against std::unordered_map — the
standard separate-chaining hash table in the C++ standard library, and
the natural baseline for a hand-rolled hash map. A category can support
more than one standard for comparison, but for now each category is
benchmarked against a single standard.

Every `BENCH()` call, in every category below, pits a HashMap lambda
against an equivalent std::unordered_map lambda for the same operation,
so the two are always measured under identical setup.

---

## Access

Benchmarks read and lookup operations on an already-populated map —
retrieving and inserting via element access, and checking for key
presence.

### Benchmarks

- `element_access.cpp` — `operator[]` on an existing key, `operator[]`
  inserting a missing key, `at()` on an existing key
- `lookup.cpp` — `find()` hit, `find()` miss, `contains()` hit,
  `contains()` miss

---

## Core

Benchmarks the fundamental, most frequently exercised operations —
inserting, updating, erasing, and clearing entries.

### Benchmarks

- `insert.cpp` — `insert()` of a new key, `insert()` of an
  already-present key (no-op)
- `update.cpp` — `update()` of an existing key, `update()` of a missing
  key (no-op)
- `erase.cpp` — `erase()` of an existing key, `erase()` of a missing key
  (no-op)
- `clear.cpp` — `clear()` on an empty map, `clear()` on a populated map

---

## Lifecycle

Benchmarks object lifetime operations — construction, destruction,
copying, and moving.

### Benchmarks

- `constructor.cpp` — default construction, construction with a
  reserved bucket count, construction followed by populating a handful
  of elements
- `copy.cpp` — copy construction, copy assignment
- `move.cpp` — move construction, move assignment
- `destructor.cpp` — construct-destroy cost for an empty map,
  construct-populate-destroy cost for a populated map (destruction can
  only be observed by letting a map fall out of scope, so each case
  necessarily times construction plus destruction together)

---

## Scaling

Benchmarks how per-operation cost changes with the map's size and
bucket state — load factor, rehashing, and pre-reserved capacity —
rather than the flat, fixed-size operations measured elsewhere in the
suite.

### Benchmarks

- `load_factor.cpp` — `find()` at a sparse load factor vs. a dense load
  factor (just under the 0.75 rehash threshold), with bucket count held
  fixed so chain length is the only variable
- `rehash.cpp` — insertion that crosses the load factor threshold and
  triggers a rehash, vs. an equal-sized insertion into a map already
  sized large enough that no rehash occurs
- `reserve.cpp` — cost of `reserve()` alone on an empty map, bulk
  insertion without pre-reserving (organic growth/rehashing), bulk
  insertion with pre-reserved capacity (no rehashing)

---

## Utility

Benchmarks introspection operations that don't belong to any of the
categories above — size/capacity reporting and full traversal.

### Benchmarks

- `capacity.cpp` — `size()`, `capacity()` (bucket count), `empty()`
- `iteration.cpp` — forward traversal, reverse traversal (compared
  against std::unordered_map's forward traversal, since it has no
  reverse iterators — the closest available baseline, not a
  like-for-like comparison)
