
#include "../Compression_Decompression/general_define.h"
#include "../Compression_Decompression/compressor_decompressor.h"
#include "lz77_test.h"
#include "general_define.h"

#define COMPRESS 1
#define UN_COMPRESS 1
#define DEFAULT_COMPRESSION_LEVEL 1

void end_to_end_treatment(U_08* buffer, U_32 buffer_size, U_08* test_name, void (*test_function)(void));
//size
void regular_file_test();
void short_file_test();
void long_file_test();
//suffix
void Text_file_test();
void Image_file_test();
void Audio_file_test();
void PDF_file_test();
void Unknown_file_test();






