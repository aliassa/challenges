#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "dynamic_array.h"

/* ── test framework ───────────────────────────────────────────────────── */

static int tests_run    = 0;
static int tests_passed = 0;

#define TEST(name) static void name(void)

#define RUN(name) do {                              \
    printf("  %-45s", #name);                       \
    tests_run++;                                    \
    name();                                         \
    tests_passed++;                                 \
    printf("PASS\n");                               \
} while(0)

#define ASSERT(cond) do {                                           \
    if (!(cond)) {                                                  \
        printf("FAIL\n  assertion failed: %s\n  %s:%d\n",          \
               #cond, __FILE__, __LINE__);                          \
        exit(1);                                                    \
    }                                                               \
} while(0)

/* ── helpers ──────────────────────────────────────────────────────────── */

static void print_int(void* value) {
    printf("%d", *(int*)value);
}

/* ── vec_create ───────────────────────────────────────────────────────── */

TEST(test_create_valid) {
    vec_t* v = vec_create(8);
    ASSERT(v != NULL);
    ASSERT(v->capacity == 8);
    ASSERT(v->length   == 0);
    ASSERT(v->data     != NULL);
    vec_destroy(v);
}

TEST(test_create_zero_capacity) {
    vec_t* v = vec_create(0);
    ASSERT(v != NULL);
}

TEST(test_create_capacity_one) {
    vec_t* v = vec_create(1);
    ASSERT(v != NULL);
    ASSERT(v->capacity == 1);
    vec_destroy(v);
}

/* ── vec_push ─────────────────────────────────────────────────────────── */

TEST(test_push_single) {
    vec_t* v = vec_create(4);
    int a = 42;
    int ret = vec_push(v, &a);
    ASSERT(ret == 0);
    ASSERT(v->length == 1);
    vec_destroy(v);
}

TEST(test_push_multiple) {
    vec_t* v = vec_create(4);
    int a = 1, b = 2, c = 3;
    vec_push(v, &a);
    vec_push(v, &b);
    vec_push(v, &c);
    ASSERT(v->length == 3);
    vec_destroy(v);
}

TEST(test_push_null_vec) {
    int a = 1;
    ASSERT(vec_push(NULL, &a) == -1);
}

TEST(test_push_triggers_resize) {
    vec_t* v = vec_create(2);
    int a = 1, b = 2, c = 3;
    vec_push(v, &a);
    vec_push(v, &b);
    /* third push must trigger resize */
    int ret = vec_push(v, &c);
    ASSERT(ret == 0);
    ASSERT(v->capacity >= 3);
    ASSERT(v->length   == 3);
    vec_destroy(v);
}

TEST(test_push_preserves_order) {
    vec_t* v = vec_create(4);
    int a = 10, b = 20, c = 30;
    vec_push(v, &a);
    vec_push(v, &b);
    vec_push(v, &c);
    ASSERT(*(int*)vec_get(v, 0) == 10);
    ASSERT(*(int*)vec_get(v, 1) == 20);
    ASSERT(*(int*)vec_get(v, 2) == 30);
    vec_destroy(v);
}

/* ── vec_get ──────────────────────────────────────────────────────────── */

TEST(test_get_valid_index) {
    vec_t* v = vec_create(4);
    int a = 99;
    vec_push(v, &a);
    int* result = vec_get(v, 0);
    ASSERT(result != NULL);
    ASSERT(*result == 99);
    vec_destroy(v);
}

TEST(test_get_out_of_bounds) {
    vec_t* v = vec_create(4);
    int a = 1;
    vec_push(v, &a);
    ASSERT(vec_get(v, 5) == NULL);
    vec_destroy(v);
}

TEST(test_get_null_vec) {
    ASSERT(vec_get(NULL, 0) == NULL);
}

TEST(test_get_empty_vec) {
    vec_t* v = vec_create(4);
    ASSERT(vec_get(v, 0) == NULL);
    vec_destroy(v);
}

/* ── vec_pop ──────────────────────────────────────────────────────────── */

TEST(test_pop_returns_last) {
    vec_t* v = vec_create(4);
    int a = 1, b = 2;
    vec_push(v, &a);
    vec_push(v, &b);
    int* result = vec_pop(v);
    ASSERT(result != NULL);
    ASSERT(*result == 2);
    ASSERT(v->length == 1);
    vec_destroy(v);
    }
    
TEST(test_pop_empty_vec) {
    vec_t* v = vec_create(4);
    ASSERT(vec_pop(v) == NULL);
    vec_destroy(v);
    }
    
    TEST(test_pop_null_vec) {
    ASSERT(vec_pop(NULL) == NULL);
    }

    TEST(test_pop_until_empty) {
    vec_t* v = vec_create(4);
    int a = 1, b = 2, c = 3;
    vec_push(v, &a);
    vec_push(v, &b);
    vec_push(v, &c);
    vec_pop(v);
    vec_pop(v);
    vec_pop(v);
    ASSERT(v->length == 0);
    ASSERT(vec_pop(v) == NULL);
    vec_destroy(v);
}

/* ── vec_set ──────────────────────────────────────────────────────────── */

TEST(test_set_valid_index) {
    vec_t* v = vec_create(4);
    int a = 1, b = 99;
    vec_push(v, &a);
    int ret = vec_set(v, 0, &b);
    ASSERT(ret == 0);
    ASSERT(*(int*)vec_get(v, 0) == 99);
    vec_destroy(v);
    }

TEST(test_set_out_of_bounds) {
    vec_t* v = vec_create(4);
    int a = 1;
    ASSERT(vec_set(v, 5, &a) == -1);
    vec_destroy(v);
}

TEST(test_set_null_vec) {
    int a = 1;
    ASSERT(vec_set(NULL, 0, &a) == -1);
}

/* ── vec_insert ───────────────────────────────────────────────────────── */

TEST(test_insert_at_beginning) {
    vec_t* v = vec_create(4);
    int a = 10, b = 20, c = 99;
    vec_push(v, &a);
    vec_push(v, &b);
    int ret = vec_insert(v, 0, &c);
    ASSERT(ret == 0);
    ASSERT(*(int*)vec_get(v, 0) == 99);
    ASSERT(*(int*)vec_get(v, 1) == 10);
    ASSERT(*(int*)vec_get(v, 2) == 20);
    ASSERT(v->length == 3);
    vec_destroy(v);
}

TEST(test_insert_at_middle) {
    vec_t* v = vec_create(4);
    int a = 10, b = 20, c = 99;
    vec_push(v, &a);
    vec_push(v, &b);
    vec_insert(v, 1, &c);
    ASSERT(*(int*)vec_get(v, 0) == 10);
    ASSERT(*(int*)vec_get(v, 1) == 99);
    ASSERT(*(int*)vec_get(v, 2) == 20);
    vec_destroy(v);
}

TEST(test_insert_at_end) {
    vec_t* v = vec_create(4);
    int a = 10, b = 20, c = 99;
    vec_push(v, &a);
    vec_push(v, &b);
    vec_insert(v, 2, &c);
    ASSERT(*(int*)vec_get(v, 2) == 99);
    vec_destroy(v);
}

TEST(test_insert_out_of_bounds) {
    vec_t* v = vec_create(4);
    int a = 1;
    ASSERT(vec_insert(v, 5, &a) == -1);
    vec_destroy(v);
}

TEST(test_insert_triggers_resize) {
    vec_t* v = vec_create(2);
    int a = 1, b = 2, c = 3;
    vec_push(v, &a);
    vec_push(v, &b);
    int ret = vec_insert(v, 0, &c);
    ASSERT(ret == 0);
    ASSERT(v->capacity >= 3);
    ASSERT(*(int*)vec_get(v, 0) == 3);
    vec_destroy(v);
}

TEST(test_insert_null_vec) {
    int a = 1;
    ASSERT(vec_insert(NULL, 0, &a) == -1);
}

/* ── vec_delete_item ───────────────────────────────────────────────────────── */
// void vec_delete_item(vec_t* v, void* item)
TEST(test_delete_item) {
    vec_t* v = vec_create(4);
    int a = 10, b = 20, c = 30;
    vec_push(v, &a);
    vec_push(v, &b);
    vec_push(v, &c);
}



/* ── vec_delete ───────────────────────────────────────────────────────── */

TEST(test_delete_first) {
    vec_t* v = vec_create(4);
    int a = 10, b = 20, c = 30;
    vec_push(v, &a);
    vec_push(v, &b);
    vec_push(v, &c);
    void* removed = vec_delete(v, 0);
    ASSERT(*(int*)removed == 10);
    ASSERT(*(int*)vec_get(v, 0) == 20);
    ASSERT(*(int*)vec_get(v, 1) == 30);
    ASSERT(v->length == 2);
    vec_destroy(v);
}

TEST(test_delete_middle) {
    vec_t* v = vec_create(4);
    int a = 10, b = 20, c = 30;
    vec_push(v, &a);
    vec_push(v, &b);
    vec_push(v, &c);
    void* removed = vec_delete(v, 1);
    ASSERT(*(int*)removed == 20);
    ASSERT(*(int*)vec_get(v, 0) == 10);
    ASSERT(*(int*)vec_get(v, 1) == 30);
    ASSERT(v->length == 2);
    vec_destroy(v);
}

TEST(test_delete_last) {
    vec_t* v = vec_create(4);
    int a = 10, b = 20;
    vec_push(v, &a);
    vec_push(v, &b);
    void* removed = vec_delete(v, 1);
    ASSERT(*(int*)removed == 20);
    ASSERT(v->length == 1);
    vec_destroy(v);
}

TEST(test_delete_out_of_bounds) {
    vec_t* v = vec_create(4);
    int a = 1;
    vec_push(v, &a);
    ASSERT(vec_delete(v, 5) == NULL);
    vec_destroy(v);
}

TEST(test_delete_null_vec) {
    ASSERT(vec_delete(NULL, 0) == NULL);
}

/* ── vec_resize ───────────────────────────────────────────────────────── */

TEST(test_resize_doubles_capacity) {
    vec_t* v = vec_create(4);
    size_t old_capacity = v->capacity;
    vec_resize(v);
    ASSERT(v->capacity == old_capacity * 2);
    vec_destroy(v);
}

TEST(test_resize_preserves_elements) {
    vec_t* v = vec_create(4);
    int a = 1, b = 2, c = 3;
    vec_push(v, &a);
    vec_push(v, &b);
    vec_push(v, &c);
    vec_resize(v);
    ASSERT(*(int*)vec_get(v, 0) == 1);
    ASSERT(*(int*)vec_get(v, 1) == 2);
    ASSERT(*(int*)vec_get(v, 2) == 3);
    ASSERT(v->length == 3);
    vec_destroy(v);
}

TEST(test_resize_null_vec) {
    ASSERT(vec_resize(NULL) == -1);
}

/* ── vec_destroy ──────────────────────────────────────────────────────── */

TEST(test_destroy_null) {
    vec_destroy(NULL);
}

TEST(test_destroy_empty) {
    vec_t* v = vec_create(4);
    vec_destroy(v);
}

TEST(test_destroy_with_elements) {
    vec_t* v = vec_create(4);
    int a = 1, b = 2;
    vec_push(v, &a);
    vec_push(v, &b);
    vec_destroy(v);
}

/* ── vec_print ────────────────────────────────────────────────────────── */

TEST(test_print_no_crash) {
    vec_t* v = vec_create(4);
    int a = 1, b = 2, c = 3;
    vec_push(v, &a);
    vec_push(v, &b);
    vec_push(v, &c);
    vec_print(v, print_int);
    vec_print(v, NULL);
    vec_print(NULL, NULL);
    vec_destroy(v);
}

/* ── main ─────────────────────────────────────────────────────────────── */

int main(void) {
    printf("\n=== Vec Test Suite ===\n\n");

    printf("-- vec_create --\n");
    RUN(test_create_valid);
    RUN(test_create_zero_capacity);
    RUN(test_create_capacity_one);

    printf("\n-- vec_push --\n");
    RUN(test_push_single);
    RUN(test_push_multiple);
    RUN(test_push_null_vec);
    RUN(test_push_triggers_resize);
    RUN(test_push_preserves_order);

    printf("\n-- vec_get --\n");
    RUN(test_get_valid_index);
    RUN(test_get_out_of_bounds);
    RUN(test_get_null_vec);
    RUN(test_get_empty_vec);
    printf("\n-- vec_pop --\n");
    RUN(test_pop_returns_last);
    RUN(test_pop_empty_vec);
    RUN(test_pop_null_vec);
    RUN(test_pop_until_empty);
    
    printf("\n-- vec_set --\n");
    RUN(test_set_valid_index);
    RUN(test_set_out_of_bounds);
    RUN(test_set_null_vec);
    
    printf("\n-- vec_insert --\n");
    RUN(test_insert_at_beginning);
    RUN(test_insert_at_middle);
    RUN(test_insert_at_end);
    RUN(test_insert_out_of_bounds);
    RUN(test_insert_triggers_resize);
    RUN(test_insert_null_vec);

    printf("\n-- vec_delete --\n");
    RUN(test_delete_first);
    RUN(test_delete_middle);
    RUN(test_delete_last);
    RUN(test_delete_out_of_bounds);
    RUN(test_delete_null_vec);

    printf("\n-- vec_resize --\n");
    RUN(test_resize_doubles_capacity);
    RUN(test_resize_preserves_elements);
    RUN(test_resize_null_vec);

    printf("\n-- vec_destroy --\n");
    RUN(test_destroy_null);
    RUN(test_destroy_empty);
    RUN(test_destroy_with_elements);
    printf("\n-- vec_print --\n");
    RUN(test_print_no_crash);
    printf("\n=== Results: %d / %d passed ===\n\n", tests_passed, tests_run);
    return tests_passed == tests_run ? 0 : 1;
}