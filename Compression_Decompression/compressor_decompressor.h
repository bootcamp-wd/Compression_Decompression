
#ifndef COMPRESSOR_DECOMPRESSOR_H
#define COMPRESSOR_DECOMPRESSOR_H

#define __STDC_WANT_LIB_EXT1__ 1
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <io.h>
#include "lz77.h"
#include "huffman_code.h"
#include "file_managment.h"
#include "general_define.h"

typedef struct file_metadata {
	
	//lz77 decode output
	size_t original_size;
	//huffman decode output
	size_t size_output_huffman;
}file_metadata;


void compress_data(const U_08* input_buffer, size_t input_size, U_08** output_buffer, 
	size_t* output_size, U_32 compress_level);
void decompress_data(const U_08* input_buffer, size_t input_size, U_08** output_buffer, size_t* output_size);
void process_file(const U_08* input_path, const U_08* output_path, U_32 compress_level, U_32 compress);

#endif //COMPRESSOR_DECOMPRESSOR_H