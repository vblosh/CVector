#include <stdio.h>
#include <stdlib.h>
#include "cvector.h"

int main(void) {
    printf("=== CVector Basic Example ===\n\n");
    
    // Create a vector of integers
    CVector *numbers = cvector_create(sizeof(int));
    
    // Push some values
    printf("Adding numbers: ");
    for (int i = 1; i <= 5; i++) {
        cvector_push_back(numbers, &i);
        printf("%d ", i);
    }
    printf("\n\n");
    
    // Print vector info
    printf("Vector size: %zu\n", cvector_size(numbers));
    printf("Vector capacity: %zu\n\n", cvector_capacity(numbers));
    
    // Access and print elements
    printf("Accessing elements:\n");
    for (size_t i = 0; i < cvector_size(numbers); i++) {
        int *val = (int *)cvector_at(numbers, i);
        printf("  Element[%zu] = %d\n", i, *val);
    }
    printf("\n");
    
    // Access first and last
    printf("First element: %d\n", *(int *)cvector_front(numbers));
    printf("Last element: %d\n\n", *(int *)cvector_back(numbers));
    
    // Pop elements
    printf("Popping last element...\n");
    int popped;
    cvector_pop_back(numbers, &popped);
    printf("Popped value: %d\n", popped);
    printf("New size: %zu\n\n", cvector_size(numbers));
    
    // Insert an element
    printf("Inserting 99 at position 1...\n");
    int insert_val = 99;
    cvector_insert(numbers, 1, &insert_val);
    printf("Elements after insert: ");
    for (size_t i = 0; i < cvector_size(numbers); i++) {
        printf("%d ", *(int *)cvector_at(numbers, i));
    }
    printf("\n\n");
    
    // Cleanup
    cvector_destroy(numbers);
    printf("Vector destroyed.\n");
    
    return 0;
}
