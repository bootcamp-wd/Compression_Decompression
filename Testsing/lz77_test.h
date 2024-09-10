#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "test_framework.h"
#include "../Compression_Decompression/lz77.h"
#include "../Compression_Decompression/general_define.h"

void lz77_test_treatment(const U_08* test_name, U_08* input_buffer, U_32 input_size, U_32 compress_level);
void test_lz77_search_in_dictionary_treatment(const U_08* dictionary, const U_08* buffer, const U_08* buffer_expected);

void lz77_test_size_of_windo_treatment(const U_32 level, const U_32 dictionary_expected, const U_32 buffer_search_expected);
void generate_random_input(U_08* buffer, U_32 size);

void test_lz77_regular_size(void);
void test_lz77_identical_characters(void);
void test_lz77_empty(void);
void test_lz77_many_repeat(void);
void test_lz77_search_dictionary_exist_in_middle(void);
void test_lz77_search_dictionary_dont_exist(void);
void test_lz77_search_dictionary_in_begin(void);
void test_lz77_search_dictionary_in_end(void);
void test_lz77_size_window_regular(void);
void test_lz77_size_window_not_in_range(void);
