#include <stdio.h>
#include <string.h>
#include "lz77_test.c"
#include "file_managment.h"
#include "compressor_decompressor.h"
#include "huffman_code.h"
#include "log.h"

#define COMPRESS 1
#define UN_COMPRESS 1
#define DEFAULT_COMPRESSION_LEVEL 1

int main()
{
	FILE* file;
	U_08* file_name = "logger";
	errno_t error_code;

	//Opening file in r mode
	error_code = fopen_s(&file ,file_name, "w");
	if (error_code != 0) {
		printf("Failed to open log file.\n");
		return 1;
	}

	int level = LOG_TRACE;  // רמת הלוגים שתיכתב
	log_add_fp(file, level);  // הוספת קובץ הלוג כמקום יעד ללוגים
	const U_08* input_buffer = "abab";
	const U_32 input_size = strlen(input_buffer);
	U_08* output_buffer = (U_08*)malloc(strlen(input_buffer) * sizeof(Encoded_sequence_t) * sizeof(U_08));
	U_32 output_size;
	const U_32 compress_level = 6;
	lz77_encode(input_buffer, input_size, output_buffer, &output_size, compress_level);
	return 0;
}
