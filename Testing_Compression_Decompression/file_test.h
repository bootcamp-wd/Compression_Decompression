
#ifndef TEST_FILE_H
#define TEST_FILE_H

#include "lz77_test.h"
#include "../Compression_Decompression/general_define.h"
#include "../Compression_Decompression/file_managment.h"

#define ASSERT(condition, message) \
    do { \
        if (!(condition)) { \
            printf("ASSERTION FAILED: %s\n", message); \
            printf("File: %s, Line: %d\n", __FILE__, __LINE__); \
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
            return; \
        } \
         else \
        { \
            printf("TEST PASSED\n"); \
        } \
    } while (0)


void files_test();
void read_test(const U_08* file_path, const U_08* exepted_buffer, size_t file_size);
void write_test(const U_08* file_path, const U_08* buffer, size_t file_size);



#endif 
