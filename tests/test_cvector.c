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
    
    printf("\n? All tests passed!\n");
    return 0;
}
