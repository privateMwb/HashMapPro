# Example Suite

This document describes the example categories under `suite/` — what
each one demonstrates, and the individual example files it contains.

Unlike the test suite, an example doesn't assert correctness — it
demonstrates real usage of the library, including deliberate misuse
where instructive (see Misuse), so the reader sees both the correct
pattern and the mistake it guards against.

Every example file ends with `REGISTER_EXAMPLE_SUITE()`, which derives
the suite's category from its containing directory and assigns it a
sequential id within that category. This applies uniformly across
every category below.

---

## Advanced

Demonstrates deeper mechanics of the library — bucket placement and
automatic rehashing, and the distinction between a deep copy and a
cheap ownership transfer.

### Examples

- `bucket_inspection.cpp` — reasoning about bucket placement with the
  same hash-and-mask formula the map uses internally, counting how many
  keys land in each bucket, and how growth redistributes keys across a
  larger bucket set
- `rehash_growth.cpp` — automatic rehashing and capacity doubling as
  elements are inserted, reading `load_factor()`/`max_load_factor()`,
  and capacity always rounding up to a power of two regardless of the
  requested size
- `copy_vs_move.cpp` — deep-copying a map with the copy constructor and
  the independence of the original and the copy, cheap ownership
  transfer with the move constructor, and the source map's state
  immediately after a move
- `reuse_after_move.cpp` — a moved-from map left in a valid, empty
  state, safely reusing it instead of only destroying it, lazily-
  allocated storage recovering on the next insertion, and move
  assignment into an already-populated map

---

## Integration

Demonstrates interoperability with the rest of a codebase — embedding
the map inside a domain-specific class, working through it by const
reference, and exchanging data with standard containers and algorithms.

### Examples

- `class_wrapping.cpp` — embedding a HashMap as a private member behind
  a narrow, purpose-built public interface, and the wrapper class
  benefiting from HashMap's own copy/move semantics
- `const_correctness.cpp` — passing a HashMap by const reference, the
  const-qualified overloads of `at()`, `find()`, `begin()`, and `end()`,
  and `const_iterator` selected automatically for a const map
- `vector_of_pairs.cpp` — building a HashMap from a `std::vector` of
  key-value pairs, reserving capacity based on the source vector's
  size, and draining a map's contents back into a `std::vector`
- `stl_algorithms.cpp` — *not included in this upload; description
  pending*

---

## Misuse

Demonstrates common mistakes and the exceptions or undefined behavior
they lead to, alongside the correct pattern — including examples shown
but not executed, so the reader can see what to avoid without the
program actually invoking undefined behavior.

### Examples

- `end_dereference.cpp` — why dereferencing `end()` is undefined
  behavior, the correct pattern of guarding with a comparison against
  `end()`, and the same rule applied to a failed `find()`
- `stale_iterator.cpp` — why an iterator to an erased element must not
  be used afterward, re-lookup or re-iterate as the correct pattern,
  and that other, non-erased iterators remain valid
- `missing_key.cpp` — `at()` throwing `std::out_of_range` on a missing
  key, the risk of calling `at()` without checking `contains()` first,
  and the correct pattern of checking first or catching the exception
- `use_after_move.cpp` — the mistake of expecting a moved-from map to
  still hold its data, what a moved-from map actually looks like
  (valid, but empty), and the correct pattern of treating it as empty
  rather than stale
- `self_assignment.cpp` — assigning a map to itself via copy assignment
  and via move assignment, and that both are safe no-ops rather than
  data-corrupting mistakes

---

## Patterns

Demonstrates common usage idioms built on top of the core API — custom
key types, aggregate values, capacity planning, and a classic counting
pattern.

### Examples

- `custom_hash.cpp` — defining a custom key type with a custom hash
  functor, constructing a HashMap with a non-default `Hash`, and
  inserting/looking up custom keys
- `struct_values.cpp` — storing an aggregate struct as the mapped
  value, updating individual fields in place through `operator[]`, and
  reading back struct members after mutation
- `reserve_capacity.cpp` — reserving capacity before a bulk insertion
  loop to avoid repeated rehashing, and capacity growth with and
  without `reserve()`
- `word_counter.cpp` — a word-frequency counting pattern using
  `std::string` keys, combining `operator[]` with default-constructed
  values, and iterating the results

---

## Quickstart

Demonstrates fundamental, everyday usage — construction, inserting and
accessing entries, looking keys up, traversing the map, and the core
modifying operations.

### Examples

- `basic.cpp` — construction with an initial capacity, `insert()`,
  `operator[]` and `at()`, `contains()`, `size()`/`capacity()`/`empty()`
- `lookup.cpp` — `contains()`, `find()`, handling a missing key, `at()`
  with exception handling
- `modifiers.cpp` — `insert()`, duplicate insert behavior, `update()`,
  `erase()`, `clear()`
- `iteration.cpp` — forward iteration, range-based iteration, mutating
  values through iteration, reverse iteration, const iteration
