#include "types.h"

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>

void vector_init(Vector *vec){
    vec->size = 0;
    vec->capacity = 4;  
    vec->data = (double *)malloc(sizeof(double) * vec->capacity);
    if (vec->data == NULL) {
        fprintf(stderr, "Failed to allocate memory\n");
        exit(EXIT_FAILURE);
    } else {
        memset(vec->data, 0, sizeof(double) * vec->capacity);
    }
}

void vector_add(Vector *vec, double value){
  if (vec->size >= vec->capacity) {
        vec->capacity *= 2;
        vec->data = (double *)realloc(vec->data, sizeof(double) * vec->capacity);
        if (vec->data == NULL) {
            fprintf(stderr, "Failed to reallocate memory\n");
            exit(EXIT_FAILURE);
        }
    }
    vec->data[vec->size++] = value;
}

size_t vector_size(const Vector *vec){
  return vec->size;
}

double vector_get(const Vector *vec, size_t index){
    if (index >= vec->size) {
        fprintf(stderr, "Index out of bounds\n");
        exit(EXIT_FAILURE);
    }
    return vec->data[index];
}

void vector_free(Vector *vec){
    free(vec->data);
    vec->data = NULL;
    vec->size = 0;
    vec->capacity = 0;
}

double get_max(Vector *vec){
    double max_value = 0;
    for(int i = 0; i < vec->size; i++){
        if (vec->data[i] > max_value){
            max_value = vec->data[i];
        }
    }
    return max_value;
}

double get_min(Vector *vec){
    
    double min_value = (double) LONG_MAX;
    for(int i = 0; i < vec->size; i++){
        if (vec->data[i] < min_value){
            min_value = vec->data[i];
        }
    }
    return min_value;
}

