/**
 * @file HashMap.tpp
 * @brief HashMap template implementation.
 *
 * Contains the implementation of HashMap template member
 * functions and internal implementation details.
 */

// ============================================================
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

// clang-format off
#include <cstddef>   // std::size_t
#include <new>       // std::bad_alloc (implicit, via operator new)
#include <stdexcept> // std::out_of_range
#include <utility>   // std::move, std::exchange
// clang-format on

namespace HashMapPro {

// ============================================================
//  Section 1 — Constructors & Destructor
// ============================================================

template <typename K, typename V, typename Hash>
HashMap<K, V, Hash>::HashMap(std::size_t bucketCount)
    : buckets_{nullptr} // A hash table always maintains at least one bucket, and the count
                        // is always a power of two so lookups can mask instead of dividing.
      ,
      bucketCount_{normalizeBucketCount(bucketCount == 0 ? 1 : bucketCount)}, elementCount_{0},
      hasher_{Hash()} {
    buckets_ = new Node<K, V>*[bucketCount_];

    for (std::size_t i = 0; i < bucketCount_; ++i) {
        buckets_[i] = nullptr;
    }
}

template <typename K, typename V, typename Hash> HashMap<K, V, Hash>::~HashMap() {
    release();
}

// ============================================================
//  Section 2 — Copy & Move Semantics
// ============================================================

template <typename K, typename V, typename Hash>
HashMap<K, V, Hash>::HashMap(const HashMap<K, V, Hash>& other)
    : buckets_{nullptr}, bucketCount_{0}, elementCount_{0}, hasher_{other.hasher_} {
    cloneFrom(other);
}

template <typename K, typename V, typename Hash>
HashMap<K, V, Hash>& HashMap<K, V, Hash>::operator=(const HashMap<K, V, Hash>& other) {
    if (this != &other) {
        release();
        hasher_ = other.hasher_;
        cloneFrom(other);
    }

    return *this;
}

template <typename K, typename V, typename Hash>
HashMap<K, V, Hash>::HashMap(HashMap<K, V, Hash>&& other) noexcept
    : buckets_{std::exchange(other.buckets_, nullptr)},
      bucketCount_{std::exchange(other.bucketCount_, 0)},
      elementCount_{std::exchange(other.elementCount_, 0)}, hasher_(std::move(other.hasher_)) {}

template <typename K, typename V, typename Hash>
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

template <typename K, typename V, typename Hash> V& HashMap<K, V, Hash>::operator[](const K& key) {
    ensureStorage();

    std::size_t index;
    Node<K, V>* node = findNode(key, &index);

    if (node == nullptr) {
        // Insert directly at the already-known index instead of
        // re-searching for the key a second time.
        node = new Node<K, V>(key, V{});
        node->next = buckets_[index];
        buckets_[index] = node;

        ++elementCount_;

        // Rehashing relinks existing nodes but never reallocates or moves
        // them, so `node` (and the reference we return below) stays valid
        // even if a rehash happens here.
        if (static_cast<double>(elementCount_) >
            static_cast<double>(bucketCount_) * MAX_LOAD_FACTOR) {
            rehash(bucketCount_ * 2);
        }
    }

    return node->value;
}

template <typename K, typename V, typename Hash> V& HashMap<K, V, Hash>::at(const K& key) {
    Node<K, V>* node = findNode(key);

    if (node == nullptr)
        throw std::out_of_range("HashMap::at: key not found");

    return node->value;
}

template <typename K, typename V, typename Hash>
const V& HashMap<K, V, Hash>::at(const K& key) const {
    const Node<K, V>* node = findNode(key);

    if (node == nullptr)
        throw std::out_of_range("HashMap::at: key not found");

    return node->value;
}

// ============================================================
//  Section 4 — Modifiers
// ============================================================

template <typename K, typename V, typename Hash>
void HashMap<K, V, Hash>::insert(const K& key, const V& value) {
    ensureStorage();

    std::size_t index;
    if (findNode(key, &index) != nullptr)
        return;

    Node<K, V>* node = new Node<K, V>(key, value);

    // Inserts at the front of the bucket chain. Since lookups traverse the
    // entire chain anyway, prepending avoids walking to the tail and keeps
    // insertion O(1) once the bucket is known.
    node->next = buckets_[index];
    buckets_[index] = node;

    ++elementCount_;

    if (static_cast<double>(elementCount_) > static_cast<double>(bucketCount_) * MAX_LOAD_FACTOR) {
        rehash(bucketCount_ * 2);
    }
}

template <typename K, typename V, typename Hash>
bool HashMap<K, V, Hash>::update(const K& key, const V& value) {
    Node<K, V>* node = findNode(key);

    if (node == nullptr)
        return false;

    node->value = value;
    return true;
}

template <typename K, typename V, typename Hash> bool HashMap<K, V, Hash>::erase(const K& key) {
    if (buckets_ == nullptr)
        return false;

    std::size_t index = bucketIndex(key);

    Node<K, V>* current = buckets_[index];
    Node<K, V>* prev = nullptr;

    while (current) {
        if (current->key == key) {
            if (prev == nullptr) {
                buckets_[index] = current->next;
            } else {
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

template <typename K, typename V, typename Hash> void HashMap<K, V, Hash>::clear() {
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

template <typename K, typename V, typename Hash>
void HashMap<K, V, Hash>::reserve(std::size_t desiredCapacity) {
    if (desiredCapacity == 0)
        return;

    ensureStorage();

    // Smallest bucket count such that desiredCapacity / bucketCount does
    // not exceed the max load factor.
    std::size_t needed =
        static_cast<std::size_t>(static_cast<double>(desiredCapacity) / MAX_LOAD_FACTOR) + 1;

    std::size_t newBucketCount = normalizeBucketCount(needed);

    if (newBucketCount > bucketCount_) {
        rehash(newBucketCount);
    }
}

// ============================================================
//  Section 5 — Lookup
// ============================================================

template <typename K, typename V, typename Hash>
typename HashMap<K, V, Hash>::iterator HashMap<K, V, Hash>::find(const K& key) {
    std::size_t index;
    Node<K, V>* node = findNode(key, &index);

    if (node == nullptr) {
        return end();
    }

    return iterator(node, buckets_, index, bucketCount_);
}

template <typename K, typename V, typename Hash>
typename HashMap<K, V, Hash>::const_iterator HashMap<K, V, Hash>::find(const K& key) const {
    std::size_t index;
    const Node<K, V>* node = findNode(key, &index);

    if (node == nullptr) {
        return end();
    }

    return const_iterator(node, buckets_, index, bucketCount_);
}

template <typename K, typename V, typename Hash>
bool HashMap<K, V, Hash>::contains(const K& key) const noexcept {
    return findNode(key) != nullptr;
}

// ============================================================
//  Section 6 — Capacity
// ============================================================

template <typename K, typename V, typename Hash>
std::size_t HashMap<K, V, Hash>::capacity() const noexcept {
    return bucketCount_;
}

template <typename K, typename V, typename Hash>
std::size_t HashMap<K, V, Hash>::size() const noexcept {
    return elementCount_;
}

template <typename K, typename V, typename Hash>
double HashMap<K, V, Hash>::load_factor() const noexcept {
    if (bucketCount_ == 0)
        return 0.0;
    return static_cast<double>(elementCount_) / static_cast<double>(bucketCount_);
}

template <typename K, typename V, typename Hash>
constexpr double HashMap<K, V, Hash>::max_load_factor() noexcept {
    return MAX_LOAD_FACTOR;
}

// ============================================================
//  Section 7 — State
// ============================================================

template <typename K, typename V, typename Hash> bool HashMap<K, V, Hash>::empty() const noexcept {
    return elementCount_ == 0;
}

// ============================================================
//  Section 8 — Iterator Access
// ============================================================

// Finds the first non-empty bucket to establish the start of iteration.
template <typename K, typename V, typename Hash>
typename HashMap<K, V, Hash>::iterator HashMap<K, V, Hash>::begin() noexcept {
    for (std::size_t i = 0; i < bucketCount_; ++i) {
        if (buckets_[i] != nullptr) {
            return iterator(buckets_[i], buckets_, i, bucketCount_);
        }
    }

    return end();
}

template <typename K, typename V, typename Hash>
typename HashMap<K, V, Hash>::iterator HashMap<K, V, Hash>::end() noexcept {
    return iterator(nullptr, buckets_, 0, bucketCount_);
}

// Finds the first non-empty bucket to establish the start of iteration.
template <typename K, typename V, typename Hash>
typename HashMap<K, V, Hash>::const_iterator HashMap<K, V, Hash>::begin() const noexcept {
    for (std::size_t i = 0; i < bucketCount_; ++i) {
        if (buckets_[i] != nullptr) {
            return const_iterator(buckets_[i], buckets_, i, bucketCount_);
        }
    }

    return end();
}

template <typename K, typename V, typename Hash>
typename HashMap<K, V, Hash>::const_iterator HashMap<K, V, Hash>::end() const noexcept {
    return const_iterator(nullptr, buckets_, 0, bucketCount_);
}

template <typename K, typename V, typename Hash>
typename HashMap<K, V, Hash>::const_iterator HashMap<K, V, Hash>::cbegin() const noexcept {
    return begin();
}

template <typename K, typename V, typename Hash>
typename HashMap<K, V, Hash>::const_iterator HashMap<K, V, Hash>::cend() const noexcept {
    return end();
}

template <typename K, typename V, typename Hash>
typename HashMap<K, V, Hash>::reverse_iterator HashMap<K, V, Hash>::rbegin() noexcept {
    return reverse_iterator(end());
}

template <typename K, typename V, typename Hash>
typename HashMap<K, V, Hash>::reverse_iterator HashMap<K, V, Hash>::rend() noexcept {
    return reverse_iterator(begin());
}

template <typename K, typename V, typename Hash>
typename HashMap<K, V, Hash>::const_reverse_iterator HashMap<K, V, Hash>::rbegin() const noexcept {
    return const_reverse_iterator(cend());
}

template <typename K, typename V, typename Hash>
typename HashMap<K, V, Hash>::const_reverse_iterator HashMap<K, V, Hash>::rend() const noexcept {
    return const_reverse_iterator(cbegin());
}

template <typename K, typename V, typename Hash>
typename HashMap<K, V, Hash>::const_reverse_iterator HashMap<K, V, Hash>::crbegin() const noexcept {
    return const_reverse_iterator(cend());
}

template <typename K, typename V, typename Hash>
typename HashMap<K, V, Hash>::const_reverse_iterator HashMap<K, V, Hash>::crend() const noexcept {
    return const_reverse_iterator(cbegin());
}

// ============================================================
//  Section 9 — Private Helpers
// ============================================================

template <typename K, typename V, typename Hash>
std::size_t HashMap<K, V, Hash>::bucketIndex(const K& key) const noexcept {
    // bucketCount_ is always a power of two (see normalizeBucketCount), so
    // masking replaces a division/modulo on the hot path.
    return hasher_(key) & (bucketCount_ - 1);
}

template <typename K, typename V, typename Hash>
Node<K, V>* HashMap<K, V, Hash>::findNode(const K& key, std::size_t* indexOut) {
    if (buckets_ == nullptr) {
        if (indexOut)
            *indexOut = 0;
        return nullptr;
    }

    std::size_t index = bucketIndex(key);
    if (indexOut)
        *indexOut = index;

    Node<K, V>* current = buckets_[index];

    while (current) {
        if (current->key == key)
            return current;

        current = current->next;
    }

    return nullptr;
}

template <typename K, typename V, typename Hash>
const Node<K, V>* HashMap<K, V, Hash>::findNode(const K& key, std::size_t* indexOut) const {
    if (buckets_ == nullptr) {
        if (indexOut)
            *indexOut = 0;
        return nullptr;
    }

    std::size_t index = bucketIndex(key);
    if (indexOut)
        *indexOut = index;

    Node<K, V>* current = buckets_[index];

    while (current) {
        if (current->key == key)
            return current;

        current = current->next;
    }

    return nullptr;
}

// Rebuilds the bucket array by relinking existing nodes into their new
// buckets_ without allocating or copying elements.
template <typename K, typename V, typename Hash>
void HashMap<K, V, Hash>::rehash(std::size_t newBucketCount) {
    if (newBucketCount <= bucketCount_)
        return;

    Node<K, V>** newBuckets = new Node<K, V>*[newBucketCount];

    for (std::size_t i = 0; i < newBucketCount; ++i) {
        newBuckets[i] = nullptr;
    }

    std::size_t mask = newBucketCount - 1;

    for (std::size_t i = 0; i < bucketCount_; ++i) {
        Node<K, V>* current = buckets_[i];

        while (current) {
            Node<K, V>* next = current->next;

            std::size_t index = hasher_(current->key) & mask;

            current->next = newBuckets[index];
            newBuckets[index] = current;

            current = next;
        }
    }

    delete[] buckets_;

    buckets_ = newBuckets;
    bucketCount_ = newBucketCount;
}

template <typename K, typename V, typename Hash> void HashMap<K, V, Hash>::ensureStorage() {
    if (buckets_ == nullptr) {
        bucketCount_ = 1;
        buckets_ = new Node<K, V>*[1];
        buckets_[0] = nullptr;
    }
}

template <typename K, typename V, typename Hash>
std::size_t HashMap<K, V, Hash>::normalizeBucketCount(std::size_t requested) noexcept {
    std::size_t count = 1;

    while (count < requested) {
        count <<= 1;
    }

    return count;
}

// Clones every element of `other` directly, without the per-element
// existence check that insert() would normally perform. This is safe
// because `other`'s keys are already guaranteed unique, and it turns what
// would otherwise be an O(n * average chain length) copy into a straight
// O(n) copy. `hasher_` must already be assigned/initialized by the caller
// before this runs.
template <typename K, typename V, typename Hash>
void HashMap<K, V, Hash>::cloneFrom(const HashMap<K, V, Hash>& other) {
    bucketCount_ = other.bucketCount_;
    elementCount_ = 0;
    buckets_ = new Node<K, V>*[bucketCount_];

    for (std::size_t i = 0; i < bucketCount_; ++i) {
        buckets_[i] = nullptr;
    }

    try {
        for (std::size_t i = 0; i < bucketCount_; ++i) {
            Node<K, V>* current = other.buckets_[i];

            while (current) {
                Node<K, V>* node = new Node<K, V>(current->key, current->value);

                node->next = buckets_[i];
                buckets_[i] = node;

                ++elementCount_;

                current = current->next;
            }
        }
    } catch (...) {
        // Prevent leaks if copying any node throws.
        release();
        throw;
    }
}

// Destroys every node and releases the bucket array, leaving the hash map
// in an empty state.
template <typename K, typename V, typename Hash> void HashMap<K, V, Hash>::release() noexcept {
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
