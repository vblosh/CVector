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

static int element_offset(const CVector *vec, const void *element,
                          size_t *offset) {
    size_t bytes;
    if (allocation_size(vec->size, vec->element_size, &bytes) != 0) {
        return 0;
    }
    uintptr_t start = (uintptr_t)vec->data;
    uintptr_t address = (uintptr_t)element;
    if (address < start || address - start >= bytes) {
        return 0;
    }
    *offset = (size_t)(address - start);
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

void* cvector_at(const CVector *vec, size_t index) {
    if (vec == NULL || index >= vec->size) {
        return NULL;
    }

    return (uint8_t *)vec->data + (index * vec->element_size);
}

void* cvector_front(const CVector *vec) {
    return cvector_at(vec, 0);
}

void* cvector_back(const CVector *vec) {
    if (vec == NULL || vec->size == 0) {
        return NULL;
    }

    return cvector_at(vec, vec->size - 1);
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
        if (cvector_reserve(vec, new_size) != 0) {
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

void* cvector_data(const CVector *vec) {
    return (vec != NULL) ? vec->data : NULL;
}

int cvector_shrink_to_fit(CVector *vec) {
    if (vec == NULL || vec->size == vec->capacity) {
        return 0;
    }

    if (vec->size == 0) {
        void *new_data = malloc(vec->element_size);
        if (new_data == NULL) {
            return -1;
        }
        free(vec->data);
        vec->data = new_data;
        vec->capacity = 1;
    } else {
        void *new_data = realloc(vec->data, vec->size * vec->element_size);
        if (new_data == NULL) {
            return -1;
        }
        vec->data = new_data;
        vec->capacity = vec->size;
    }

    return 0;
}
