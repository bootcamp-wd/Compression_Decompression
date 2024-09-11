
#include "general_define.h"
#include "compressor_decompressor.h"
#include "lz77_test.h"

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


#define COMPRESS 1
#define UN_COMPRESS 1
#define DEFAULT_COMPRESSION_LEVEL 1


void end_to_end_treatment(const U_08* orginal_test_file, U_08* input_buffer);
//size
void regular_size_file_test();
void short_file_test();
void long_file_test();
void empty_file_test();
void single_character();
//suffix
void Text_file_test();
void Image_file_test();
void Audio_file_test();
void PDF_file_test();
void Unknown_file_test();
//more
void random_file_test();
void all_ascii_file_test();
void repeating_single_character_file_test();
//error handling
void not_exist_file_test();
void Incorrect_path();



//exa file






