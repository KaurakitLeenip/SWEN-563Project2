#include <stdio.h>
#include <stdlib.h>
#include "dynamic_array.h"

/*
 * Initializes the vector by setting it's size to 0 and it's capacity to the
 * initial capacity specified by the header file.  Also allocates memory for the
 * vector's items by callocing
*/
void vector_init(Vector *vector, size_t itemSize){
    // initialize size and capacity
    vector->size = 0;
    vector->capacity = ARRAY_INITIAL_CAPACITY;
    
    // allocate memory for vector data
    vector->data = calloc(vector->capacity, itemSize * vector->capacity);
}

/*
 * Appends an item to the end of the vector.  Has implementations for int, double, and char.  These are
 * all seperate functions as C does not (easily) support function overloading, although c11 does have 
 * some form of overloading through generics support it's more difficult to implement than just using
 * seperate functions.
*/
void vector_append_int(Vector *vector, int value){
    // make sure there's room to expand
    vector_double_capacity_if_full(vector, sizeof(int));
    
    // append the value and increment the vector size
    vector->data[vector->size++] = value;
}

void vector_append_double(Vector *vector, double value){
    // make sure there's room to expand
    vector_double_capacity_if_full(vector, sizeof(double));
    
    // append the value and increment the vector size
    vector->data[vector->size++] = value;
}

void vector_append_char(Vector *vector, char value){
    // make sure there's room to expand
    vector_double_capacity_if_full(vector, sizeof(char));
    
    // append the value and increment the vector size
    vector->data[vector->size++] = value;
}

/*
 * Gets the item in the vector at the specified index.  Seperate functions for getting an int, double,
 * or char item.  Notifies and exits if the index is out of bounds.
*/
int vector_get_int(Vector *vector, int index){
    if(index >= vector->size || index < 0){
        printf("Index %d out of bounds for vector of size %d\n", index, vector->size);
        exit(1);
    }
    
    return vector->data[index];
}

double vector_get_double(Vector *vector, int index){
    if(index >= vector->size || index < 0){
        printf("Index %d out of bounds for vector of size %d\n", index, vector->size);
        exit(1);
    }
    
    return vector->data[index];
}

char vector_get_char(Vector *vector, int index){
    if(index >= vector->size || index < 0){
        printf("Index %d out of bounds for vector of size %d\n", index, vector->size);
        exit(1);
    }
    
    return vector->data[index];
}

/*
 *  Sets the item at the given index to the specified value.  Seperate functions for setting an int, double,
 *  or char item.  Fills the vector with 0s if the index is out of bounds
*/
void vector_set_int(Vector *vector, int index, int value){
    // if the desired slot is out of range, then appends 0s until filled
    // to the desired index
    while(index >= vector->size){
        vector_append_int(vector, 0);
    }
    
    vector->data[index] = value;
}

void vector_set_double(Vector *vector, int index, double value){
    // if the desired slot is out of range, then appends 0s until filled
    // to the desired index
    while(index >= vector->size){
        vector_append_double(vector, 0);
    }
    
    vector->data[index] = value;
}
void vector_set_char(Vector *vector, int index, char value){
    // if the desired slot is out of range, then appends 0s until filled
    // to the desired index
    while(index >= vector->size){
        vector_append_char(vector, 0);
    }
    
    vector->data[index] = value;
}
/*
 * Doubles vector capcity if it's full.  Checks if the current vector size is greater than or equal to
 * the vector capacity, and if so, doubles the capacity and reallocs the memory
*/
void vector_double_capacity_if_full(Vector *vector, size_t itemSize){
    if(vector->size >= vector->capacity){
        // double vector capacity and resize allocated memory
        vector->capacity *= 2;
        vector->data = realloc(vector->data, itemSize * vector->capacity);
    }
}

/*
 *  Frees vector memory when done
*/
void vector_free(Vector *vector){
    free(vector->data);
}