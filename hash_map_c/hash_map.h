#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef HM_LOAD_FACTOR
#define HM_LOAD_FACTOR 0.7f
#endif
typedef struct entry_t {
    const char*     key;
    void*           value;
    struct entry_t* next;
} entry_t;

typedef struct {
    size_t size;
    size_t count;
    entry_t** buckets;
} HashMap;

HashMap* hm_create(size_t capacity);
void hm_destroy(HashMap* hm);
void hm_set(HashMap* hm, const char* key, void* value);
void* hm_get(HashMap* hm, const char* key);
int hm_delete(HashMap* hm, const char* key);
int hm_has(HashMap* hm, const char* key);
char** hm_keys(HashMap* hm);
void hm_resize(HashMap* hm);
typedef void (*print_func)(void*);

void hm_print(HashMap* hm, print_func p);