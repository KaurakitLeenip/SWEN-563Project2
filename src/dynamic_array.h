#ifndef DYNAMIC_ARRAY_H

#define ARRAY_INITIAL_CAPACITY 100

typedef struct {
    int size; // slots used so far
    int capacity; // total slots available
    int *data; // array of integers
}Vector;

typedef struct {
    int size; // slots used so far
    int capacity; // total slots available
    double *data; // array of integers
}Vector_double;

typedef struct {
    int size; // slots used so far
    int capacity; // total slots available
    char *data; // array of integers
}Vector_char;

void vector_init(Vector *vector, size_t itemSize);

void vector_append_int(Vector *vector, int value);

void vector_append_double(Vector *vector, double value);

void vector_append_char(Vector *vector, char value);

int vector_get_int(Vector *vector, int index);

double vector_get_double(Vector *vector, int index);

char vector_get_char(Vector *vector, int index);

void vector_set_int(Vector *vector, int index, int value);

void vector_set_double(Vector *vector, int index, double value);

void vector_set_char(Vector *vector, int index, char value);

void vector_double_capacity_if_full(Vector *vector, size_t itemSize);

void vector_free(Vector *vector);

#endif
