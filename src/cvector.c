#include "cvector.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define INITIAL_CAPACITY 10
static int allocation_size(size_t count, size_t element_size, size_t *bytes) {
    if (element_size != 0 && count > SIZE_MAX / element_size) {
        return -1;
    }
    *bytes = count * element_size;
    return 0;
}

static int next_capacity(size_t capacity, size_t *result) {
    size_t increment = capacity / 2;
    if (increment == 0) {
        increment = 1;
    }
    if (capacity > SIZE_MAX - increment) {
        return -1;
    }
    *result = capacity + increment;
    return 0;
}

static int grow_capacity(const CVector *vec, size_t required, size_t *result) {
    size_t capacity = vec->capacity;

    while (capacity < required) {
        size_t next;
        if (next_capacity(capacity, &next) != 0) {
            return -1;
        }
        capacity = next;
    }

    *result = capacity;
    return 0;
}

static int element_offset(const CVector *vec, const void *element,
                          size_t *offset) {
    size_t bytes;
    if (vec == NULL || vec->data == NULL || vec->element_size == 0) {
        return 0;
    }
    if (allocation_size(vec->capacity, vec->element_size, &bytes) != 0 ||
        bytes < vec->element_size) {
        return 0;
    }

    uintptr_t start = (uintptr_t)vec->data;
    uintptr_t address = (uintptr_t)element;
    if (address < start) {
        return 0;
    }

    uintptr_t difference = address - start;
    if (difference > bytes - vec->element_size ||
        difference % vec->element_size != 0) {
        return 0;
    }

    *offset = (size_t)difference;
    return 1;
}

CVector* cvector_create(size_t element_size) {
    return cvector_create_with_capacity(element_size, INITIAL_CAPACITY);
}

CVector* cvector_create_with_capacity(size_t element_size, size_t capacity) {
    size_t bytes;
    if (element_size == 0 || capacity == 0) {
        return NULL;
    }
    if (allocation_size(capacity, element_size, &bytes) != 0) {
        return NULL;
    }

    CVector *vec = (CVector *)malloc(sizeof(CVector));
    if (vec == NULL) {
        return NULL;
    }

    vec->data = malloc(bytes);
    if (vec->data == NULL) {
        free(vec);
        return NULL;
    }

    vec->element_size = element_size;
    vec->size = 0;
    vec->capacity = capacity;

    return vec;
}

void cvector_destroy(CVector *vec) {
    if (vec == NULL) {
        return;
    }

    if (vec->data != NULL) {
        free(vec->data);
    }
    free(vec);
}

size_t cvector_size(const CVector *vec) {
    return (vec != NULL) ? vec->size : 0;
}

size_t cvector_capacity(const CVector *vec) {
    return (vec != NULL) ? vec->capacity : 0;
}

int cvector_is_empty(const CVector *vec) {
    return (vec == NULL) || (vec->size == 0);
}

int cvector_push_back(CVector *vec, const void *element) {
    if (vec == NULL || element == NULL) {
        return -1;
    }

    if (vec->size >= vec->capacity) {
        size_t new_capacity;
        size_t offset;
        int aliases_data = element_offset(vec, element, &offset);

        if (next_capacity(vec->capacity, &new_capacity) != 0 ||
            cvector_reserve(vec, new_capacity) != 0) {
            return -1;
        }
        if (aliases_data) {
            element = (uint8_t *)vec->data + offset;
        }
    }

    void *dest = (uint8_t *)vec->data + (vec->size * vec->element_size);
    memcpy(dest, element, vec->element_size);
    vec->size++;

    return 0;
}

int cvector_pop_back(CVector *vec, void *out_element) {
    if (vec == NULL || vec->size == 0) {
        return -1;
    }

    vec->size--;
    if (out_element != NULL) {
        void *src = (uint8_t *)vec->data + (vec->size * vec->element_size);
        memmove(out_element, src, vec->element_size);
    }

    return 0;
}

void* cvector_at(CVector *vec, size_t index) {
    if (vec == NULL || index >= vec->size) {
        return NULL;
    }

    return (uint8_t *)vec->data + (index * vec->element_size);
}

const void* cvector_at_const(const CVector *vec, size_t index) {
    if (vec == NULL || index >= vec->size) {
        return NULL;
    }

    return (const uint8_t *)vec->data + (index * vec->element_size);
}

void* cvector_front(CVector *vec) {
    return cvector_at(vec, 0);
}

const void* cvector_front_const(const CVector *vec) {
    return cvector_at_const(vec, 0);
}

void* cvector_back(CVector *vec) {
    if (vec == NULL || vec->size == 0) {
        return NULL;
    }

    return cvector_at(vec, vec->size - 1);
}

const void* cvector_back_const(const CVector *vec) {
    if (vec == NULL || vec->size == 0) {
        return NULL;
    }

    return cvector_at_const(vec, vec->size - 1);
}

int cvector_reserve(CVector *vec, size_t new_capacity) {
    size_t bytes;
    if (vec == NULL) {
        return -1;
    }

    if (new_capacity <= vec->capacity) {
        return 0;
    }
    if (allocation_size(new_capacity, vec->element_size, &bytes) != 0) {
        return -1;
    }

    void *new_data = realloc(vec->data, bytes);
    if (new_data == NULL) {
        return -1;
    }

    vec->data = new_data;
    vec->capacity = new_capacity;

    return 0;
}

int cvector_resize(CVector *vec, size_t new_size) {
    if (vec == NULL) {
        return -1;
    }

    if (new_size > vec->capacity) {
        size_t new_capacity;
        if (grow_capacity(vec, new_size, &new_capacity) != 0 ||
            cvector_reserve(vec, new_capacity) != 0) {
            return -1;
        }
    }

    if (new_size > vec->size) {
        size_t diff = new_size - vec->size;
        void *dest = (uint8_t *)vec->data + (vec->size * vec->element_size);
        memset(dest, 0, diff * vec->element_size);
    }

    vec->size = new_size;
    return 0;
}

void cvector_clear(CVector *vec) {
    if (vec != NULL) {
        vec->size = 0;
    }
}

int cvector_insert(CVector *vec, size_t index, const void *element) {
    void *element_copy = NULL;
    if (vec == NULL || element == NULL || index > vec->size) {
        return -1;
    }

    size_t offset;
    if (element_offset(vec, element, &offset)) {
        element_copy = malloc(vec->element_size);
        if (element_copy == NULL) {
            return -1;
        }
        memcpy(element_copy, element, vec->element_size);
        element = element_copy;
    }

    if (vec->size >= vec->capacity) {
        size_t new_capacity;

        if (next_capacity(vec->capacity, &new_capacity) != 0 ||
            cvector_reserve(vec, new_capacity) != 0) {
            free(element_copy);
            return -1;
        }
    }

    if (index < vec->size) {
        void *src = (uint8_t *)vec->data + (index * vec->element_size);
        void *dst = (uint8_t *)vec->data + ((index + 1) * vec->element_size);
        size_t bytes_to_move = (vec->size - index) * vec->element_size;
        memmove(dst, src, bytes_to_move);
    }

    void *dest = (uint8_t *)vec->data + (index * vec->element_size);
    memcpy(dest, element, vec->element_size);
    vec->size++;
    free(element_copy);

    return 0;
}

int cvector_erase(CVector *vec, size_t index) {
    if (vec == NULL || index >= vec->size) {
        return -1;
    }

    if (index < vec->size - 1) {
        void *dst = (uint8_t *)vec->data + (index * vec->element_size);
        void *src = (uint8_t *)vec->data + ((index + 1) * vec->element_size);
        size_t bytes_to_move = (vec->size - index - 1) * vec->element_size;
        memmove(dst, src, bytes_to_move);
    }

    vec->size--;
    return 0;
}

int cvector_insert_n(CVector *vec, size_t index, const void *element,
                     size_t count) {
    if (vec == NULL || (element == NULL && count != 0) ||
        index > vec->size) {
        return -1;
    }
    if (count == 0) {
        return 0;
    }
    if (vec->size > SIZE_MAX - count) {
        return -1;
    }

    size_t new_size = vec->size + count;
    size_t bytes;
    if (allocation_size(new_size, vec->element_size, &bytes) != 0) {
        return -1;
    }

    void *element_copy = NULL;
    size_t offset;
    if (element_offset(vec, element, &offset)) {
        element_copy = malloc(vec->element_size);
        if (element_copy == NULL) {
            return -1;
        }
        memcpy(element_copy, element, vec->element_size);
        element = element_copy;
    }

    if (new_size > vec->capacity) {
        size_t new_capacity;
        if (grow_capacity(vec, new_size, &new_capacity) != 0 ||
            cvector_reserve(vec, new_capacity) != 0) {
            free(element_copy);
            return -1;
        }
    }

    size_t element_size = vec->element_size;
    uint8_t *data = (uint8_t *)vec->data;
    if (index < vec->size) {
        void *src = data + (index * element_size);
        void *dst = data + ((index + count) * element_size);
        size_t bytes_to_move = (vec->size - index) * element_size;
        memmove(dst, src, bytes_to_move);
    }
    for (size_t i = 0; i < count; ++i) {
        memcpy(data + ((index + i) * element_size), element, element_size);
    }

    vec->size = new_size;
    free(element_copy);
    return 0;
}

int cvector_insert_array(CVector *vec, size_t index, const void *elements,
                         size_t count) {
    if (vec == NULL || (elements == NULL && count != 0) ||
        index > vec->size) {
        return -1;
    }
    if (count == 0) {
        return 0;
    }
    if (vec->size > SIZE_MAX - count) {
        return -1;
    }

    size_t new_size = vec->size + count;
    size_t bytes;
    size_t source_bytes;
    if (allocation_size(new_size, vec->element_size, &bytes) != 0 ||
        allocation_size(count, vec->element_size, &source_bytes) != 0) {
        return -1;
    }

    void *source_copy = NULL;
    size_t storage_bytes;
    if (allocation_size(vec->capacity, vec->element_size, &storage_bytes) == 0) {
        uintptr_t source = (uintptr_t)elements;
        uintptr_t storage = (uintptr_t)vec->data;
        if (source <= UINTPTR_MAX - source_bytes &&
            storage <= UINTPTR_MAX - storage_bytes) {
            uintptr_t source_end = source + source_bytes;
            uintptr_t storage_end = storage + storage_bytes;
            if (source < storage_end && storage < source_end) {
                source_copy = malloc(source_bytes);
                if (source_copy == NULL) {
                    return -1;
                }
                memcpy(source_copy, elements, source_bytes);
                elements = source_copy;
            }
        }
    }

    if (new_size > vec->capacity) {
        size_t new_capacity;
        if (grow_capacity(vec, new_size, &new_capacity) != 0 ||
            cvector_reserve(vec, new_capacity) != 0) {
            free(source_copy);
            return -1;
        }
    }

    size_t element_size = vec->element_size;
    uint8_t *data = (uint8_t *)vec->data;
    if (index < vec->size) {
        void *src = data + (index * element_size);
        void *dst = data + ((index + count) * element_size);
        size_t bytes_to_move = (vec->size - index) * element_size;
        memmove(dst, src, bytes_to_move);
    }
    memcpy(data + (index * element_size), elements, source_bytes);

    vec->size = new_size;
    free(source_copy);
    return 0;
}

int cvector_erase_range(CVector *vec, size_t index, size_t count) {
    if (vec == NULL || index > vec->size || count > vec->size - index) {
        return -1;
    }
    if (count == 0) {
        return 0;
    }

    size_t element_size = vec->element_size;
    uint8_t *data = (uint8_t *)vec->data;
    void *dst = data + (index * element_size);
    void *src = data + ((index + count) * element_size);
    size_t bytes_to_move = (vec->size - index - count) * element_size;
    memmove(dst, src, bytes_to_move);

    vec->size -= count;
    return 0;
}

void* cvector_data(CVector *vec) {
    return (vec != NULL) ? vec->data : NULL;
}

const void* cvector_data_const(const CVector *vec) {
    return (vec != NULL) ? vec->data : NULL;
}

int cvector_shrink_to_fit(CVector *vec) {
    if (vec == NULL) {
        return -1;
    }

    if (vec->size == 0) {
        if (vec->capacity == 1 && vec->data != NULL) {
            return 0;
        }

        void *new_data = malloc(vec->element_size);
        if (new_data == NULL) {
            return -1;
        }
        free(vec->data);
        vec->data = new_data;
        vec->capacity = 1;
        return 0;
    }

    if (vec->size == vec->capacity) {
        return 0;
    }

    void *new_data = realloc(vec->data, vec->size * vec->element_size);
    if (new_data == NULL) {
        return -1;
    }
    vec->data = new_data;
    vec->capacity = vec->size;

    return 0;
}
