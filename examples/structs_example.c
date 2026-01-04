#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cvector.h"

typedef struct {
    int id;
    float score;
    char name[50];
} Student;

void print_student(const Student *s) {
    printf("  ID: %d, Name: %-20s, Score: %.2f\n", s->id, s->name, s->score);
}

int main(void) {
    printf("=== CVector Struct Example ===\n\n");
    
    // Create a vector of Student structures
    CVector *students = cvector_create(sizeof(Student));
    
    // Create some students
    Student s1 = {101, 85.5f, "Alice"};
    Student s2 = {102, 92.0f, "Bob"};
    Student s3 = {103, 78.5f, "Charlie"};
    Student s4 = {104, 88.0f, "Diana"};
    
    printf("Adding students:\n");
    cvector_push_back(students, &s1);
    print_student(&s1);
    cvector_push_back(students, &s2);
    print_student(&s2);
    cvector_push_back(students, &s3);
    print_student(&s3);
    cvector_push_back(students, &s4);
    print_student(&s4);
    printf("\n");
    
    // Print all students
    printf("All students:\n");
    for (size_t i = 0; i < cvector_size(students); i++) {
        Student *student = (Student *)cvector_at(students, i);
        print_student(student);
    }
    printf("\n");
    
    // Modify a student
    printf("Updating student at index 1...\n");
    Student *student_to_update = (Student *)cvector_at(students, 1);
    student_to_update->score = 95.0f;
    printf("Updated:\n");
    print_student(student_to_update);
    printf("\n");
    
    // Find students with score >= 85
    printf("Students with score >= 85.0:\n");
    for (size_t i = 0; i < cvector_size(students); i++) {
        Student *student = (Student *)cvector_at(students, i);
        if (student->score >= 85.0f) {
            print_student(student);
        }
    }
    printf("\n");
    
    // Get front and back
    printf("First student: %s\n", ((Student *)cvector_front(students))->name);
    printf("Last student: %s\n", ((Student *)cvector_back(students))->name);
    printf("\n");
    
    // Cleanup
    cvector_destroy(students);
    printf("Vector destroyed.\n");
    
    return 0;
}
