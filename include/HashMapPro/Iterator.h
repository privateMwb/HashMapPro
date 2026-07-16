/**
 * @file Iterator.h
 * @brief Iterator implementation for HashMap.
 *
 * Contains the iterator types used by HashMap for traversing
 * elements stored within the container.
 */

#pragma once

// clang-format off
#include <cstddef>     // std::size_t, std::ptrdiff_t
#include <iterator>    // std::bidirectional_iterator_tag
#include <type_traits> // std::conditional_t
// clang-format on

#include "Node.h"

namespace HashMapPro {

/**
 * @brief Bidirectional iterator over a HashMap's bucket-chained storage.
 * @tparam K Key type.
 * @tparam V Value type.
 * @tparam IsConst Whether this is a const_iterator. Defaults to `false`.
 * @details Traverses every node across all buckets in bucket order, and
 * within a bucket in chain order. Advancing past the last node in a bucket
 * moves to the first node of the next non-empty bucket; the reverse holds
 * for decrementing.
 */
template <typename K, typename V, bool IsConst = false> class Iterator {
  private:
    using NodeType = Node<K, V>;

  public:
    // Standard iterator traits.
    using value_type = NodeType;
    using difference_type = std::ptrdiff_t;
    using pointer = std::conditional_t<IsConst, const NodeType*, NodeType*>;
    using reference = std::conditional_t<IsConst, const NodeType&, NodeType&>;
    using iterator_category = std::bidirectional_iterator_tag;

    /// @brief Pointer-to-bucket-array type, const-qualified for const_iterator.
    using BucketPointer = std::conditional_t<IsConst, NodeType* const*, NodeType**>;

  private:
    pointer current = nullptr;       ///< Node the iterator currently refers to.
    BucketPointer buckets = nullptr; ///< The owning HashMap's bucket array.
    std::size_t bucketIndex = 0;     ///< Index of `current`'s bucket within `buckets`.
    std::size_t bucketCount = 0;     ///< Total number of buckets in `buckets`.

  public:
    /// @brief Constructs a singular (unusable) iterator.
    Iterator() = default;

    /**
     * @brief Constructs an iterator from its internal traversal state.
     * @param node Node the iterator should refer to, or `nullptr` for end().
     * @param buckets The owning HashMap's bucket array.
     * @param index Bucket index of `node` within `buckets`.
     * @param bucketCount Total number of buckets in `buckets`.
     */
    Iterator(pointer node, BucketPointer buckets, std::size_t index, std::size_t bucketCount)
        : current{node}, buckets{buckets}, bucketIndex{index}, bucketCount{bucketCount} {}

    /// @brief Dereferences to the referenced node. Undefined behavior at end().
    [[nodiscard]] reference operator*() const {
        return *current;
    }

    /// @brief Accesses a member of the referenced node. Undefined behavior at end().
    [[nodiscard]] pointer operator->() const {
        return current;
    }

    /**
     * @brief Advances to the next node, skipping empty buckets.
     * @return Reference to `*this`.
     */
    Iterator& operator++() {
        if (current && current->next) {
            current = current->next;
            return *this;
        }

        ++bucketIndex;

        while (bucketIndex < bucketCount) {
            if (buckets[bucketIndex] != nullptr) {
                current = buckets[bucketIndex];
                return *this;
            }

            ++bucketIndex;
        }

        current = nullptr;
        return *this;
    }

    /// @brief Post-increment. See operator++().
    Iterator operator++(int) {
        Iterator temp = *this;
        ++(*this);
        return temp;
    }

    /**
     * @brief Moves to the previous node across bucket chains.
     * @return Reference to `*this`.
     */
    Iterator& operator--() {
        if (current == nullptr) {
            for (std::size_t i = bucketCount; i > 0; --i) {
                NodeType* node = buckets[i - 1];

                if (!node)
                    continue;

                while (node->next) {
                    node = node->next;
                }

                current = node;
                bucketIndex = i - 1;
                return *this;
            }

            return *this;
        }

        NodeType* node = buckets[bucketIndex];

        if (node != current) {
            while (node->next != current) {
                node = node->next;
            }

            current = node;
            return *this;
        }

        for (std::size_t i = bucketIndex; i > 0; --i) {
            NodeType* prevBucket = buckets[i - 1];

            if (!prevBucket)
                continue;

            while (prevBucket->next) {
                prevBucket = prevBucket->next;
            }

            current = prevBucket;
            bucketIndex = i - 1;
            return *this;
        }

        current = nullptr;
        bucketIndex = 0;

        return *this;
    }

    /// @brief Post-decrement. See operator--().
    Iterator operator--(int) {
        Iterator temp = *this;
        --(*this);
        return temp;
    }

    /// @brief Compares two iterators by the node they refer to.
    [[nodiscard]] bool operator==(const Iterator& other) const {
        return current == other.current;
    }

    /// @brief Compares two iterators by the node they refer to.
    [[nodiscard]] bool operator!=(const Iterator& other) const {
        return !(*this == other);
    }

    /// @brief Implicit conversion from iterator to const_iterator.
    operator Iterator<K, V, true>() const {
        return Iterator<K, V, true>(current, buckets, bucketIndex, bucketCount);
    }
};

} // namespace HashMapPro

/// @brief Short alias so this library can be used as `rain::Iterator`, while
/// its true namespace remains `HashMapPro`. See HashMap.h for the same alias
/// applied to `rain::HashMap`.
namespace rain = HashMapPro;
