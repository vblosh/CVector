#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cvector.h"

int main(void) {
    printf("=== CVector String Example ===\n\n");
    
    // Create a vector to store strings (char pointers)
    CVector *strings = cvector_create(sizeof(char*));
    
    // Add some strings
    const char *fruit[] = {"apple", "banana", "cherry", "date", "elderberry"};
    
    printf("Adding fruits:\n");
    for (int i = 0; i < 5; i++) {
        // Make a copy of the string
        char *copy = (char *)malloc(strlen(fruit[i]) + 1);
        strcpy(copy, fruit[i]);
        cvector_push_back(strings, &copy);
        printf("  Added: %s\n", fruit[i]);
    }
    printf("\n");
    
    // Print all strings
    printf("All fruits in vector:\n");
    for (size_t i = 0; i < cvector_size(strings); i++) {
        char **str_ptr = (char **)cvector_at(strings, i);
        printf("  [%zu]: %s\n", i, *str_ptr);
    }
    printf("\n");
    
    // Insert a new string
    printf("Inserting 'fig' at position 3...\n");
    char *new_fruit = (char *)malloc(4);
    strcpy(new_fruit, "fig");
    cvector_insert(strings, 3, &new_fruit);
    printf("\n");
    
    // Print updated list
    printf("Updated fruits:\n");
    for (size_t i = 0; i < cvector_size(strings); i++) {
        char **str_ptr = (char **)cvector_at(strings, i);
        printf("  [%zu]: %s\n", i, *str_ptr);
    }
    printf("\n");
    
    // Cleanup - free all allocated strings
    for (size_t i = 0; i < cvector_size(strings); i++) {
        char **str_ptr = (char **)cvector_at(strings, i);
        free(*str_ptr);
    }
    cvector_destroy(strings);
    
    printf("Vector destroyed.\n");
    return 0;
}
