#ifndef CVECTOR_H
#define CVECTOR_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @struct CVector
 * @brief A dynamic array container similar to std::vector
 *
 * This structure represents a dynamic vector that can grow and shrink
 * as needed. It maintains a contiguous block of memory for efficient
 * access and iteration.
 */
typedef struct {
    void *data;           /**< Pointer to the underlying data */
    size_t size;          /**< Number of elements currently in the vector */
    size_t capacity;      /**< Total allocated capacity */
    size_t element_size;  /**< Size of each element in bytes */
} CVector;

/**
 * @brief Creates a new vector with a specified element size
 * @param element_size The size of each element in bytes
 * @return A newly initialized CVector, or NULL if allocation fails
 */
CVector* cvector_create(size_t element_size);

/**
 * @brief Creates a vector with a pre-allocated capacity
 * @param element_size The size of each element in bytes
 * @param capacity Initial capacity to allocate
 * @return A newly initialized CVector with reserved capacity
 */
CVector* cvector_create_with_capacity(size_t element_size, size_t capacity);

/**
 * @brief Destroys a vector and frees all allocated memory
 * @param vec Pointer to the vector to destroy
 */
void cvector_destroy(CVector *vec);

/**
 * @brief Returns the current number of elements in the vector
 * @param vec Pointer to the vector
 * @return The number of elements
 */
size_t cvector_size(const CVector *vec);

/**
 * @brief Returns the current capacity of the vector
 * @param vec Pointer to the vector
 * @return The allocated capacity
 */
size_t cvector_capacity(const CVector *vec);

/**
 * @brief Checks if the vector is empty
 * @param vec Pointer to the vector
 * @return Non-zero if empty, 0 if not empty
 */
int cvector_is_empty(const CVector *vec);

/**
 * @brief Pushes an element to the back of the vector
 * @param vec Pointer to the vector
 * @param element Pointer to the element to push
 * @return 0 on success, non-zero on failure
 */
int cvector_push_back(CVector *vec, const void *element);

/**
 * @brief Removes and returns the last element from the vector
 * @param vec Pointer to the vector
 * @param out_element Pointer to store the popped element
 * @return 0 on success, non-zero if vector is empty
 */
int cvector_pop_back(CVector *vec, void *out_element);

/**
 * @brief Gets a pointer to the element at the specified index
 * @param vec Pointer to the vector
 * @param index The index of the element
 * @return Pointer to the element, or NULL if index is out of bounds
 */
void* cvector_at(const CVector *vec, size_t index);

/**
 * @brief Gets a pointer to the first element
 * @param vec Pointer to the vector
 * @return Pointer to the first element, or NULL if vector is empty
 */
void* cvector_front(const CVector *vec);

/**
 * @brief Gets a pointer to the last element
 * @param vec Pointer to the vector
 * @return Pointer to the last element, or NULL if vector is empty
 */
void* cvector_back(const CVector *vec);

/**
 * @brief Reserves capacity for a specified number of elements
 * @param vec Pointer to the vector
 * @param new_capacity The desired capacity
 * @return 0 on success, non-zero on failure
 */
int cvector_reserve(CVector *vec, size_t new_capacity);

/**
 * @brief Resizes the vector to contain count elements
 * @param vec Pointer to the vector
 * @param new_size The desired size
 * @return 0 on success, non-zero on failure
 *
 * If new_size is greater than the current size, new elements are zero-initialized.
 * If new_size is less than the current size, elements are removed.
 */
int cvector_resize(CVector *vec, size_t new_size);

/**
 * @brief Clears all elements from the vector
 * @param vec Pointer to the vector
 */
void cvector_clear(CVector *vec);

/**
 * @brief Inserts an element at the specified position
 * @param vec Pointer to the vector
 * @param index The position to insert at
 * @param element Pointer to the element to insert
 * @return 0 on success, non-zero on failure
 */
int cvector_insert(CVector *vec, size_t index, const void *element);

/**
 * @brief Removes an element at the specified position
 * @param vec Pointer to the vector
 * @param index The position to remove from
 * @return 0 on success, non-zero on failure
 */
int cvector_erase(CVector *vec, size_t index);

/**
 * @brief Gets a pointer to the underlying data array
 * @param vec Pointer to the vector
 * @return Pointer to the data, or NULL if vector is empty
 */
void* cvector_data(const CVector *vec);

/**
 * @brief Shrinks the vector's capacity to match its size
 * @param vec Pointer to the vector
 * @return 0 on success, non-zero on failure
 */
int cvector_shrink_to_fit(CVector *vec);

#ifdef __cplusplus
}
#endif

#endif /* CVECTOR_H */
