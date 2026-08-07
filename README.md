# CVector - A C Implementation of std::vector

A high-performance, generic dynamic array container for C, similar to C++'s `std::vector`. It provides a flexible, type-safe way to manage dynamically-sized arrays of any data type.

## Features

- **Generic**: Works with any data type (primitives, structs, pointers, etc.)
- **Dynamic Allocation**: Automatically grows and shrinks as needed
- **Standard Operations**: Push, pop, insert, erase, and more
- **Efficient**: Uses exponential growth strategy for O(1) amortized push operations
- **Type-Safe**: Element size specified at creation, memory-safe operations
- **C11 Compatible**: Written in C11 with no external dependencies
- **C++ Wrapper**: STL-style interface for trivially copyable value types
- **Iterator Support**: Bidirectional and random access iterators for STL algorithms
- **Well-Tested**: Comprehensive test suite included
- **Examples**: Multiple examples demonstrating various use cases

## Building

### Prerequisites
- CMake 3.10 or higher
- C11 compatible compiler (GCC, Clang, MSVC, etc.)

### Build Instructions

```bash
# Create build directory
mkdir build
cd build

# Generate build files
cmake ..

# Build the library
cmake --build .

# Build tests and examples
cmake --build . --config Debug

# Run tests
ctest
```

### Build Options

```bash
# Disable building examples
cmake .. -DBUILD_EXAMPLES=OFF

# Specify generator
cmake .. -G "Visual Studio 16 2019"
cmake .. -G "Unix Makefiles"
```

## Installation

```bash
cmake --install . --prefix /usr/local
```

## Usage

### Basic Example

```c
#include <stdio.h>
#include "cvector.h"

int main(void) {
    // Create a vector of integers
    CVector *vec = cvector_create(sizeof(int));
    
    // Add elements
    int values[] = {10, 20, 30};
    for (int i = 0; i < 3; i++) {
        cvector_push_back(vec, &values[i]);
    }
    
    // Access elements
    for (size_t i = 0; i < cvector_size(vec); i++) {
        int *val = (int *)cvector_at(vec, i);
        printf("Element %zu: %d\n", i, *val);
    }
    
    // Cleanup
    cvector_destroy(vec);
    return 0;
}
```

### Working with Structures

```c
typedef struct {
    int x;
    int y;
} Point;

CVector *points = cvector_create(sizeof(Point));

Point p = {10, 20};
cvector_push_back(points, &p);

Point *retrieved = (Point *)cvector_at(points, 0);
printf("Point: (%d, %d)\n", retrieved->x, retrieved->y);

cvector_destroy(points);
```

### Working with Strings

```c
// Vector of string pointers
CVector *strings = cvector_create(sizeof(char*));

char *str = (char *)malloc(6);
strcpy(str, "hello");
cvector_push_back(strings, &str);

char **retrieved = (char **)cvector_at(strings, 0);
printf("String: %s\n", *retrieved);

// Don't forget to free allocated strings
free(*retrieved);
cvector_destroy(strings);
```

## C++ Interface

CVector also provides a modern C++ template wrapper with STL-compatible iterators, making it easy to use with C++ standard library algorithms.

The wrapper intentionally accepts only trivially copyable and trivially
destructible value types. Use `std::vector` for owning or non-trivial C++ types
such as `std::string`, smart pointers, or classes with custom constructors and
destructors.

### C++ Basic Example

```cpp
#include <iostream>
#include "cvector.hpp"

int main() {
    // Create a vector using C++ interface
    cvector::Vector<int> vec;
    
    // Add elements
    vec.push_back(10);
    vec.push_back(20);
    vec.push_back(30);
    
    // Iterate using range-based for loop
    for (const auto& elem : vec) {
        std::cout << elem << " ";
    }
    std::cout << "\n";
    
    // Use STL algorithms
    std::sort(vec.begin(), vec.end());
    std::reverse(vec.begin(), vec.end());
    
    return 0;
}
```

### C++ Iterator Examples

```cpp
#include "cvector.hpp"
#include <algorithm>

cvector::Vector<int> vec{1, 2, 3, 4, 5};

// Forward iteration
for (auto it = vec.begin(); it != vec.end(); ++it) {
    std::cout << *it << " ";
}

// Reverse iteration
for (auto it = vec.rbegin(); it != vec.rend(); ++it) {
    std::cout << *it << " ";
}

// Const iteration
for (auto it = vec.cbegin(); it != vec.cend(); ++it) {
    std::cout << *it << " ";
}

// Random access
std::cout << vec[2] << "\n";  // Direct access
std::cout << vec.at(3) << "\n";  // Bounds-checked access

// STL Algorithms
auto it = std::find(vec.begin(), vec.end(), 3);
int count = std::count(vec.begin(), vec.end(), 2);
std::sort(vec.begin(), vec.end());
```

### C++ Constructors and Assignment

```cpp
#include "cvector.hpp"

// Default constructor
cvector::Vector<int> v1;

// With initial capacity
cvector::Vector<int> v2(100);

// Copy constructor
cvector::Vector<int> v3 = v1;

// Move constructor
cvector::Vector<int> v4 = std::move(v1);

// From initializer list
cvector::Vector<int> v5{1, 2, 3, 4, 5};

// From range
std::vector<int> std_vec = {10, 20, 30};
cvector::Vector<int> v6(std_vec.begin(), std_vec.end());

// Copy assignment
v1 = v2;

// Move assignment
v1 = std::move(v2);

// Initializer list assignment
v1 = {5, 6, 7};

// Copy an existing C vector (the caller retains ownership of raw)
CVector *raw = cvector_create(sizeof(int));
cvector::Vector<int> copied = cvector::Vector<int>::copy_from(raw);

// Or adopt it (ownership transfers; do not use or destroy raw afterward)
cvector::Vector<int> adopted = cvector::Vector<int>::adopt(raw);
```

### C++ API Reference

#### Constructors
- `Vector()` - Default constructor
- `Vector(size_t capacity)` - Constructor with capacity
- `Vector(const Vector& other)` - Copy constructor
- `Vector(Vector&& other)` - Move constructor
- `Vector(std::initializer_list<T> init)` - Initializer list constructor
- `Vector(InputIt first, InputIt last)` - Range constructor
- `static Vector adopt(CVector* vec)` - Take ownership of an existing C vector
- `static Vector copy_from(const CVector* vec)` - Copy an existing C vector

#### Iterators
- `iterator begin()` / `const_iterator begin() const` - Get begin iterator
- `iterator end()` / `const_iterator end() const` - Get end iterator
- `reverse_iterator rbegin()` / `const_reverse_iterator rbegin() const` - Get reverse begin
- `reverse_iterator rend()` / `const_reverse_iterator rend() const` - Get reverse end
- `const_iterator cbegin() const` - Get const begin iterator
- `const_iterator cend() const` - Get const end iterator

#### Element Access
- `T& at(size_t index)` - Access with bounds checking
- `T& operator[](size_t index)` - Access without bounds checking
- `T& front()` - Get first element
- `T& back()` - Get last element
- `T* data()` - Get pointer to data

#### Capacity
- `size_t size() const` - Get number of elements
- `size_t capacity() const` - Get allocated capacity
- `bool empty() const` - Check if empty
- `void reserve(size_t new_capacity)` - Reserve capacity
- `void resize(size_t new_size)` - Resize vector
- `void resize(size_t new_size, const T& value)` - Resize with fill value
- `void shrink_to_fit()` - Shrink to fit

#### Modifiers
- `void push_back(const T& value)` - Add element
- `void pop_back()` - Remove last element
- `void clear()` - Clear all elements
- `iterator insert(const_iterator pos, const T& value)` - Insert element
- `iterator insert(const_iterator pos, size_t count, const T& value)` - Insert copies
- `iterator insert(const_iterator pos, std::initializer_list<T> init)` - Insert from list
- `iterator erase(const_iterator pos)` - Erase single element
- `iterator erase(const_iterator first, const_iterator last)` - Erase range

#### Comparison
- `operator==`, `operator!=`, `operator<`, `operator<=`, `operator>`, `operator>=`

#### Utility
- `void swap(Vector& other)` - Swap with another vector

## API Reference

### Creation and Destruction

- `CVector* cvector_create(size_t element_size)` - Create a vector with default capacity
- `CVector* cvector_create_with_capacity(size_t element_size, size_t capacity)` - Create a vector with specified capacity
- `void cvector_destroy(CVector *vec)` - Destroy vector and free memory

### Size and Capacity

- `size_t cvector_size(const CVector *vec)` - Get number of elements
- `size_t cvector_capacity(const CVector *vec)` - Get allocated capacity
- `int cvector_is_empty(const CVector *vec)` - Check if vector is empty
- `int cvector_reserve(CVector *vec, size_t new_capacity)` - Reserve capacity
- `int cvector_resize(CVector *vec, size_t new_size)` - Resize vector
- `int cvector_shrink_to_fit(CVector *vec)` - Shrink capacity to size

### Element Access

- `void* cvector_at(const CVector *vec, size_t index)` - Get element at index
- `void* cvector_front(const CVector *vec)` - Get first element
- `void* cvector_back(const CVector *vec)` - Get last element
- `void* cvector_data(const CVector *vec)` - Get pointer to data array

### Modification

- `int cvector_push_back(CVector *vec, const void *element)` - Add element at end
- `int cvector_pop_back(CVector *vec, void *out_element)` - Remove and get last element
- `int cvector_insert(CVector *vec, size_t index, const void *element)` - Insert element at position
- `int cvector_erase(CVector *vec, size_t index)` - Remove element at position
- `void cvector_clear(CVector *vec)` - Remove all elements

## Performance Characteristics

| Operation | Time Complexity | Notes |
|-----------|-----------------|-------|
| push_back | O(1) amortized | Exponential growth strategy |
| pop_back | O(1) | No reallocation |
| at/front/back | O(1) | Direct pointer arithmetic |
| insert | O(n) | May require memory move |
| erase | O(n) | May require memory move |
| reserve | O(n) | For initial allocation or shrinking |
| clear | O(1) | Just updates size |

## Memory Management

The vector automatically manages its internal memory. However:

1. **Elements containing pointers**: If your elements contain pointers (e.g., strings), you are responsible for managing the memory those pointers reference.
2. **Copy semantics**: `cvector_push_back` and `cvector_insert` copy elements byte-by-byte. If you need deep copies of complex types, implement them manually.
3. **Cleanup**: Always call `cvector_destroy` to free the vector's internal memory.

## Examples

The `examples/` directory contains:
- `basic_example.c` - Basic integer vector operations (C)
- `strings_example.c` - Vector of strings (char pointers) (C)
- `structs_example.c` - Vector of complex structures (C)
- `cpp_interface_example.cpp` - C++ interface demonstration with STL algorithms

Run with:
```bash
./example_basic
./example_strings
./example_structs
./example_cpp_interface
```

## Testing

Run the test suite:
```bash
cd build
ctest --verbose
```

Or run directly:
```bash
./test_cvector
```

Tests cover:
- Creation and destruction
- Push/pop operations
- Element access (at, front, back)
- Capacity management
- Insert/erase operations
- Resizing and clearing
- Storage of complex types (structs)

## Limitations

- **Not thread-safe**: Synchronization must be handled externally
- **No callbacks**: Custom cleanup must be done before destroying
- **Simple growth strategy**: Uses 1.5x multiplier; not optimizable
- **C Interface**: Limited to pointer-based access (use C++ for easier use)

## License

This project is provided as-is for educational and commercial use.

## Contributing

Contributions are welcome! Please ensure:
1. Code follows existing style
2. All tests pass
3. New features include tests
4. Documentation is updated

## Author

Created as a demonstration of generic containers in C.
