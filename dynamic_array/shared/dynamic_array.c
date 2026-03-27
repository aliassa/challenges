#include "dynamic_array.h"
#include <stdlib.h>

vec_t* vec_create(size_t capacity) {
    vec_t* vec = malloc(sizeof *vec);
    if(!vec) return NULL;

    vec->data = malloc(capacity * sizeof(void*));
    if(!vec->data) { free(vec); return NULL; }
    vec->capacity = capacity;
    vec->length = 0;
    return vec;
}

void vec_destroy(vec_t* v) {
    if(!v) return;
    free(v->data);
    free(v);
}

int vec_push(vec_t* v, void* item) {
    if(!v) return -1;

    if(v->length == v->capacity) {
        size_t new_cap = v->capacity ? v->capacity * 2 : 1; // handle capacity == 0
        void** new_data = realloc(v->data, v->capacity * 2 * sizeof(void*));
        if(!new_data) return -1;
        v->data = new_data;
        v->capacity = new_cap;
    }
    v->data[v->length++] = item;
    return 0;
}

void* vec_pop(vec_t* v) {
    if(!v || v->length == 0) return NULL;
    void* elt = v->data[v->length - 1];
    v->data[v->length - 1] = NULL;
    v->length--;
    return elt;
}

// Returns NULL if index is out of bounds
void* vec_get(vec_t* v, size_t idx) {
    if(!v || idx >= v->length) return NULL;
    return v->data[idx];
}

int vec_set(vec_t* v, size_t idx, void* item) {
    if(!v || idx >= v->length) return -1;

    v->data[idx] = item;
    return 0;
}

int vec_insert(vec_t* v, size_t idx, void* item) {
    if(!v || idx > v->length) return -1;
    // handle when vec is full
    if(v->length == v->capacity) {
        size_t new_cap = v->capacity ? v->capacity * 2 : 1; // handle capacity == 0
        void** new_data = realloc(v->data, new_cap * sizeof(void*));
        if(!new_data) return -1;
        v->data = new_data;
        v->capacity = new_cap;
    }
    
    for(size_t i = v->length; i > idx; i--) {
        v->data[i] = v->data[i-1];
    }
    v->data[idx] = item;
    v->length++;
    return 0;
}

void* vec_delete(vec_t* v, size_t idx) {
    if(!v || idx >= v->length) return NULL;

    void* item = v->data[idx];
    for(size_t i = idx; i < v->length - 1; i++) {
        v->data[i] = v->data[i+1];
    }
    v->data[v->length - 1] = NULL;
    v->length--;
    return item;
}

#include <stdio.h>

void vec_delete_item(vec_t* v, void* item) {
    
    if(!v || !v->data) return;
    for(size_t i = 0; i < v->length; i++) {
        if(v->data[i] == item) {
            // Shilft element left
            for(size_t j = i; j < v->length - 1; j++) {
                v->data[j] = v->data[j+1];
            }
            v->length--;

            v->data[v->length] = NULL;
            return;
        }
    }
}

int vec_resize(vec_t* v) {
    if(!v) return -1;
    size_t new_cap = v->capacity ? v->capacity * 2 : 1; // handle capacity == 0
    void** new_data = realloc(v->data, new_cap * sizeof(void*));
    if(!new_data) return -1;
    v->data = new_data;
    v->capacity = new_cap;
    return 0;
}

#include <stdio.h>
void vec_print(vec_t* v, print_func p) {
    if(!v) return;
    for(size_t i = 0; i < v->length; i++) {
        printf("[%zu] -> ", i);
        if(p) p(v->data[i]);
    }
    printf("\n");
}