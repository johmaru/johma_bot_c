#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>


typedef struct {
    void** data;
    size_t size;
    size_t capacity;
} Vector;

Vector* vector_create(void);

void vector_push(Vector* vec, void* data);

void* vector_get(Vector* vec, size_t index);
void vector_free(Vector* vec);

#endif  // VECTOR_H