#include <iostream>
#include <algorithm>
#include "cvector.hpp"

int main() {
    std::cout << "=== CVector C++ Interface Example ===\n\n";

    // Create a vector of integers using C++ interface
    cvector::Vector<int> vec;
    
    // Add some elements using push_back
    std::cout << "Adding numbers 1-5...\n";
    for (int i = 1; i <= 5; ++i) {
        vec.push_back(i);
    }
    std::cout << "Size: " << vec.size() << ", Capacity: " << vec.capacity() << "\n\n";

    // Iterate using range-based for loop
    std::cout << "Elements using range-based for loop:\n";
    for (const auto& elem : vec) {
        std::cout << "  " << elem << "\n";
    }
    std::cout << "\n";

    // Iterate using explicit iterators
    std::cout << "Elements using iterators:\n";
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        std::cout << "  " << *it << "\n";
    }
    std::cout << "\n";

    // Reverse iteration
    std::cout << "Reverse iteration:\n";
    for (auto it = vec.rbegin(); it != vec.rend(); ++it) {
        std::cout << "  " << *it << "\n";
    }
    std::cout << "\n";

    // Element access
    std::cout << "Element access:\n";
    std::cout << "  First: " << vec.front() << "\n";
    std::cout << "  Last: " << vec.back() << "\n";
    std::cout << "  Index 2: " << vec[2] << "\n";
    std::cout << "  At index 3: " << vec.at(3) << "\n\n";

    // Modify elements using iterators
    std::cout << "Doubling all elements...\n";
    for (auto& elem : vec) {
        elem *= 2;
    }
    std::cout << "Modified elements: ";
    for (const auto& elem : vec) {
        std::cout << elem << " ";
    }
    std::cout << "\n\n";

    // Insert operation
    std::cout << "Inserting 99 at index 2...\n";
    vec.insert(vec.cbegin() + 2, 99);
    std::cout << "Elements: ";
    for (const auto& elem : vec) {
        std::cout << elem << " ";
    }
    std::cout << "\nSize: " << vec.size() << "\n\n";

    // Erase operation
    std::cout << "Erasing element at index 2...\n";
    vec.erase(vec.cbegin() + 2);
    std::cout << "Elements: ";
    for (const auto& elem : vec) {
        std::cout << elem << " ";
    }
    std::cout << "\nSize: " << vec.size() << "\n\n";

    // Pop operation
    std::cout << "Popping last element...\n";
    vec.pop_back();
    std::cout << "Elements: ";
    for (const auto& elem : vec) {
        std::cout << elem << " ";
    }
    std::cout << "\nSize: " << vec.size() << "\n\n";

    // Using STL algorithms
    std::cout << "Using STL algorithms:\n";
    
    // Find using std::find
    auto it = std::find(vec.begin(), vec.end(), 4);
    if (it != vec.end()) {
        std::cout << "  Found 4 at index: " << std::distance(vec.begin(), it) << "\n";
    }
    
    // Count using std::count
    size_t count = std::count(vec.begin(), vec.end(), 2);
    std::cout << "  Count of 2: " << count << "\n";
    
    // Sort (needs a copy since we want to preserve original)
    cvector::Vector<int> sorted_vec = vec;
    std::sort(sorted_vec.begin(), sorted_vec.end(), std::greater<int>());
    std::cout << "  Sorted descending: ";
    for (const auto& elem : sorted_vec) {
        std::cout << elem << " ";
    }
    std::cout << "\n\n";

    // Copy constructor
    std::cout << "Copy constructor test:\n";
    cvector::Vector<int> vec_copy = vec;
    std::cout << "  Original size: " << vec.size() << "\n";
    std::cout << "  Copy size: " << vec_copy.size() << "\n";
    std::cout << "  Are they equal? " << (vec == vec_copy ? "yes" : "no") << "\n\n";

    // Initializer list constructor
    std::cout << "Initializer list constructor:\n";
    cvector::Vector<int> vec_init{10, 20, 30, 40, 50};
    std::cout << "  Elements: ";
    for (const auto& elem : vec_init) {
        std::cout << elem << " ";
    }
    std::cout << "\n\n";

    // Comparison operators
    std::cout << "Comparison operators:\n";
    cvector::Vector<int> vec_a{1, 2, 3};
    cvector::Vector<int> vec_b{1, 2, 3};
    cvector::Vector<int> vec_c{1, 2, 4};
    std::cout << "  vec_a == vec_b: " << (vec_a == vec_b ? "true" : "false") << "\n";
    std::cout << "  vec_a < vec_c: " << (vec_a < vec_c ? "true" : "false") << "\n";
    std::cout << "  vec_c > vec_a: " << (vec_c > vec_a ? "true" : "false") << "\n\n";

    std::cout << "All C++ interface tests completed successfully!\n";
    return 0;
}
