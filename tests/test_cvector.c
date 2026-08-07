#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "cvector.h"

int test_create_destroy(void) {
    CVector *vec = cvector_create(sizeof(int));
    assert(vec != NULL);
    assert(cvector_size(vec) == 0);
    assert(cvector_is_empty(vec));
    cvector_destroy(vec);
    printf("? test_create_destroy passed\n");
    return 0;
}

int test_push_back_pop_back(void) {
    CVector *vec = cvector_create(sizeof(int));
    
    int values[] = {10, 20, 30, 40, 50};
    for (int i = 0; i < 5; i++) {
        assert(cvector_push_back(vec, &values[i]) == 0);
    }
    
    assert(cvector_size(vec) == 5);
    
    int popped;
    for (int i = 4; i >= 0; i--) {
        assert(cvector_pop_back(vec, &popped) == 0);
        assert(popped == values[i]);
    }
    
    assert(cvector_is_empty(vec));
    cvector_destroy(vec);
    printf("? test_push_back_pop_back passed\n");
    return 0;
}

int test_at_front_back(void) {
    CVector *vec = cvector_create(sizeof(int));
    
    int values[] = {100, 200, 300};
    for (int i = 0; i < 3; i++) {
        cvector_push_back(vec, &values[i]);
    }
    
    int *front = (int *)cvector_front(vec);
    assert(*front == 100);
    
    int *back = (int *)cvector_back(vec);
    assert(*back == 300);
    
    int *middle = (int *)cvector_at(vec, 1);
    assert(*middle == 200);
    
    cvector_destroy(vec);
    printf("? test_at_front_back passed\n");
    return 0;
}

int test_reserve_capacity(void) {
    CVector *vec = cvector_create(sizeof(double));
    
    size_t initial_capacity = cvector_capacity(vec);
    assert(cvector_reserve(vec, 100) == 0);
    assert(cvector_capacity(vec) >= 100);
    assert(cvector_size(vec) == 0);
    
    cvector_destroy(vec);
    printf("? test_reserve_capacity passed\n");
    return 0;
}

int test_resize(void) {
    CVector *vec = cvector_create(sizeof(int));
    
    assert(cvector_resize(vec, 5) == 0);
    assert(cvector_size(vec) == 5);
    
    int *data = (int *)cvector_data(vec);
    for (int i = 0; i < 5; i++) {
        assert(data[i] == 0);
    }
    
    assert(cvector_resize(vec, 3) == 0);
    assert(cvector_size(vec) == 3);
    
    cvector_destroy(vec);
    printf("? test_resize passed\n");
    return 0;
}

int test_insert_erase(void) {
    CVector *vec = cvector_create(sizeof(int));
    
    int values[] = {1, 2, 4, 5};
    for (int i = 0; i < 4; i++) {
        cvector_push_back(vec, &values[i]);
    }
    
    int insert_val = 3;
    assert(cvector_insert(vec, 2, &insert_val) == 0);
    assert(cvector_size(vec) == 5);
    
    int *elem = (int *)cvector_at(vec, 2);
    assert(*elem == 3);
    
    assert(cvector_erase(vec, 2) == 0);
    assert(cvector_size(vec) == 4);
    
    cvector_destroy(vec);
    printf("? test_insert_erase passed\n");
    return 0;
}

int test_clear(void) {
    CVector *vec = cvector_create(sizeof(char));
    
    for (int i = 0; i < 10; i++) {
        char c = 'a' + i;
        cvector_push_back(vec, &c);
    }
    
    assert(cvector_size(vec) == 10);
    cvector_clear(vec);
    assert(cvector_size(vec) == 0);
    assert(cvector_is_empty(vec));
    
    cvector_destroy(vec);
    printf("? test_clear passed\n");
    return 0;
}

int test_shrink_to_fit(void) {
    CVector *vec = cvector_create(sizeof(int));
    
    for (int i = 0; i < 5; i++) {
        cvector_push_back(vec, &i);
    }
    
    assert(cvector_capacity(vec) > cvector_size(vec));
    assert(cvector_shrink_to_fit(vec) == 0);
    assert(cvector_capacity(vec) == cvector_size(vec));
    
    cvector_destroy(vec);
    printf("? test_shrink_to_fit passed\n");
    return 0;
}

int test_struct_storage(void) {
    typedef struct {
        int x;
        int y;
        char name[50];
    } Point;
    
    CVector *vec = cvector_create(sizeof(Point));
    
    Point p1 = {10, 20, "Point1"};
    Point p2 = {30, 40, "Point2"};
    
    cvector_push_back(vec, &p1);
    cvector_push_back(vec, &p2);
    
    Point *stored = (Point *)cvector_at(vec, 0);
    assert(stored->x == 10);
    assert(stored->y == 20);
    assert(strcmp(stored->name, "Point1") == 0);
    
    cvector_destroy(vec);
    printf("? test_struct_storage passed\n");
    return 0;
}

int test_invalid_arguments_and_empty_vector(void) {
    int value = 42;

    assert(cvector_create(0) == NULL);
    assert(cvector_create_with_capacity(sizeof(int), 0) == NULL);
    cvector_destroy(NULL);
    assert(cvector_size(NULL) == 0);
    assert(cvector_capacity(NULL) == 0);
    assert(cvector_is_empty(NULL));
    assert(cvector_data(NULL) == NULL);
    assert(cvector_at(NULL, 0) == NULL);
    assert(cvector_front(NULL) == NULL);
    assert(cvector_back(NULL) == NULL);
    assert(cvector_push_back(NULL, &value) == -1);
    assert(cvector_pop_back(NULL, &value) == -1);
    assert(cvector_reserve(NULL, 20) == -1);
    assert(cvector_resize(NULL, 1) == -1);
    cvector_clear(NULL);
    assert(cvector_insert(NULL, 0, &value) == -1);
    assert(cvector_erase(NULL, 0) == -1);
    assert(cvector_shrink_to_fit(NULL) == 0);

    CVector *vec = cvector_create(sizeof(int));
    assert(vec != NULL);
    assert(cvector_push_back(vec, NULL) == -1);
    assert(cvector_pop_back(vec, &value) == -1);
    assert(cvector_at(vec, 0) == NULL);
    assert(cvector_front(vec) == NULL);
    assert(cvector_back(vec) == NULL);
    assert(cvector_insert(vec, 1, &value) == -1);
    assert(cvector_insert(vec, 0, NULL) == -1);
    assert(cvector_erase(vec, 0) == -1);
    cvector_destroy(vec);

    printf("? test_invalid_arguments_and_empty_vector passed\n");
    return 0;
}

int test_growth_and_boundary_modifications(void) {
    CVector *vec = cvector_create_with_capacity(sizeof(int), 1);
    assert(vec != NULL);

    int first = 10;
    int second = 20;
    int zero = 0;
    assert(cvector_push_back(vec, &first) == 0);
    assert(cvector_push_back(vec, &second) == 0);
    assert(cvector_capacity(vec) > 1);

    assert(cvector_insert(vec, 0, &zero) == 0);
    assert(*(int *)cvector_front(vec) == 0);
    assert(cvector_erase(vec, cvector_size(vec) - 1) == 0);
    assert(*(int *)cvector_back(vec) == 10);
    assert(cvector_at(vec, cvector_size(vec)) == NULL);

    assert(cvector_reserve(vec, cvector_capacity(vec)) == 0);
    assert(cvector_pop_back(vec, NULL) == 0);
    assert(cvector_pop_back(vec, NULL) == 0);
    assert(cvector_is_empty(vec));
    assert(cvector_shrink_to_fit(vec) == 0);
    assert(cvector_capacity(vec) == 1);

    cvector_destroy(vec);
    printf("? test_growth_and_boundary_modifications passed\n");
    return 0;
}

int main(void) {
    printf("Running CVector tests...\n\n");
    
    if (test_create_destroy() != 0) return 1;
    if (test_push_back_pop_back() != 0) return 1;
    if (test_at_front_back() != 0) return 1;
    if (test_reserve_capacity() != 0) return 1;
    if (test_resize() != 0) return 1;
    if (test_insert_erase() != 0) return 1;
    if (test_clear() != 0) return 1;
    if (test_shrink_to_fit() != 0) return 1;
    if (test_struct_storage() != 0) return 1;
    if (test_invalid_arguments_and_empty_vector() != 0) return 1;
    if (test_growth_and_boundary_modifications() != 0) return 1;
    
    printf("\n? All tests passed!\n");
    return 0;
}
