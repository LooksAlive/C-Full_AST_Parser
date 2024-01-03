#ifndef GENERIC_VECTOR_H
#define GENERIC_VECTOR_H




#include <stdio.h>
#include <stdlib.h>

// Define a macro to create a generic dynamic array structure for a specific type
#define DEFINE_GENERIC_DYNAMIC_ARRAY(type, structName) \
    typedef struct {                      \
        type* data;                       \
        size_t size;                       \
        size_t capacity;                   \
    } structName; //type##DynamicArray;

// Define a macro to initialize a generic dynamic array for a specific type
#define INIT_GENERIC_DYNAMIC_ARRAY(arr, initialCapacity) \
    do {                                                \
        (arr).data = malloc((initialCapacity) * sizeof(*(arr).data)); \
        if (!(arr).data) {                               \
            perror("Memory allocation failed");         \
            exit(EXIT_FAILURE);                          \
        }                                                \
        (arr).size = 0;                                  \
        (arr).capacity = (initialCapacity);              \
    } while (0)

// Define a macro to push a value to a generic dynamic array for a specific type
#define PUSH_GENERIC_ELEMENT(arr, value) \
    do {                                 \
        if ((arr).size == (arr).capacity) { \
            (arr).capacity *= 2;            \
            (arr).data = realloc((arr).data, (arr).capacity * sizeof(*(arr).data)); \
            if (!(arr).data) {              \
                perror("Memory reallocation failed"); \
                exit(EXIT_FAILURE);          \
            }                              \
        }                                  \
        (arr).data[(arr).size++] = (value); \
    } while (0)

// Define a macro to free a generic dynamic array for a specific type
#define FREE_GENERIC_DYNAMIC_ARRAY(arr) \
    do {                                \
        free((arr).data);               \
        (arr).data = NULL;              \
        (arr).size = 0;                 \
        (arr).capacity = 0;             \
    } while (0)


/*
DEFINE_GENERIC_DYNAMIC_ARRAY(char, my_num_struct);
my_num_struct numbers;

INIT_GENERIC_DYNAMIC_ARRAY(numbers, 2);

PUSH_GENERIC_ELEMENT(numbers, 42);
PUSH_GENERIC_ELEMENT(numbers, 17);
PUSH_GENERIC_ELEMENT(numbers, 99);

// Access elements directly
printf("Element at index 1: %d\n", numbers.data[1]);

FREE_GENERIC_DYNAMIC_ARRAY(numbers);
*/





// Push an element to a generic dynamic array
#define PUSH_BACK(arr, typename, element)                          \
    do {                                                            \
        if ((arr).size == (arr).capacity) {                         \
            (arr).capacity *= 2;                                    \
            (arr).data = (typeof((arr).data))realloc((arr).data, (arr).capacity * sizeof(*(arr).data)); \
            if (!(arr).data) {                                      \
                fprintf(stderr, "Memory allocation failed.\n");    \
                exit(EXIT_FAILURE);                                 \
            }                                                       \
        }                                                           \
        (arr).data[(arr).size++] = (element);                       \
    } while (0)









#endif // GENERIC_VECTOR_H
