#include <stddef.h>

typedef struct {
    void** data;
    size_t length;
    size_t capacity;
} vec_t;

vec_t* vec_create(size_t capacity);
void vec_destroy(vec_t* v);
int vec_push(vec_t* v, void* item);
void* vec_pop(vec_t* v);
void* vec_get(vec_t* v, size_t idx);
int vec_set(vec_t* v, size_t idx, void* item);
int vec_insert(vec_t* v, size_t idx, void* item);
void* vec_delete(vec_t* v, size_t idx);
int vec_resize(vec_t* v);
void vec_delete_item(vec_t* v, void* item);
typedef void (*print_func)(void*);
void vec_print(vec_t* v, print_func p);