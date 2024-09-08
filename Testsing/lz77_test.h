#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "test_framework.h"
#include "../Compression_Decompression/lz77.h"
#include "../Compression_Decompression/general_define.h"

void lz77_test_treatment(const char* test_name, U_08* input_buffer, U_32 input_size, U_32 compress_level);
void test_lz77_regular_size(void);
void test_lz77_identical_characters(void);
void test_lz77_empty(void);
void test_lz77_many_repeat(void);