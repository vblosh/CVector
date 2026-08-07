#include <iostream>
#include <cassert>
#include <vector>
#include <algorithm>
#include "cvector.hpp"

int test_construction() {
    // Default constructor
    cvector::Vector<int> v1;
    assert(v1.empty());
    assert(v1.size() == 0);
    
    // Constructor with capacity
    cvector::Vector<int> v2(20);
    assert(v2.empty());
    assert(v2.capacity() >= 20);
    
    // Copy constructor
    v1.push_back(42);
    cvector::Vector<int> v3(v1);
    assert(v3.size() == 1);
    assert(v3[0] == 42);
    
    // Move constructor
    cvector::Vector<int> v4(std::move(v1));
    assert(v4.size() == 1);
    assert(v1.size() == 0);  // v1 should be empty after move
    
    // Initializer list constructor
    cvector::Vector<int> v5{1, 2, 3, 4, 5};
    assert(v5.size() == 5);
    assert(v5[0] == 1);
    assert(v5[4] == 5);
    
    std::cout << "? test_construction passed\n";
    return 0;
}

int test_push_pop() {
    cvector::Vector<int> v;
    
    // Push back
    for (int i = 0; i < 5; ++i) {
        v.push_back(i * 10);
    }
    assert(v.size() == 5);
    
    // Pop back
    v.pop_back();
    assert(v.size() == 4);
    assert(v.back() == 30);
    
    std::cout << "? test_push_pop passed\n";
    return 0;
}

int test_element_access() {
    cvector::Vector<int> v{10, 20, 30, 40, 50};
    
    // operator[]
    assert(v[0] == 10);
    assert(v[2] == 30);
    assert(v[4] == 50);
    
    // at()
    assert(v.at(1) == 20);
    assert(v.at(3) == 40);
    
    // front() and back()
    assert(v.front() == 10);
    assert(v.back() == 50);
    
    // Bounds checking
    try {
        v.at(5);
        assert(false);  // Should throw
    } catch (const std::out_of_range&) {
        // Expected
    }
    
    std::cout << "? test_element_access passed\n";
    return 0;
}

int test_iterators() {
    cvector::Vector<int> v{1, 2, 3, 4, 5};
    
    // Forward iteration
    int count = 0;
    for (auto it = v.begin(); it != v.end(); ++it) {
        assert(*it == ++count);
    }
    
    // Reverse iteration
    count = 6;
    for (auto it = v.rbegin(); it != v.rend(); ++it) {
        assert(*it == --count);
    }
    
    // const iterator
    const cvector::Vector<int>& cv = v;
    count = 0;
    for (auto it = cv.begin(); it != cv.end(); ++it) {
        assert(*it == ++count);
    }
    
    // Range-based for loop
    count = 0;
    for (const auto& elem : v) {
        assert(elem == ++count);
    }
    
    // Iterator arithmetic
    auto it = v.begin();
    assert(*(it + 2) == 3);
    assert(*(v.end() - 1) == 5);
    
    // Iterator distance
    assert(std::distance(v.begin(), v.end()) == 5);
    
    std::cout << "? test_iterators passed\n";
    return 0;
}

int test_modification() {
    cvector::Vector<int> v{1, 2, 3, 4, 5};
    
    // Modify through iterator
    auto it = v.begin();
    ++it;
    *it = 99;
    assert(v[1] == 99);
    
    // Insert single element
    v.insert(v.begin() + 2, 77);
    assert(v.size() == 6);
    assert(v[2] == 77);
    
    // Insert multiple elements (count version)
    cvector::Vector<int> v2{10, 20, 30};
    v2.insert(v2.cbegin() + 1, size_t(3), 15);
    assert(v2.size() == 6);
    assert(v2[1] == 15);
    assert(v2[2] == 15);
    assert(v2[3] == 15);
    
    // Erase single element
    cvector::Vector<int> v3{1, 2, 3, 4, 5};
    v3.erase(v3.begin() + 2);
    assert(v3.size() == 4);
    assert(v3[2] == 4);
    
    // Erase range
    cvector::Vector<int> v4{1, 2, 3, 4, 5};
    v4.erase(v4.begin() + 1, v4.begin() + 3);
    assert(v4.size() == 3);
    assert(v4[0] == 1);
    assert(v4[1] == 4);
    assert(v4[2] == 5);
    
    // Clear
    v4.clear();
    assert(v4.empty());
    
    std::cout << "? test_modification passed\n";
    return 0;
}

int test_capacity() {
    cvector::Vector<int> v;
    assert(v.capacity() >= 10);  // Initial capacity
    
    // Reserve
    v.reserve(100);
    assert(v.capacity() >= 100);
    
    // Resize
    v.resize(10);
    assert(v.size() == 10);
    
    // Resize with value
    cvector::Vector<int> v2{1, 2, 3};
    v2.resize(5, 99);
    assert(v2.size() == 5);
    assert(v2[3] == 99);
    assert(v2[4] == 99);
    
    // Shrink to fit
    cvector::Vector<int> v3{1, 2, 3};
    v3.reserve(1000);
    v3.shrink_to_fit();
    assert(v3.capacity() == 3);
    assert(v3.size() == 3);
    
    std::cout << "? test_capacity passed\n";
    return 0;
}

int test_comparison() {
    cvector::Vector<int> v1{1, 2, 3};
    cvector::Vector<int> v2{1, 2, 3};
    cvector::Vector<int> v3{1, 2, 4};
    cvector::Vector<int> v4{1, 2};
    
    // Equality
    assert(v1 == v2);
    assert(!(v1 == v3));
    
    // Inequality
    assert(v1 != v3);
    assert(!(v1 != v2));
    
    // Less than
    assert(v1 < v3);
    assert(!(v3 < v1));
    assert(v4 < v1);
    
    // Greater than
    assert(v3 > v1);
    assert(!(v1 > v3));
    
    // Less than or equal
    assert(v1 <= v2);
    assert(v1 <= v3);
    
    // Greater than or equal
    assert(v2 >= v1);
    assert(v3 >= v1);
    
    std::cout << "? test_comparison passed\n";
    return 0;
}

int test_assignment() {
    // Copy assignment
    cvector::Vector<int> v1{1, 2, 3};
    cvector::Vector<int> v2;
    v2 = v1;
    assert(v2 == v1);
    assert(v2[0] == 1);
    
    // Move assignment
    cvector::Vector<int> v3;
    v3 = std::move(v1);
    assert(v3[0] == 1);
    assert(v1.size() == 0);
    
    // Initializer list assignment
    cvector::Vector<int> v4;
    v4 = {5, 6, 7};
    assert(v4.size() == 3);
    assert(v4[0] == 5);
    
    std::cout << "? test_assignment passed\n";
    return 0;
}

int test_swap() {
    cvector::Vector<int> v1{1, 2, 3};
    cvector::Vector<int> v2{4, 5, 6, 7};
    
    size_t size1 = v1.size();
    size_t size2 = v2.size();
    
    v1.swap(v2);
    
    assert(v1.size() == size2);
    assert(v2.size() == size1);
    assert(v1[0] == 4);
    assert(v2[0] == 1);
    
    // Using non-member swap
    cvector::swap(v1, v2);
    assert(v1[0] == 1);
    assert(v2[0] == 4);
    
    std::cout << "? test_swap passed\n";
    return 0;
}

int test_stl_algorithms() {
    cvector::Vector<int> v{3, 1, 4, 1, 5, 9, 2, 6};
    
    // std::find
    auto it = std::find(v.begin(), v.end(), 4);
    assert(it != v.end());
    assert(*it == 4);
    assert(std::distance(v.begin(), it) == 2);
    
    // std::count
    size_t count = std::count(v.begin(), v.end(), 1);
    assert(count == 2);
    
    // std::sort
    cvector::Vector<int> v_copy = v;
    std::sort(v_copy.begin(), v_copy.end());
    assert(v_copy[0] == 1);
    assert(v_copy[7] == 9);
    
    // std::reverse
    cvector::Vector<int> v2{1, 2, 3, 4, 5};
    std::reverse(v2.begin(), v2.end());
    assert(v2[0] == 5);
    assert(v2[4] == 1);
    
    // std::fill
    cvector::Vector<int> v3(5);
    std::fill(v3.begin(), v3.end(), 42);
    for (const auto& elem : v3) {
        assert(elem == 42);
    }
    
    std::cout << "? test_stl_algorithms passed\n";
    return 0;
}

int test_complex_types() {
    struct Point {
        int x, y;
        bool operator==(const Point& other) const {
            return x == other.x && y == other.y;
        }
    };
    
    cvector::Vector<Point> v;
    v.push_back({1, 2});
    v.push_back({3, 4});
    v.push_back({5, 6});
    
    assert(v.size() == 3);
    assert(v[0].x == 1);
    assert(v[1].y == 4);
    assert(v.back().x == 5);
    
    // Modify through iterator
    v[1].y = 99;
    assert(v[1].y == 99);
    
    std::cout << "? test_complex_types passed\n";
    return 0;
}

int test_empty_access_and_iterator_operations() {
    cvector::Vector<int> empty;
    try {
        empty.front();
        assert(false);
    } catch (const std::out_of_range&) {
    }
    try {
        empty.back();
        assert(false);
    } catch (const std::out_of_range&) {
    }
    try {
        empty.pop_back();
        assert(false);
    } catch (const std::out_of_range&) {
    }

    cvector::Vector<int>::iterator invalid;
    try {
        (void)*invalid;
        assert(false);
    } catch (const std::runtime_error&) {
    }
    try {
        (void)invalid.operator->();
        assert(false);
    } catch (const std::runtime_error&) {
    }

    cvector::Vector<int>::const_iterator const_invalid;
    try {
        (void)*const_invalid;
        assert(false);
    } catch (const std::runtime_error&) {
    }
    try {
        (void)const_invalid.operator->();
        assert(false);
    } catch (const std::runtime_error&) {
    }

    cvector::Vector<int> values{1, 2, 3};
    auto it = values.begin();
    assert(*it++ == 1);
    assert(*it == 2);
    assert(*++it == 3);
    assert(*it-- == 3);
    assert(*--it == 1);
    it += 2;
    assert(it[-1] == 2);
    it -= 1;
    assert(it == values.begin() + 1);
    assert(it > values.begin());
    assert(it >= values.begin());
    assert(it <= values.end());

    const cvector::Vector<int>& const_values = values;
    auto cit = const_values.cbegin();
    assert(*cit++ == 1);
    assert(*++cit == 3);
    assert(*cit-- == 3);
    assert(*--cit == 1);
    cit += 2;
    assert(cit[-1] == 2);
    cit -= 1;
    assert(cit > const_values.cbegin());
    assert(cit >= const_values.cbegin());
    assert(cit <= const_values.cend());
    assert(const_values.crbegin() != const_values.crend());
    assert(*const_values.crbegin() == 3);
    assert(const_values.data()[1] == 2);

    std::cout << "? test_empty_access_and_iterator_operations passed\n";
    return 0;
}

int test_range_and_initializer_list_insert(void) {
    std::vector<int> source{4, 5, 6};
    cvector::Vector<int> values(source.begin(), source.end());
    assert(values.size() == 3);

    auto inserted = values.insert(values.cbegin() + 1, {7, 8});
    assert(*inserted == 7);
    assert(values.size() == 5);
    assert(values[0] == 4);
    assert(values[1] == 7);
    assert(values[2] == 8);

    values = values;
    assert(values.size() == 5);
    std::cout << "? test_range_and_initializer_list_insert passed\n";
    return 0;
}

int test_empty_copy_end_insert_and_moved_from_reuse(void) {
    cvector::Vector<int> empty;
    cvector::Vector<int> empty_copy(empty);
    assert(empty_copy.empty());

    cvector::Vector<int> empty_list(std::initializer_list<int>{});
    assert(empty_list.empty());

    cvector::Vector<int> values{1, 2};
    auto inserted = values.insert(values.end(), 3);
    assert(inserted == values.end() - 1);
    assert(values.back() == 3);

    cvector::Vector<int> moved_to(std::move(values));
    assert(moved_to.size() == 3);
    assert(values.empty());
    values.push_back(4);
    assert(values.size() == 1);
    assert(values.front() == 4);

    cvector::Vector<int> moved_again(std::move(values));
    values.insert(values.end(), 5);
    assert(values.size() == 1);
    assert(values[0] == 5);
    assert(moved_again[0] == 4);

    cvector::Vector<int> aliases{10, 20};
    aliases.resize(5, aliases[1]);
    assert(aliases[2] == 20);
    assert(aliases[4] == 20);
    aliases.insert(aliases.cbegin(), size_t(3), aliases[4]);
    assert(aliases[0] == 20);
    assert(aliases[2] == 20);

    std::cout << "? test_empty_copy_end_insert_and_moved_from_reuse passed\n";
    return 0;
}

int test_adopt_and_copy_from_cvector(void) {
    CVector *adopted_raw = cvector_create_with_capacity(sizeof(int), 4);
    assert(adopted_raw != nullptr);
    int one = 1;
    int two = 2;
    assert(cvector_push_back(adopted_raw, &one) == 0);
    assert(cvector_push_back(adopted_raw, &two) == 0);
    void *adopted_data = adopted_raw->data;

    {
        auto adopted = cvector::Vector<int>::adopt(adopted_raw);
        assert(adopted.size() == 2);
        assert(adopted[0] == 1);
        assert(adopted[1] == 2);
        assert(adopted.data() == adopted_data);
        adopted[0] = 10;
        assert(*(int *)adopted_data == 10);
    }

    CVector *source = cvector_create_with_capacity(sizeof(int), 4);
    assert(source != nullptr);
    assert(cvector_push_back(source, &one) == 0);
    assert(cvector_push_back(source, &two) == 0);
    {
        auto copy = cvector::Vector<int>::copy_from(source);
        assert(copy.size() == 2);
        assert(copy.capacity() == source->capacity);
        assert(copy.data() != source->data);
        copy[0] = 99;
        assert(*(int *)cvector_at(source, 0) == 1);
    }
    cvector_destroy(source);

    bool threw = false;
    try {
        (void)cvector::Vector<int>::adopt(nullptr);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    CVector wrong_element_size = {&one, 1, 1, sizeof(char)};
    threw = false;
    try {
        (void)cvector::Vector<int>::copy_from(&wrong_element_size);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    CVector invalid_metadata = {&one, 2, 1, sizeof(int)};
    threw = false;
    try {
        (void)cvector::Vector<int>::adopt(&invalid_metadata);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    std::cout << "? test_adopt_and_copy_from_cvector passed\n";
    return 0;
}

int main() {
    std::cout << "Running C++ Vector Tests...\n\n";
    
    if (test_construction() != 0) return 1;
    if (test_push_pop() != 0) return 1;
    if (test_element_access() != 0) return 1;
    if (test_iterators() != 0) return 1;
    if (test_modification() != 0) return 1;
    if (test_capacity() != 0) return 1;
    if (test_comparison() != 0) return 1;
    if (test_assignment() != 0) return 1;
    if (test_swap() != 0) return 1;
    if (test_stl_algorithms() != 0) return 1;
    if (test_complex_types() != 0) return 1;
    if (test_empty_access_and_iterator_operations() != 0) return 1;
    if (test_range_and_initializer_list_insert() != 0) return 1;
    if (test_empty_copy_end_insert_and_moved_from_reuse() != 0) return 1;
    if (test_adopt_and_copy_from_cvector() != 0) return 1;
    
    std::cout << "\n? All C++ tests passed!\n";
    return 0;
}
