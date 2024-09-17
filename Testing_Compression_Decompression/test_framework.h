#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

#include <stdio.h>

#define MAX_TESTS 100

typedef struct {
    const char* name;
    void (*test_function)(void);
} Test;

extern Test g_tests[MAX_TESTS];
extern int g_test_count;
// Global variable to track test success
extern int g_test_failed;

void add_test(const char* name, void (*test_function)(void));
void run_all_tests(void);

#define ASSERT(condition, message) \
    do { \
        if (!(condition)) { \
            printf("ASSERTION FAILED: %s\n", message); \
            printf("File: %s, Line: %d\n", __FILE__, __LINE__); \
            g_test_failed = 1; \
            return; \
        } \
        else \
        { \
            printf("TEST PASSED\n"); \
        } \
    } while (0)

#define ASSERT_EQUAL(expected, actual, message) \
    do { \
        if ((expected) != (actual)) { \
            printf("ASSERTION FAILED: %s\n", message); \
            printf("Expected: %d, Actual: %d\n", (int)(expected), (int)(actual)); \
            printf("File: %s, Line: %d\n", __FILE__, __LINE__); \
            g_test_failed = 1; \
            return; \
        } \
         else \
        { \
            printf("TEST PASSED\n"); \
        } \
    } while (0)

#endif // TEST_FRAMEWORK_H
