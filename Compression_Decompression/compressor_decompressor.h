#ifndef COMPRESSOR_DECOMPRESSOR_H
#define COMPRESSOR_DECOMPRESSOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "lz77.h"
#include "huffman_code.h"
#include "file_managment.h"
#include "general_define.h"

#define BUFFER_SIZE 4096

void process_file(const S_08* input_path, const S_08* output_path, S_32 compress_level, S_32 compress);

void compress_data(const U_08* input_buffer, U_32 input_size, U_08** output_buffer, 
	U_32* output_size, U_32 compress_level);
void decompress_data(const U_08* input_buffer, S_32 input_size, U_08** output_buffer, S_32* output_size);

void metadata_treatment(const S_08* file_path, S_08* extension, const S_08* metadata_path, FILE* metadata_file,
	U_08* buffer, const S_08* read_write_mode);


#endif //COMPRESSOR_DECOMPRESSOR_H