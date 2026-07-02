#pragma once

#include <cstddef>
#include <type_traits>
#include <iterator>

#include "Node.h"

namespace HashMapPro {

	// Bidirectional iterator over the hash table.
	//
	// Traverses all nodes across buckets while preserving
	// the order within each bucket chain.
	template<
		typename K,
		typename V,
		bool IsConst = false
	>
	class Iterator {
	private:

		// Internal node type.
		using NodeType = Node<K, V>;

	public:

		// Standard iterator traits.
		using value_type = NodeType;
		using difference_type = std::ptrdiff_t;

		using pointer = std::conditional_t<
			IsConst,
			const NodeType*,
			NodeType*
		>;

		using reference = std::conditional_t<
			IsConst,
			const NodeType&,
			NodeType&
		>;

		using iterator_category = std::bidirectional_iterator_tag;

		using BucketPointer = std::conditional_t<
			IsConst,
			NodeType* const*,
			NodeType**
		>;

	private:

		// Iterator state.
		pointer current = nullptr;
		BucketPointer buckets = nullptr;
		std::size_t bucketIndex = 0;
		std::size_t bucketCount = 0;

	public:

		// Constructors.
		Iterator() = default;

		// Constructs an iterator from its internal traversal state.
		Iterator(pointer node,
			BucketPointer buckets,
			std::size_t index,
			std::size_t bucketCount)
			: current{ node }
			, buckets{ buckets }
			, bucketIndex{ index }
			, bucketCount{ bucketCount }
		{
		}

		[[nodiscard]] reference operator*() const {
			return *current;
		}

		[[nodiscard]] pointer operator->() const {
			return current;
		}

		// Advances to the next node, skipping empty buckets.
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

		Iterator operator++(int) {
			Iterator temp = *this;
			++(*this);

			return temp;
		}

		// Moves to the previous node across bucket chains.
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

		Iterator operator--(int) {
			Iterator temp = *this;
			--(*this);

			return temp;
		}

		[[nodiscard]] bool operator==(const Iterator& other) const {
			return current == other.current;
		}

		[[nodiscard]] bool operator!=(const Iterator& other) const {
			return !(*this == other);
		}

		// Implicit conversion from iterator to const_iterator.
		operator Iterator<K, V, true>() const {
			return Iterator<K, V, true>(
				current,
				buckets,
				bucketIndex,
				bucketCount
			);
		}
	};

} // namespace HashMapPro