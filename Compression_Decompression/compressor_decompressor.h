
#ifndef COMPRESSOR_DECOMPRESSOR_H
#define COMPRESSOR_DECOMPRESSOR_H

#define __STDC_WANT_LIB_EXT1__ 1
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <io.h>
#include "lz77.h"
#include "huffman_code.h"
#include "file_managment.h"
#include "general_define.h"

#define BUFFER_SIZE 4096

typedef struct file_metadata {
	
	//lz77 decode output
	U_32 original_size;
	//huffman decode output
	U_32 size_output_huffman;
}file_metadata;

void process_file(const U_08* input_path,const U_08* output_path, U_32 compress_level, U_32 compress);

void compress_data(const U_08* input_buffer, U_32 input_size, U_08** output_buffer, 
	U_32* output_size, U_32 compress_level);
void decompress_data(const U_08* input_buffer, U_32 input_size, U_08** output_buffer, U_32* output_size);

//void metadata_treatment(const U_08* file_path, U_08* extension, U_08* metadata_path, FILE* metadata_file,
//	U_08* buffer, const U_08* read_write_mode);

#endif //COMPRESSOR_DECOMPRESSOR_H