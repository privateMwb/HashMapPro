/**
 * @file            HashMap.h
 *
 * @date            2026-16-7
 *
 * @version         1.0.0
 *
 * @copyright       Copyright (c) 2026 Your Name
 *                  All rights reserved.
 *                  https://github.com/yourhandle/HashMapPro
 *
 * @attention       This source is released under the MIT license
 *                  SPDX-License-Identifier: MIT
 *                  <http://opensource.org/licenses/MIT>
 */

#pragma once

#include "Iterator.h"
#include "Node.h"

// clang-format off
#include <cstddef>    // std::size_t
#include <functional> // std::hash
#include <iterator>   // std::reverse_iterator
// clang-format on

// A separate-chaining hash map with automatic rehashing, power-of-two
// bucket sizing (bitmask indexing instead of modulo), and lazily-allocated
// storage so a moved-from instance can be safely reused rather than only
// destroyed or reassigned.

namespace HashMapPro {

/**
 * @brief A hash map using separate chaining for collision resolution.
 * @tparam K Key type. Must be equality-comparable and hashable by `Hash`.
 * @tparam V Value type.
 * @tparam Hash Hash function object type. Defaults to `std::hash<K>`.
 * @details Stores key-value pairs in per-bucket linked lists and rehashes
 * automatically once the load factor exceeds `max_load_factor()`. Bucket
 * count is always kept a power of two so bucket indexing can use a bitmask
 * rather than a division/modulo.
 */
template <typename K, typename V, typename Hash = std::hash<K>> class HashMap {
  public:
    // Standard iterator types.
    using NodeType = Node<K, V>;
    using iterator = Iterator<K, V>;
    using const_iterator = Iterator<K, V, true>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  private:
    /// @brief Threshold at which insertion triggers an automatic rehash.
    static constexpr double MAX_LOAD_FACTOR = 0.75;

    // Core storage.
    NodeType** buckets_;

    // Hash table state.
    std::size_t bucketCount_;
    std::size_t elementCount_;

    // Hash function object.
    Hash hasher_;

    /// @brief Computes the bucket index for `key` via `hasher_(key) & (bucketCount_ - 1)`.
    [[nodiscard]] std::size_t bucketIndex(const K& key) const noexcept;

    /**
     * @brief Locates the node storing `key`, if any.
     * @param key Key to search for.
     * @param indexOut If non-null, receives `key`'s bucket index regardless
     * of whether the node was found — lets callers that need the index
     * (insert, operator[], find) avoid hashing `key` a second time.
     * @return Pointer to the matching node, or `nullptr` if not present.
     */
    [[nodiscard]] Node<K, V>* findNode(const K& key, std::size_t* indexOut = nullptr);
    /// @brief Const-qualified overload. See the non-const findNode().
    [[nodiscard]] const Node<K, V>* findNode(const K& key, std::size_t* indexOut = nullptr) const;

    /**
     * @brief Rebuilds the bucket array at a larger size, relinking existing
     * nodes without allocating or copying any element.
     * @param newBucketCount New bucket count. No-op if not greater than the current count.
     */
    void rehash(std::size_t newBucketCount);

    /**
     * @brief Allocates a minimal single-bucket array if this map currently
     * has none (e.g. right after being moved from).
     * @details Lets a moved-from map be safely reused instead of only being
     * assignable or destructible.
     */
    void ensureStorage();

    /**
     * @brief Rounds `requested` up to the next power of two.
     * @param requested Minimum bucket count needed.
     * @return The smallest power of two that is `>= requested`, at least 1.
     */
    [[nodiscard]] static std::size_t normalizeBucketCount(std::size_t requested) noexcept;

    /**
     * @brief Shared implementation for the copy constructor and copy assignment.
     * @param other Map to clone elements from.
     * @details Clones `other`'s elements directly rather than going through
     * insert(), since `other`'s keys are already known to be unique. Turns
     * what would otherwise be an O(n * average chain length) copy into a
     * straight O(n) copy. `hasher_` must already be assigned by the caller
     * before this runs.
     */
    void cloneFrom(const HashMap& other);

    /// @brief Destroys every node and releases the bucket array, leaving an empty state.
    void release() noexcept;

  public:
    /**
     * @brief Constructs an empty map with at least `bucketCount` buckets.
     * @param bucketCount Requested initial bucket count, rounded up to the
     * next power of two (minimum 1). Defaults to 16.
     */
    explicit HashMap(std::size_t bucketCount = 16);

    /// @brief Destroys all elements and releases storage.
    ~HashMap();

    /**
     * @brief Copy-constructs a map, deep-copying `other`'s elements.
     * @param other Map to copy from.
     */
    HashMap(const HashMap& other);

    /**
     * @brief Copy-assigns from `other`, replacing this map's contents.
     * @param other Map to copy from.
     * @return Reference to `*this`.
     */
    HashMap& operator=(const HashMap& other);

    /**
     * @brief Move-constructs a map, taking ownership of `other`'s storage.
     * @param other Map to move from. Left in a valid, reusable empty state.
     */
    HashMap(HashMap&& other) noexcept;

    /**
     * @brief Move-assigns from `other`, replacing this map's contents.
     * @param other Map to move from. Left in a valid, reusable empty state.
     * @return Reference to `*this`.
     */
    HashMap& operator=(HashMap&& other) noexcept;

    /**
     * @brief Returns a reference to the value mapped to `key`, inserting a
     * default-constructed value if `key` is not already present.
     * @param key Key to look up or insert.
     * @return Reference to the (possibly newly inserted) mapped value.
     */
    [[nodiscard]] V& operator[](const K& key);

    /**
     * @brief Returns a reference to the value mapped to `key`, with bounds checking.
     * @param key Key to look up.
     * @return Reference to the mapped value.
     * @throws std::out_of_range if `key` is not present.
     */
    [[nodiscard]] V& at(const K& key);

    /**
     * @brief Returns a const reference to the value mapped to `key`, with bounds checking.
     * @param key Key to look up.
     * @return Const reference to the mapped value.
     * @throws std::out_of_range if `key` is not present.
     */
    [[nodiscard]] const V& at(const K& key) const;

    /**
     * @brief Inserts `key`/`value` if `key` is not already present. No-op otherwise.
     * @param key Key to insert.
     * @param value Value to associate with `key`.
     */
    void insert(const K& key, const V& value);

    /**
     * @brief Replaces the value mapped to `key`, if present.
     * @param key Key to look up.
     * @param value New value to store.
     * @return `true` if `key` was found and updated, `false` otherwise.
     */
    [[nodiscard]] bool update(const K& key, const V& value);

    /**
     * @brief Removes the element mapped to `key`, if present.
     * @param key Key to remove.
     * @return `true` if `key` was found and removed, `false` otherwise.
     */
    [[nodiscard]] bool erase(const K& key);

    /// @brief Removes all elements. Bucket count is unchanged.
    void clear();

    /**
     * @brief Ensures the map can hold at least `desiredCapacity` elements
     * without triggering a rehash. Useful before a bulk insertion loop.
     * @param desiredCapacity Minimum number of elements to accommodate.
     */
    void reserve(std::size_t desiredCapacity);

    /**
     * @brief Finds the element mapped to `key`.
     * @param key Key to search for.
     * @return Iterator to the element, or end() if not found.
     */
    [[nodiscard]] iterator find(const K& key);

    /**
     * @brief Finds the element mapped to `key`.
     * @param key Key to search for.
     * @return Const iterator to the element, or end() if not found.
     */
    [[nodiscard]] const_iterator find(const K& key) const;

    /**
     * @brief Checks whether `key` is present in the map.
     * @param key Key to search for.
     * @return `true` if `key` is present.
     */
    [[nodiscard]] bool contains(const K& key) const noexcept;

    /// @brief Returns the current number of buckets.
    [[nodiscard]] std::size_t capacity() const noexcept;
    /// @brief Returns the number of elements currently stored.
    [[nodiscard]] std::size_t size() const noexcept;
    /// @brief Returns `size() / capacity()`, or `0.0` if there are no buckets.
    [[nodiscard]] double load_factor() const noexcept;
    /// @brief Returns the load factor threshold that triggers an automatic rehash.
    [[nodiscard]] static constexpr double max_load_factor() noexcept;

    /// @brief Returns whether the map has no elements.
    [[nodiscard]] bool empty() const noexcept;

    /// @brief Returns an iterator to the first element.
    [[nodiscard]] iterator begin() noexcept;
    /// @brief Returns an iterator one past the last element.
    [[nodiscard]] iterator end() noexcept;
    /// @brief Returns a const iterator to the first element.
    [[nodiscard]] const_iterator begin() const noexcept;
    /// @brief Returns a const iterator one past the last element.
    [[nodiscard]] const_iterator end() const noexcept;
    /// @brief Returns a const iterator to the first element.
    [[nodiscard]] const_iterator cbegin() const noexcept;
    /// @brief Returns a const iterator one past the last element.
    [[nodiscard]] const_iterator cend() const noexcept;
    /// @brief Returns a reverse iterator to the last element.
    [[nodiscard]] reverse_iterator rbegin() noexcept;
    /// @brief Returns a reverse iterator one before the first element.
    [[nodiscard]] reverse_iterator rend() noexcept;
    /// @brief Returns a const reverse iterator to the last element.
    [[nodiscard]] const_reverse_iterator rbegin() const noexcept;
    /// @brief Returns a const reverse iterator one before the first element.
    [[nodiscard]] const_reverse_iterator rend() const noexcept;
    /// @brief Returns a const reverse iterator to the last element.
    [[nodiscard]] const_reverse_iterator crbegin() const noexcept;
    /// @brief Returns a const reverse iterator one before the first element.
    [[nodiscard]] const_reverse_iterator crend() const noexcept;
};

} // namespace HashMapPro

/// @brief Short alias so this library can be used as `rain::HashMap`, while
/// its true namespace (and all internal diagnostics) remains `HashMapPro`.
/// See Node.h and Iterator.h for the same alias applied to `rain::Node` and
/// `rain::Iterator`.
namespace rain = HashMapPro;

#include "HashMap.tpp"
