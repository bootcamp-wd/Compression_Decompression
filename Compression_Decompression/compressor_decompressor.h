#ifndef COMPRESSOR_DECOMPRESSOR_H
#define COMPRESSOR_DECOMPRESSOR_H

#include "general_define.h"
#include "lz77.h"

void process_file(const U_08* input_path, const U_08* output_path, U_32 compress_level,U_32 compress);

void compress_data(const U_08* input_buffer, U_32 input_size, U_08** output_buffer, 
	U_32* output_size,U_32 compress_level);

void decompress_data(const U_08* input_buffer, U_32 input_size, U_08** output_buffer, U_32* output_size);


#endif //COMPRESSOR_DECOMPRESSOR_H