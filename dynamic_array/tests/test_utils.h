#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <stdio.h>

static int  _tests_run    = 0;
static int  _tests_passed = 0;
static int  _tests_failed = 0;

#define ASSERT(cond, msg)                                               \
    do {                                                                \
        _tests_run++;                                                   \
        if (cond) {                                                     \
            _tests_passed++;                                            \
            printf("  [PASS] %s\n", msg);                              \
        } else {                                                        \
            _tests_failed++;                                            \
            printf("  [FAIL] %s  (%s:%d)\n", msg, __FILE__, __LINE__); \
        }                                                               \
    } while(0)

#define ASSERT_EQ(a, b, msg)  ASSERT((a) == (b), msg)
#define ASSERT_NULL(p, msg)   ASSERT((p) == NULL, msg)
#define ASSERT_NOTNULL(p,msg) ASSERT((p) != NULL, msg)
#define ASSERT_STREQ(a, b, msg) ASSERT(strcmp((a),(b)) == 0, msg)

#define TEST_SUMMARY()                                                  \
    printf("\n%d tests run — %d passed, %d failed\n",                  \
           _tests_run, _tests_passed, _tests_failed);                  \
    return _tests_failed > 0 ? 1 : 0

#endif