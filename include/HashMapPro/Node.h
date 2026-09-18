/**
 * @file            Node.h
 *
 * @date            2026-16-7
 *
 * @version         1.0.0
 *
 * @copyright       Copyright (c) 2026 privateMwb
 *                  All rights reserved.
 *                  https://github.com/privateMwb/HashMapPro
 *
 * @attention       This source is released under the MIT license
 *                  SPDX-License-Identifier: MIT
 *                  <http://opensource.org/licenses/MIT>
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
