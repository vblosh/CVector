#include "cvector.h"
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 10
#define GROWTH_FACTOR 1.5f

CVector* cvector_create(size_t element_size) {
    return cvector_create_with_capacity(element_size, INITIAL_CAPACITY);
}

CVector* cvector_create_with_capacity(size_t element_size, size_t capacity) {
    if (element_size == 0 || capacity == 0) {
        return NULL;
    }

    CVector *vec = (CVector *)malloc(sizeof(CVector));
    if (vec == NULL) {
        return NULL;
    }

    vec->data = malloc(element_size * capacity);
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
        size_t new_capacity = (size_t)(vec->capacity * GROWTH_FACTOR);
        if (new_capacity <= vec->capacity) {
            new_capacity = vec->capacity + 1;
        }

        if (cvector_reserve(vec, new_capacity) != 0) {
            return -1;
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
        memcpy(out_element, src, vec->element_size);
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
    if (vec == NULL) {
        return -1;
    }

    if (new_capacity <= vec->capacity) {
        return 0;
    }

    void *new_data = realloc(vec->data, new_capacity * vec->element_size);
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
    if (vec == NULL || element == NULL || index > vec->size) {
        return -1;
    }

    if (vec->size >= vec->capacity) {
        size_t new_capacity = (size_t)(vec->capacity * GROWTH_FACTOR);
        if (new_capacity <= vec->capacity) {
            new_capacity = vec->capacity + 1;
        }

        if (cvector_reserve(vec, new_capacity) != 0) {
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
