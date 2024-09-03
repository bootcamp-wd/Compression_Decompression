#include "test_framework.h"

Test g_tests[MAX_TESTS];
int g_test_count = 0;

void add_test(const char* name, void (*test_function)(void)) {
    if (g_test_count < MAX_TESTS) {
        g_tests[g_test_count].name = name;
        g_tests[g_test_count].test_function = test_function;
        g_test_count++;
    }
    else {
        printf("ERROR: Maximum number of tests reached\n");
    }
}

void run_all_tests(void) {
    int passed = 0;
    int failed = 0;

    for (int i = 0; i < g_test_count; i++) {
        printf("Running test: %s\n", g_tests[i].name);
        g_tests[i].test_function();
        passed++;
    }

    printf("\nTest Results: %d passed, %d failed\n", passed, failed);
}