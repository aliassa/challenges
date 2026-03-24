/* FULLY AI GENERATED TEST FILE (CLAUDE) */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "hash_map.h"

/* ── helpers ──────────────────────────────────────────────────────────── */

static int tests_run    = 0;
static int tests_passed = 0;

#define TEST(name) static void name(void)

#define RUN(name) do {                          \
    printf("  %-40s", #name);                   \
    tests_run++;                                \
    name();                                     \
    tests_passed++;                             \
    printf("PASS\n");                           \
} while(0)

#define ASSERT(cond) do {                                           \
    if (!(cond)) {                                                  \
        printf("FAIL\n  assertion failed: %s\n  %s:%d\n",          \
               #cond, __FILE__, __LINE__);                          \
        exit(1);                                                    \
    }                                                               \
} while(0)

static void print_int(void* value) {
    printf("%d", *(int*)value);
}

/* ── tests ────────────────────────────────────────────────────────────── */

/* hm_create */
TEST(test_create_valid) {
    HashMap* hm = hm_create(8);
    ASSERT(hm != NULL);
    ASSERT(hm->size  == 8);
    ASSERT(hm->count == 0);
    hm_destroy(hm);
}

TEST(test_create_zero_capacity) {
    HashMap* hm = hm_create(0);
    ASSERT(hm == NULL);
}

/* hm_set / hm_get */
TEST(test_set_and_get) {
    HashMap* hm = hm_create(8);
    int val = 42;
    hm_set(hm, "name", &val);
    int* result = hm_get(hm, "name");
    ASSERT(result != NULL);
    ASSERT(*result == 42);
    hm_destroy(hm);
}

TEST(test_set_multiple_keys) {
    HashMap* hm = hm_create(8);
    int a = 1, b = 2, c = 3;
    hm_set(hm, "a", &a);
    hm_set(hm, "b", &b);
    hm_set(hm, "c", &c);
    ASSERT(*(int*)hm_get(hm, "a") == 1);
    ASSERT(*(int*)hm_get(hm, "b") == 2);
    ASSERT(*(int*)hm_get(hm, "c") == 3);
    ASSERT(hm->count == 3);
    hm_destroy(hm);
}

TEST(test_set_updates_existing_key) {
    HashMap* hm = hm_create(8);
    int v1 = 10, v2 = 99;
    hm_set(hm, "key", &v1);
    hm_set(hm, "key", &v2);          // update same key
    ASSERT(*(int*)hm_get(hm, "key") == 99);
    ASSERT(hm->count == 1);           // count must not grow
    hm_destroy(hm);
}

TEST(test_get_missing_key) {
    HashMap* hm = hm_create(8);
    ASSERT(hm_get(hm, "ghost") == NULL);
    hm_destroy(hm);
}

TEST(test_get_null_hm) {
    ASSERT(hm_get(NULL, "key") == NULL);
}

TEST(test_get_null_key) {
    HashMap* hm = hm_create(8);
    ASSERT(hm_get(hm, NULL) == NULL);
    hm_destroy(hm);
}

/* hm_has */
TEST(test_has_existing_key) {
    HashMap* hm = hm_create(8);
    int v = 1;
    hm_set(hm, "exists", &v);
    ASSERT(hm_has(hm, "exists") == 1);
    hm_destroy(hm);
}

TEST(test_has_missing_key) {
    HashMap* hm = hm_create(8);
    ASSERT(hm_has(hm, "nope") == 0);
    hm_destroy(hm);
}

/* hm_delete */
TEST(test_delete_existing_key) {
    HashMap* hm = hm_create(8);
    int v = 5;
    hm_set(hm, "del", &v);
    ASSERT(hm->count == 1);
    int ret = hm_delete(hm, "del");
    ASSERT(ret == 0);
    ASSERT(hm->count == 0);
    ASSERT(hm_get(hm, "del") == NULL);
    hm_destroy(hm);
}

TEST(test_delete_missing_key) {
    HashMap* hm = hm_create(8);
    int ret = hm_delete(hm, "ghost");
    ASSERT(ret == -1);
    hm_destroy(hm);
}

TEST(test_delete_head_of_chain) {
    /* force two keys into the same bucket by using a capacity of 1 */
    HashMap* hm = hm_create(1);
    int a = 1, b = 2;
    hm_set(hm, "a", &a);
    hm_set(hm, "b", &b);
    /* delete whichever ended up as head */
    hm_delete(hm, "a");
    ASSERT(hm_get(hm, "a") == NULL);
    ASSERT(*(int*)hm_get(hm, "b") == 2);
    hm_destroy(hm);
}

TEST(test_delete_then_reinsert) {
    HashMap* hm = hm_create(8);
    int v1 = 1, v2 = 2;
    hm_set(hm, "key", &v1);
    hm_delete(hm, "key");
    hm_set(hm, "key", &v2);
    ASSERT(*(int*)hm_get(hm, "key") == 2);
    ASSERT(hm->count == 1);
    hm_destroy(hm);
}

/* hm_keys */
TEST(test_keys_count) {
    HashMap* hm = hm_create(8);
    int a = 1, b = 2, c = 3;
    hm_set(hm, "x", &a);
    hm_set(hm, "y", &b);
    hm_set(hm, "z", &c);

    char** keys = hm_keys(hm);
    ASSERT(keys != NULL);

    /* verify all three keys are present */
    int found_x = 0, found_y = 0, found_z = 0;
    for(size_t i = 0; i < hm->count; i++) {
        if(strcmp(keys[i], "x") == 0) found_x = 1;
        if(strcmp(keys[i], "y") == 0) found_y = 1;
        if(strcmp(keys[i], "z") == 0) found_z = 1;
    }
    ASSERT(found_x && found_y && found_z);
    free(keys);
    hm_destroy(hm);
}

TEST(test_keys_null_hm) {
    ASSERT(hm_keys(NULL) == NULL);
}

/* hm_resize */
TEST(test_resize_preserves_all_entries) {
    /* start with capacity 2 and insert enough to trigger resize */
    HashMap* hm = hm_create(2);
    int vals[10];
    char keys[10][8];
    for(int i = 0; i < 10; i++) {
        vals[i] = i * 10;
        snprintf(keys[i], sizeof keys[i], "key%d", i);
        hm_set(hm, keys[i], &vals[i]);
    }
    /* all entries must still be retrievable after resize(s) */
    for(int i = 0; i < 10; i++) {
        int* r = hm_get(hm, keys[i]);
        ASSERT(r != NULL);
        ASSERT(*r == i * 10);
    }
    ASSERT(hm->count == 10);
    hm_destroy(hm);
}

TEST(test_resize_grows_size) {
    HashMap* hm = hm_create(2);
    int v = 1;
    /* insert 3 items — load factor (1.5) exceeds 0.7, must resize */
    hm_set(hm, "a", &v);
    hm_set(hm, "b", &v);
    hm_set(hm, "c", &v);
    ASSERT(hm->size > 2);
    hm_destroy(hm);
}

/* collisions */
TEST(test_collision_handling) {
    /* capacity 1 forces every key into the same bucket */
    HashMap* hm = hm_create(1);
    int a = 1, b = 2, c = 3;
    hm_set(hm, "alpha",   &a);
    hm_set(hm, "beta",    &b);
    hm_set(hm, "gamma",   &c);
    ASSERT(*(int*)hm_get(hm, "alpha") == 1);
    ASSERT(*(int*)hm_get(hm, "beta")  == 2);
    ASSERT(*(int*)hm_get(hm, "gamma") == 3);
    hm_destroy(hm);
}

/* hm_destroy */
TEST(test_destroy_null) {
    hm_destroy(NULL);   /* must not crash */
}

TEST(test_destroy_empty_map) {
    HashMap* hm = hm_create(8);
    hm_destroy(hm);     /* must not crash */
}

/* hm_print (smoke test — just verify it doesn't crash) */
TEST(test_print_no_crash) {
    HashMap* hm = hm_create(4);
    int v = 7;
    hm_set(hm, "hello", &v);
    hm_print(hm, print_int);   /* with print func  */
    hm_print(hm, NULL);        /* keys only        */
    hm_print(NULL, NULL);      /* NULL map         */
    hm_destroy(hm);
}

/* ── main ─────────────────────────────────────────────────────────────── */

int main(void) {
    printf("\n=== HashMap Test Suite ===\n\n");

    printf("-- hm_create --\n");
    RUN(test_create_valid);
    RUN(test_create_zero_capacity);

    printf("\n-- hm_set / hm_get --\n");
    RUN(test_set_and_get);
    RUN(test_set_multiple_keys);
    RUN(test_set_updates_existing_key);
    RUN(test_get_missing_key);
    RUN(test_get_null_hm);
    RUN(test_get_null_key);

    printf("\n-- hm_has --\n");
    RUN(test_has_existing_key);
    RUN(test_has_missing_key);

    printf("\n-- hm_delete --\n");
    RUN(test_delete_existing_key);
    RUN(test_delete_missing_key);
    RUN(test_delete_head_of_chain);
    RUN(test_delete_then_reinsert);

    printf("\n-- hm_keys --\n");
    RUN(test_keys_count);
    RUN(test_keys_null_hm);

    printf("\n-- hm_resize --\n");
    RUN(test_resize_preserves_all_entries);
    RUN(test_resize_grows_size);

    printf("\n-- collisions --\n");
    RUN(test_collision_handling);

    printf("\n-- hm_destroy --\n");
    RUN(test_destroy_null);
    RUN(test_destroy_empty_map);

    printf("\n-- hm_print --\n");
    RUN(test_print_no_crash);

    printf("\n=== Results: %d / %d passed ===\n\n", tests_passed, tests_run);
    return tests_passed == tests_run ? 0 : 1;
}