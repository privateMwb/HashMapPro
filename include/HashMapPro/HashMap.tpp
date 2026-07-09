// ============================================================
// HashMap.tpp
// Template implementation for HashMapPro::HashMap.
// ============================================================
//
//  Sections:
//   1. Constructors & Destructor
//   2. Copy & Move Semantics
//   3. Element Access
//   4. Modifiers
//   5. Lookup
//   6. Capacity
//   7. State
//   8. Iterator Access
//   9. Private Helpers
//
// ============================================================

#include <cstddef>
#include <stdexcept>
#include <new>

namespace HashMapPro {


	// ============================================================
	//  Section 1 — Constructors & Destructor
	// ============================================================

	template<typename K,
		typename V,
		typename Hash>
	HashMap<K, V, Hash>::HashMap(std::size_t bucketCount)
		: buckets_{ nullptr }
		// A hash table always maintains at least one bucket.
		, bucketCount_{ bucketCount == 0 ? 1 : bucketCount }
		, elementCount_{ 0 }
		, hasher_{ Hash() } {
		buckets_ = new Node<K, V>* [bucketCount_];

		for (std::size_t i = 0; i < bucketCount_; ++i) {
			buckets_[i] = nullptr;
		}
	}

	template<typename K,
		typename V,
		typename Hash>
	HashMap<K, V, Hash>::~HashMap() {
		release();
	}


	// ============================================================
	//  Section 2 — Copy & Move Semantics
	// ============================================================

	template<typename K,
		typename V,
		typename Hash>
	HashMap<K, V, Hash>::HashMap(const HashMap<K, V, Hash>& other)
		: buckets_{ new Node<K, V>*[other.bucketCount_] }
		, bucketCount_{ other.bucketCount_ }
		, elementCount_{ 0 }
		, hasher_{ other.hasher_ }
	{
		for (std::size_t i = 0; i < other.bucketCount_; ++i) {
			buckets_[i] = nullptr;
		}

		try {
			for (std::size_t i = 0; i < other.bucketCount_; ++i) {
				Node<K, V>* current = other.buckets_[i];

				while (current) {
					insert(current->key, current->value);

					current = current->next;
				}
			}
		}
		catch (...) {
			// Prevent leaks if copying any node throws.
			release();
			throw;
		}
	}

	template<typename K,
		typename V,
		typename Hash>
	HashMap<K, V, Hash>& HashMap<K, V, Hash>::operator=(const HashMap<K, V, Hash>& other) {
		if (this != &other) {
			release();

			bucketCount_ = other.bucketCount_;
			elementCount_ = 0;
			hasher_ = other.hasher_;
			buckets_ = new Node<K, V>* [bucketCount_];

			for (std::size_t i = 0; i < other.bucketCount_; ++i) {
				buckets_[i] = nullptr;
			}

			try {
				for (std::size_t i = 0; i < other.bucketCount_; ++i) {
					Node<K, V>* current = other.buckets_[i];

					while (current) {
						insert(current->key, current->value);

						current = current->next;
					}
				}
			}
			catch (...) {
				// Prevent leaks if copying any node throws.
				release();
				throw;
			}
		}

		return *this;
	}

	template<typename K,
		typename V,
		typename Hash>
	HashMap<K, V, Hash>::HashMap(HashMap<K, V, Hash>&& other) noexcept
		: buckets_{ std::exchange(other.buckets_, nullptr) }
		, bucketCount_{ std::exchange(other.bucketCount_, 0) }
		, elementCount_{ std::exchange(other.elementCount_, 0) }
		, hasher_(std::move(other.hasher_)) {
	}

	template<typename K,
		typename V,
		typename Hash>
	HashMap<K, V, Hash>& HashMap<K, V, Hash>::operator=(HashMap<K, V, Hash>&& other) noexcept {
		if (this != &other) {
			release();

			buckets_ = std::exchange(other.buckets_, nullptr);
			bucketCount_ = std::exchange(other.bucketCount_, 0);
			elementCount_ = std::exchange(other.elementCount_, 0);
			hasher_ = std::move(other.hasher_);
		}

		return *this;
	}


	// ============================================================
	//  Section 3 — Element Access
	// ============================================================

	template<typename K,
		typename V,
		typename Hash>
	V& HashMap<K, V, Hash>::operator[](const K& key) {
		Node<K, V>* node = findNode(key);

		if (node == nullptr) {
			insert(key, V{});
			node = findNode(key);
		}

		return node->value;
	}

	template<typename K,
		typename V,
		typename Hash>
	V& HashMap<K, V, Hash>::at(const K& key) {
		Node<K, V>* node = findNode(key);

		if (node == nullptr)
			throw std::out_of_range("HashMap::at: key not found");

		return node->value;
	}

	template<typename K,
		typename V,
		typename Hash>
	const V& HashMap<K, V, Hash>::at(const K& key) const {
		const Node<K, V>* node = findNode(key);

		if (node == nullptr)
			throw std::out_of_range("HashMap::at: key not found");

		return node->value;
	}


	// ============================================================
	//  Section 4 — Modifiers
	// ============================================================

	template<typename K,
		typename V,
		typename Hash>
	void HashMap<K, V, Hash>::insert(const K& key, const V& value) {
		if (findNode(key) != nullptr) return;

		std::size_t index = bucketIndex(key);

		Node<K, V>* node = new Node<K, V>(key, value);

		// Inserts at the front of the bucket chain. Since lookups traverse the
		// entire chain anyway, prepending avoids walking to the tail and keeps
		// insertion O(1) once the bucket is known.
		node->next = buckets_[index];
		buckets_[index] = node;

		++elementCount_;

		if (static_cast<double>(elementCount_) / bucketCount_ > MAX_LOAD_FACTOR) {
			rehash(bucketCount_ * 2);
		}
	}

	template<typename K,
		typename V,
		typename Hash>
	bool HashMap<K, V, Hash>::update(const K& key, const V& value) {
		Node<K, V>* node = findNode(key);

		if (node == nullptr) return false;

		node->value = value;
		return true;
	}

	template<typename K,
		typename V,
		typename Hash>
	bool HashMap<K, V, Hash>::erase(const K& key) {
		std::size_t index = bucketIndex(key);

		Node<K, V>* current = buckets_[index];
		Node<K, V>* prev = nullptr;

		while (current) {
			if (current->key == key) {
				if (prev == nullptr) {
					buckets_[index] = current->next;
				}
				else {
					prev->next = current->next;
				}

				delete current;
				--elementCount_;

				return true;
			}

			prev = current;
			current = current->next;
		}

		return false;
	}

	template<typename K,
		typename V,
		typename Hash>
	void HashMap<K, V, Hash>::clear() {
		for (std::size_t i = 0; i < bucketCount_; ++i) {
			Node<K, V>* current = buckets_[i];

			while (current) {
				Node<K, V>* next = current->next;

				delete current;

				current = next;
			}

			buckets_[i] = nullptr;
		}

		elementCount_ = 0;
	}


	// ============================================================
	//  Section 5 — Lookup
	// ============================================================

	template<typename K,
		typename V,
		typename Hash>
	typename HashMap<K, V, Hash>::iterator HashMap<K, V, Hash>::find(const K& key) {
		Node<K, V>* node = findNode(key);

		if (node == nullptr) {
			return end();
		}

		return iterator(
			node,
			buckets_,
			bucketIndex(key),
			bucketCount_
		);
	}

	template<typename K,
		typename V,
		typename Hash>
	typename HashMap<K, V, Hash>::const_iterator HashMap<K, V, Hash>::find(const K& key) const {
		const Node<K, V>* node = findNode(key);

		if (node == nullptr) {
			return end();
		}

		return const_iterator(
			node,
			buckets_,
			bucketIndex(key),
			bucketCount_
		);
	}

	template<typename K,
		typename V,
		typename Hash>
	bool HashMap<K, V, Hash>::contains(const K& key) const noexcept {
		if (bucketCount_ == 0)
        return false;
    return findNode(key) != nullptr;
	}


	// ============================================================
	//  Section 6 — Capacity
	// ============================================================

	template<typename K,
		typename V,
		typename Hash>
	std::size_t HashMap<K, V, Hash>::capacity() const noexcept {
		return bucketCount_;
	}

	template<typename K,
		typename V,
		typename Hash>
	std::size_t HashMap<K, V, Hash>::size() const noexcept {
		return elementCount_;
	}


	// ============================================================
	//  Section 7 — State
	// ============================================================

	template<typename K,
		typename V,
		typename Hash>
	bool HashMap<K, V, Hash>::empty() const noexcept {
		return elementCount_ == 0;
	}


	// ============================================================
	//  Section 8 — Iterator Access
	// ============================================================

	// Finds the first non-empty bucket to establish the start of iteration.
	template<typename K,
		typename V,
		typename Hash>
	typename HashMap<K, V, Hash>::iterator HashMap<K, V, Hash>::begin() noexcept {
		for (std::size_t i = 0; i < bucketCount_; ++i) {
			if (buckets_[i] != nullptr) {
				return iterator(
					buckets_[i],
					buckets_,
					i,
					bucketCount_
				);
			}
		}

		return end();
	}

	template<typename K,
		typename V,
		typename Hash>
	typename HashMap<K, V, Hash>::iterator HashMap<K, V, Hash>::end() noexcept {
		return iterator(
			nullptr,
			buckets_,
			0,
			bucketCount_
		);
	}

	// Finds the first non-empty bucket to establish the start of iteration.
	template<typename K,
		typename V,
		typename Hash>
	typename HashMap<K, V, Hash>::const_iterator HashMap<K, V, Hash>::begin() const noexcept {
		for (std::size_t i = 0; i < bucketCount_; ++i) {
			if (buckets_[i] != nullptr) {
				return const_iterator(
					buckets_[i],
					buckets_,
					i,
					bucketCount_
				);
			}
		}

		return end();
	}

	template<typename K,
		typename V,
		typename Hash>
	typename HashMap<K, V, Hash>::const_iterator HashMap<K, V, Hash>::end() const noexcept {
		return const_iterator(
			nullptr,
			buckets_,
			0,
			bucketCount_
		);
	}

	template<typename K,
		typename V,
		typename Hash>
	typename HashMap<K, V, Hash>::const_iterator HashMap<K, V, Hash>::cbegin() const noexcept {
		return begin();
	}

	template<typename K,
		typename V,
		typename Hash>
	typename HashMap<K, V, Hash>::const_iterator HashMap<K, V, Hash>::cend() const noexcept {
		return end();
	}

	template<typename K,
		typename V,
		typename Hash>
	typename HashMap<K, V, Hash>::reverse_iterator HashMap<K, V, Hash>::rbegin() noexcept {
		return reverse_iterator(end());
	}

	template<typename K,
		typename V,
		typename Hash>
	typename HashMap<K, V, Hash>::reverse_iterator HashMap<K, V, Hash>::rend() noexcept {
		return reverse_iterator(begin());
	}

	template<typename K,
		typename V,
		typename Hash>
	typename HashMap<K, V, Hash>::const_reverse_iterator HashMap<K, V, Hash>::rbegin() const noexcept {
		return const_reverse_iterator(cend());
	}

	template<typename K,
		typename V,
		typename Hash>
	typename HashMap<K, V, Hash>::const_reverse_iterator HashMap<K, V, Hash>::rend() const noexcept {
		return const_reverse_iterator(cbegin());
	}

	template<typename K,
		typename V,
		typename Hash>
	typename HashMap<K, V, Hash>::const_reverse_iterator HashMap<K, V, Hash>::crbegin() const noexcept {
		return const_reverse_iterator(cend());
	}

	template<typename K,
		typename V,
		typename Hash>
	typename HashMap<K, V, Hash>::const_reverse_iterator HashMap<K, V, Hash>::crend() const noexcept {
		return const_reverse_iterator(cbegin());
	}


	// ============================================================
	//  Section 9 — Private Helpers
	// ============================================================

	template<typename K,
		typename V,
		typename Hash>
	std::size_t HashMap<K, V, Hash>::bucketIndex(const K& key) const {
		if (bucketCount_ == 0)
        return 0;
    return hasher_(key) % bucketCount_;
	}

	template<typename K,
		typename V,
		typename Hash>
	Node<K, V>* HashMap<K, V, Hash>::findNode(const K& key) {
		std::size_t index = bucketIndex(key);

		Node<K, V>* current = buckets_[index];

		while (current) {
			if (current->key == key) return current;

			current = current->next;
		}

		return nullptr;
	}

	// Rebuilds the bucket array by relinking existing nodes into
	// their new buckets_ without allocating or copying elements.
	template<typename K,
		typename V,
		typename Hash>
	void HashMap<K, V, Hash>::rehash(std::size_t newBucketCount) {
		if (newBucketCount <= bucketCount_) return;

		Node<K, V>** newBuckets = new Node<K, V>* [newBucketCount];

		for (std::size_t i = 0; i < newBucketCount; ++i) {
			newBuckets[i] = nullptr;
		}

		for (std::size_t i = 0; i < bucketCount_; ++i) {
			Node<K, V>* current = buckets_[i];

			while (current) {
				Node<K, V>* next = current->next;

				std::size_t index = hasher_(current->key) % newBucketCount;

				current->next = newBuckets[index];
				newBuckets[index] = current;

				current = next;
			}
		}

		delete[] buckets_;

		buckets_ = newBuckets;
		bucketCount_ = newBucketCount;
	}

	template<typename K,
		typename V,
		typename Hash>
	const Node<K, V>* HashMap<K, V, Hash>::findNode(const K& key) const {
		std::size_t index = bucketIndex(key);

		Node<K, V>* current = buckets_[index];

		while (current) {
			if (current->key == key) return current;

			current = current->next;
		}

		return nullptr;
	}

	// Destroys every node and releases the bucket array, leaving
	// the hash map in an empty state.
	template<typename K,
		typename V,
		typename Hash>
	void HashMap<K, V, Hash>::release() noexcept {
		if (buckets_ == nullptr) {
			return;
		}

		for (std::size_t i = 0; i < bucketCount_; ++i) {
			Node<K, V>* current = buckets_[i];

			while (current) {
				Node<K, V>* next = current->next;

				delete current;

				current = next;
			}
		}

		delete[] buckets_;

		buckets_ = nullptr;
		bucketCount_ = 0;
		elementCount_ = 0;
	}

} // namespace HashMapPro
