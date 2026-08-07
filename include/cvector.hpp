#ifndef CVECTOR_HPP
#define CVECTOR_HPP

#include "cvector.h"
#include <iterator>
#include <memory>
#include <stdexcept>
#include <algorithm>
#include <initializer_list>
#include <limits>
#include <type_traits>

/**
 * @namespace cvector
 * @brief C++ wrapper for the C vector library
 */
namespace cvector {

/**
 * @class Vector
 * @brief C++ template wrapper for CVector providing STL-like interface
 * 
 * Provides a more convenient and type-safe interface to the C vector,
 * with support for iterators, STL algorithms, and RAII memory management.
 */
template <typename T>
class Vector {
private:
    static_assert(std::is_trivially_copyable<T>::value,
                  "cvector::Vector requires a trivially copyable type");
    static_assert(std::is_trivially_destructible<T>::value,
                  "cvector::Vector requires a trivially destructible type");

    CVector *m_cvec;

    struct AdoptTag {};

    Vector(CVector *vec, AdoptTag) noexcept : m_cvec(vec) {}

    static void validate_cvector(const CVector *vec) {
        if (vec == nullptr) {
            throw std::invalid_argument("CVector pointer is null");
        }
        if (vec->element_size != sizeof(T)) {
            throw std::invalid_argument("CVector element size does not match T");
        }
        if (vec->capacity == 0 || vec->size > vec->capacity ||
            vec->data == nullptr) {
            throw std::invalid_argument("CVector has invalid storage metadata");
        }
        if (vec->capacity > std::numeric_limits<size_t>::max() /
                                vec->element_size) {
            throw std::invalid_argument("CVector allocation size overflows");
        }
    }

    void ensure_storage() {
        if (m_cvec == nullptr) {
            m_cvec = cvector_create(sizeof(T));
            if (m_cvec == nullptr) {
                throw std::bad_alloc();
            }
        }
    }

    // Helper function to get element at index without bounds checking
    T* get_at(size_t index) const {
        return static_cast<T*>(cvector_at(m_cvec, index));
    }

public:
    // Type definitions
    using value_type = T;
    using size_type = size_t;
    using difference_type = std::ptrdiff_t;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;

    class ConstIterator;

    /**
     * @brief Takes ownership of an existing CVector without copying.
     *
     * On success, the caller must not use or destroy vec. If validation throws,
     * ownership remains with the caller.
     */
    static Vector adopt(CVector *vec) {
        validate_cvector(vec);
        return Vector(vec, AdoptTag{});
    }

    /**
     * @brief Creates an independent copy of an existing CVector.
     *
     * The caller retains ownership of vec.
     */
    static Vector copy_from(const CVector *vec) {
        validate_cvector(vec);
        Vector copy(vec->capacity);
        for (size_t i = 0; i < vec->size; ++i) {
            const T *element = static_cast<const T*>(cvector_at(vec, i));
            copy.push_back(*element);
        }
        return copy;
    }

    /**
     * @class Iterator
     * @brief Random access iterator for Vector
     */
    class Iterator {
    private:
        T* m_ptr;

    public:
        // Iterator traits
        using iterator_category = std::random_access_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        // Constructors
        Iterator() : m_ptr(nullptr) {}
        explicit Iterator(T* ptr) : m_ptr(ptr) {}

        // Dereference operators
        reference operator*() const {
            if (m_ptr == nullptr) {
                throw std::runtime_error("Dereferencing invalid iterator");
            }
            return *m_ptr;
        }

        pointer operator->() const {
            if (m_ptr == nullptr) {
                throw std::runtime_error("Dereferencing invalid iterator");
            }
            return m_ptr;
        }

        reference operator[](difference_type n) const {
            return *(m_ptr + n);
        }

        // Increment/Decrement operators
        Iterator& operator++() {
            ++m_ptr;
            return *this;
        }

        Iterator operator++(int) {
            Iterator tmp = *this;
            ++m_ptr;
            return tmp;
        }

        Iterator& operator--() {
            --m_ptr;
            return *this;
        }

        Iterator operator--(int) {
            Iterator tmp = *this;
            --m_ptr;
            return tmp;
        }

        // Compound assignment operators
        Iterator& operator+=(difference_type n) {
            m_ptr += n;
            return *this;
        }

        Iterator& operator-=(difference_type n) {
            m_ptr -= n;
            return *this;
        }

        // Arithmetic operators
        Iterator operator+(difference_type n) const {
            return Iterator(m_ptr + n);
        }

        Iterator operator-(difference_type n) const {
            return Iterator(m_ptr - n);
        }

        difference_type operator-(const Iterator& other) const {
            return m_ptr - other.m_ptr;
        }

        // Comparison operators
        bool operator==(const Iterator& other) const {
            return m_ptr == other.m_ptr;
        }

        bool operator!=(const Iterator& other) const {
            return m_ptr != other.m_ptr;
        }

        bool operator<(const Iterator& other) const {
            return m_ptr < other.m_ptr;
        }

        bool operator<=(const Iterator& other) const {
            return m_ptr <= other.m_ptr;
        }

        bool operator>(const Iterator& other) const {
            return m_ptr > other.m_ptr;
        }

        bool operator>=(const Iterator& other) const {
            return m_ptr >= other.m_ptr;
        }

        // Friend declarations for reverse iterator access
        template <typename U>
        friend class Vector;
        friend class ConstIterator;
    };

    /**
     * @class ConstIterator
     * @brief Const random access iterator for Vector
     */
    class ConstIterator {
    private:
        const T* m_ptr;

    public:
        // Iterator traits
        using iterator_category = std::random_access_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = const T*;
        using reference = const T&;

        // Constructors
        ConstIterator() : m_ptr(nullptr) {}
        explicit ConstIterator(const T* ptr) : m_ptr(ptr) {}
        ConstIterator(const Iterator& it) : m_ptr(it.m_ptr) {}

        // Dereference operators
        reference operator*() const {
            if (m_ptr == nullptr) {
                throw std::runtime_error("Dereferencing invalid iterator");
            }
            return *m_ptr;
        }

        pointer operator->() const {
            if (m_ptr == nullptr) {
                throw std::runtime_error("Dereferencing invalid iterator");
            }
            return m_ptr;
        }

        reference operator[](difference_type n) const {
            return *(m_ptr + n);
        }

        // Increment/Decrement operators
        ConstIterator& operator++() {
            ++m_ptr;
            return *this;
        }

        ConstIterator operator++(int) {
            ConstIterator tmp = *this;
            ++m_ptr;
            return tmp;
        }

        ConstIterator& operator--() {
            --m_ptr;
            return *this;
        }

        ConstIterator operator--(int) {
            ConstIterator tmp = *this;
            --m_ptr;
            return tmp;
        }

        // Compound assignment operators
        ConstIterator& operator+=(difference_type n) {
            m_ptr += n;
            return *this;
        }

        ConstIterator& operator-=(difference_type n) {
            m_ptr -= n;
            return *this;
        }

        // Arithmetic operators
        ConstIterator operator+(difference_type n) const {
            return ConstIterator(m_ptr + n);
        }

        ConstIterator operator-(difference_type n) const {
            return ConstIterator(m_ptr - n);
        }

        difference_type operator-(const ConstIterator& other) const {
            return m_ptr - other.m_ptr;
        }

        // Comparison operators
        bool operator==(const ConstIterator& other) const {
            return m_ptr == other.m_ptr;
        }

        bool operator!=(const ConstIterator& other) const {
            return m_ptr != other.m_ptr;
        }

        bool operator<(const ConstIterator& other) const {
            return m_ptr < other.m_ptr;
        }

        bool operator<=(const ConstIterator& other) const {
            return m_ptr <= other.m_ptr;
        }

        bool operator>(const ConstIterator& other) const {
            return m_ptr > other.m_ptr;
        }

        bool operator>=(const ConstIterator& other) const {
            return m_ptr >= other.m_ptr;
        }

        template <typename U>
        friend class Vector;
    };

    using iterator = Iterator;
    using const_iterator = ConstIterator;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    // ============================================================
    // Constructors and Destructors
    // ============================================================

    /**
     * @brief Default constructor - creates empty vector
     */
    Vector() : m_cvec(cvector_create(sizeof(T))) {
        if (m_cvec == nullptr) {
            throw std::bad_alloc();
        }
    }

    /**
     * @brief Constructor with initial capacity
     */
    explicit Vector(size_t capacity) 
        : m_cvec(cvector_create_with_capacity(sizeof(T),
                                              std::max<size_t>(capacity, 1))) {
        if (m_cvec == nullptr) {
            throw std::bad_alloc();
        }
    }

    /**
     * @brief Copy constructor - creates a copy of another vector
     */
    Vector(const Vector& other) 
        : m_cvec(cvector_create_with_capacity(sizeof(T),
                                              std::max<size_t>(other.size(), 1))) {
        if (m_cvec == nullptr) {
            throw std::bad_alloc();
        }
        for (const auto& elem : other) {
            push_back(elem);
        }
    }

    /**
     * @brief Move constructor
     */
    Vector(Vector&& other) noexcept : m_cvec(other.m_cvec) {
        other.m_cvec = nullptr;
    }

    /**
     * @brief Constructor from initializer list
     */
    Vector(std::initializer_list<T> init) 
        : m_cvec(cvector_create_with_capacity(sizeof(T),
                                              std::max<size_t>(init.size(), 1))) {
        if (m_cvec == nullptr) {
            throw std::bad_alloc();
        }
        for (const auto& elem : init) {
            push_back(elem);
        }
    }

    /**
     * @brief Constructor from range
     */
    template <typename InputIt>
    Vector(InputIt first, InputIt last)
        : Vector() {
        try {
            for (; first != last; ++first) {
                push_back(*first);
            }
        } catch (...) {
            cvector_destroy(m_cvec);
            m_cvec = nullptr;
            throw;
        }
    }

    /**
     * @brief Destructor - cleans up resources
     */
    ~Vector() {
        if (m_cvec != nullptr) {
            cvector_destroy(m_cvec);
        }
    }

    // ============================================================
    // Assignment operators
    // ============================================================

    /**
     * @brief Copy assignment operator
     */
    Vector& operator=(const Vector& other) {
        if (this != &other) {
            Vector tmp(other);
            swap(tmp);
        }
        return *this;
    }

    /**
     * @brief Move assignment operator
     */
    Vector& operator=(Vector&& other) noexcept {
        if (this != &other) {
            Vector tmp(std::move(other));
            swap(tmp);
        }
        return *this;
    }

    /**
     * @brief Assign from initializer list
     */
    Vector& operator=(std::initializer_list<T> init) {
        Vector tmp(init);
        swap(tmp);
        return *this;
    }

    // ============================================================
    // Iterators
    // ============================================================

    /**
     * @brief Returns iterator to beginning
     */
    iterator begin() {
        return iterator(static_cast<T*>(cvector_data(m_cvec)));
    }

    /**
     * @brief Returns const iterator to beginning
     */
    const_iterator begin() const {
        return const_iterator(static_cast<const T*>(cvector_data(m_cvec)));
    }

    /**
     * @brief Returns const iterator to beginning
     */
    const_iterator cbegin() const {
        return begin();
    }

    /**
     * @brief Returns iterator to end
     */
    iterator end() {
        T* first = static_cast<T*>(cvector_data(m_cvec));
        return iterator(first == nullptr ? nullptr : first + size());
    }

    /**
     * @brief Returns const iterator to end
     */
    const_iterator end() const {
        const T* first = static_cast<const T*>(cvector_data(m_cvec));
        return const_iterator(first == nullptr ? nullptr : first + size());
    }

    /**
     * @brief Returns const iterator to end
     */
    const_iterator cend() const {
        return end();
    }

    /**
     * @brief Returns reverse iterator to beginning
     */
    reverse_iterator rbegin() {
        return reverse_iterator(end());
    }

    /**
     * @brief Returns const reverse iterator to beginning
     */
    const_reverse_iterator rbegin() const {
        return const_reverse_iterator(end());
    }

    /**
     * @brief Returns const reverse iterator to beginning
     */
    const_reverse_iterator crbegin() const {
        return rbegin();
    }

    /**
     * @brief Returns reverse iterator to end
     */
    reverse_iterator rend() {
        return reverse_iterator(begin());
    }

    /**
     * @brief Returns const reverse iterator to end
     */
    const_reverse_iterator rend() const {
        return const_reverse_iterator(begin());
    }

    /**
     * @brief Returns const reverse iterator to end
     */
    const_reverse_iterator crend() const {
        return rend();
    }

    // ============================================================
    // Element access
    // ============================================================

    /**
     * @brief Accesses element at index with bounds checking
     */
    reference at(size_t index) {
        if (index >= size()) {
            throw std::out_of_range("Vector index out of range");
        }
        return *get_at(index);
    }

    /**
     * @brief Accesses element at index with bounds checking (const)
     */
    const_reference at(size_t index) const {
        if (index >= size()) {
            throw std::out_of_range("Vector index out of range");
        }
        return *get_at(index);
    }

    /**
     * @brief Accesses element at index without bounds checking
     */
    reference operator[](size_t index) {
        return *get_at(index);
    }

    /**
     * @brief Accesses element at index without bounds checking (const)
     */
    const_reference operator[](size_t index) const {
        return *get_at(index);
    }

    /**
     * @brief Returns reference to first element
     */
    reference front() {
        if (empty()) {
            throw std::out_of_range("Vector is empty");
        }
        return *static_cast<T*>(cvector_front(m_cvec));
    }

    /**
     * @brief Returns const reference to first element
     */
    const_reference front() const {
        if (empty()) {
            throw std::out_of_range("Vector is empty");
        }
        return *static_cast<const T*>(cvector_front(m_cvec));
    }

    /**
     * @brief Returns reference to last element
     */
    reference back() {
        if (empty()) {
            throw std::out_of_range("Vector is empty");
        }
        return *static_cast<T*>(cvector_back(m_cvec));
    }

    /**
     * @brief Returns const reference to last element
     */
    const_reference back() const {
        if (empty()) {
            throw std::out_of_range("Vector is empty");
        }
        return *static_cast<const T*>(cvector_back(m_cvec));
    }

    /**
     * @brief Returns pointer to underlying data
     */
    pointer data() {
        return static_cast<T*>(cvector_data(m_cvec));
    }

    /**
     * @brief Returns const pointer to underlying data
     */
    const_pointer data() const {
        return static_cast<const T*>(cvector_data(m_cvec));
    }

    // ============================================================
    // Capacity
    // ============================================================

    /**
     * @brief Returns number of elements
     */
    size_t size() const {
        return cvector_size(m_cvec);
    }

    /**
     * @brief Returns allocated capacity
     */
    size_t capacity() const {
        return cvector_capacity(m_cvec);
    }

    /**
     * @brief Checks if vector is empty
     */
    bool empty() const {
        return cvector_is_empty(m_cvec) != 0;
    }

    /**
     * @brief Reserves capacity
     */
    void reserve(size_t new_capacity) {
        ensure_storage();
        if (cvector_reserve(m_cvec, new_capacity) != 0) {
            throw std::bad_alloc();
        }
    }

    /**
     * @brief Resizes vector
     */
    void resize(size_t new_size) {
        ensure_storage();
        if (cvector_resize(m_cvec, new_size) != 0) {
            throw std::bad_alloc();
        }
    }

    /**
     * @brief Resizes vector with fill value
     */
    void resize(size_t new_size, const T& value) {
        T value_copy = value;
        ensure_storage();
        size_t old_size = size();
        if (cvector_resize(m_cvec, new_size) != 0) {
            throw std::bad_alloc();
        }
        // Fill new elements with value
        for (size_t i = old_size; i < new_size; ++i) {
            (*this)[i] = value_copy;
        }
    }

    /**
     * @brief Shrinks capacity to match size
     */
    void shrink_to_fit() {
        ensure_storage();
        if (cvector_shrink_to_fit(m_cvec) != 0) {
            throw std::bad_alloc();
        }
    }

    // ============================================================
    // Modifiers
    // ============================================================

    /**
     * @brief Clears all elements
     */
    void clear() {
        cvector_clear(m_cvec);
    }

    /**
     * @brief Pushes element to back
     */
    void push_back(const T& value) {
        ensure_storage();
        if (cvector_push_back(m_cvec, &value) != 0) {
            throw std::bad_alloc();
        }
    }

    /**
     * @brief Pushes rvalue element to back
     */
    void push_back(T&& value) {
        push_back(value);
    }

    /**
     * @brief Removes last element
     */
    void pop_back() {
        if (empty()) {
            throw std::out_of_range("Vector is empty");
        }
        cvector_pop_back(m_cvec, nullptr);
    }

    /**
     * @brief Inserts element at position
     */
    iterator insert(const_iterator pos, const T& value) {
        bool was_moved_from = (m_cvec == nullptr);
        ensure_storage();
        size_t index = was_moved_from ? 0 : std::distance(cbegin(), pos);
        if (cvector_insert(m_cvec, index, &value) != 0) {
            throw std::bad_alloc();
        }
        return begin() + index;
    }

    /**
     * @brief Inserts rvalue element at position
     */
    iterator insert(const_iterator pos, T&& value) {
        return insert(pos, value);
    }

    /**
     * @brief Inserts count copies of value
     */
    iterator insert(const_iterator pos, size_t count, const T& value) {
        T value_copy = value;
        bool was_moved_from = (m_cvec == nullptr);
        ensure_storage();
        size_t index = was_moved_from ? 0 : std::distance(cbegin(), pos);
        for (size_t i = 0; i < count; ++i) {
            if (cvector_insert(m_cvec, index + i, &value_copy) != 0) {
                throw std::bad_alloc();
            }
        }
        return begin() + index;
    }

    /**
     * @brief Inserts from initializer list
     */
    iterator insert(const_iterator pos, std::initializer_list<T> init) {
        bool was_moved_from = (m_cvec == nullptr);
        ensure_storage();
        size_t index = was_moved_from ? 0 : std::distance(cbegin(), pos);
        size_t count = 0;
        for (const auto& elem : init) {
            if (cvector_insert(m_cvec, index + count, &elem) != 0) {
                throw std::bad_alloc();
            }
            ++count;
        }
        return begin() + index;
    }

    /**
     * @brief Erases element at position
     */
    iterator erase(const_iterator pos) {
        size_t index = std::distance(cbegin(), pos);
        if (cvector_erase(m_cvec, index) != 0) {
            throw std::runtime_error("Failed to erase element");
        }
        return begin() + index;
    }

    /**
     * @brief Erases range [first, last)
     */
    iterator erase(const_iterator first, const_iterator last) {
        size_t index = std::distance(cbegin(), first);
        size_t count = std::distance(first, last);
        for (size_t i = 0; i < count; ++i) {
            if (cvector_erase(m_cvec, index) != 0) {
                throw std::runtime_error("Failed to erase element");
            }
        }
        return begin() + index;
    }

    // ============================================================
    // Utility
    // ============================================================

    /**
     * @brief Swaps content with another vector
     */
    void swap(Vector& other) noexcept {
        std::swap(m_cvec, other.m_cvec);
    }

    /**
     * @brief Equality operator
     */
    bool operator==(const Vector& other) const {
        if (size() != other.size()) {
            return false;
        }
        return std::equal(begin(), end(), other.begin());
    }

    /**
     * @brief Inequality operator
     */
    bool operator!=(const Vector& other) const {
        return !(*this == other);
    }

    /**
     * @brief Less than operator
     */
    bool operator<(const Vector& other) const {
        return std::lexicographical_compare(begin(), end(), 
                                           other.begin(), other.end());
    }

    /**
     * @brief Less than or equal operator
     */
    bool operator<=(const Vector& other) const {
        return *this < other || *this == other;
    }

    /**
     * @brief Greater than operator
     */
    bool operator>(const Vector& other) const {
        return other < *this;
    }

    /**
     * @brief Greater than or equal operator
     */
    bool operator>=(const Vector& other) const {
        return other <= *this;
    }
};

// ============================================================
// Non-member functions
// ============================================================

/**
 * @brief Swaps two vectors
 */
template <typename T>
void swap(Vector<T>& lhs, Vector<T>& rhs) noexcept {
    lhs.swap(rhs);
}

/**
 * @brief Equality comparison
 */
template <typename T>
bool operator==(const Vector<T>& lhs, const Vector<T>& rhs) {
    return lhs == rhs;
}

/**
 * @brief Inequality comparison
 */
template <typename T>
bool operator!=(const Vector<T>& lhs, const Vector<T>& rhs) {
    return lhs != rhs;
}

/**
 * @brief Less than comparison
 */
template <typename T>
bool operator<(const Vector<T>& lhs, const Vector<T>& rhs) {
    return lhs < rhs;
}

/**
 * @brief Less than or equal comparison
 */
template <typename T>
bool operator<=(const Vector<T>& lhs, const Vector<T>& rhs) {
    return lhs <= rhs;
}

/**
 * @brief Greater than comparison
 */
template <typename T>
bool operator>(const Vector<T>& lhs, const Vector<T>& rhs) {
    return lhs > rhs;
}

/**
 * @brief Greater than or equal comparison
 */
template <typename T>
bool operator>=(const Vector<T>& lhs, const Vector<T>& rhs) {
    return lhs >= rhs;
}

} // namespace cvector

#endif /* CVECTOR_HPP */
