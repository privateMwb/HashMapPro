#pragma once

namespace HashMapPro {

	// Node stored within a bucket chain.
	template<typename K, typename V>
	struct Node {
		K key;
		V value;

		// Next node in the same bucket chain.
		Node* next = nullptr;

		// Constructs a node from a key-value pair.
		Node(const K& key, const V& value)
			: key{ key }
			, value{ value }
		{
		}
	};

} // namespace HashMapPro