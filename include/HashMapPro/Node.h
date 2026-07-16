/**
 * @file Node.h
 * @brief Internal node structure used by HashMap.
 *
 * Contains the node implementation used internally by the HashMap
 * container for key-value storage and bucket chaining.
 */

#pragma once

namespace HashMapPro {

/**
 * @brief A singly-linked node stored within a hash bucket chain.
 * @tparam K Key type.
 * @tparam V Value type.
 * @details HashMap owns every Node it allocates and is solely responsible
 * for destroying it; Node itself adds no behavior beyond what K and V
 * already provide.
 */
template <typename K, typename V> struct Node {
    K key;   ///< The stored key.
    V value; ///< The stored value associated with `key`.

    /// @brief Next node in the same bucket chain, or `nullptr` if this is the last.
    Node* next = nullptr;

    /**
     * @brief Constructs a node from a key-value pair.
     * @param key Key to copy into the node.
     * @param value Value to copy into the node.
     */
    Node(const K& key, const V& value) : key{key}, value{value} {}
};

} // namespace HashMapPro

/// @brief Short alias so this library can be used as `rain::Node`, while its
/// true namespace (and all internal diagnostics) remains `HashMapPro`. See
/// HashMap.h for the same alias applied to `rain::HashMap`.
namespace rain = HashMapPro;
