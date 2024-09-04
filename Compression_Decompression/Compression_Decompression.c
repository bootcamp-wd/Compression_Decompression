#include <stdio.h>
#include <string.h>
#include "lz77_test.c"
#include "file_managment.h"
#include "compressor_decompressor.h"
#include "huffman_code.h"

#define COMPRESS 1
#define UN_COMPRESS 1
#define DEFAULT_COMPRESSION_LEVEL 1

int main()
{
    const char* original_file = "input.txt";
    const char* compressed_file = "compressed.bin";
    const char* decompressed_file = "decompressed";

    printf("compression...\n");
    process_file(original_file, compressed_file, DEFAULT_COMPRESSION_LEVEL , COMPRESS);

    printf("\ndecompression...\n");
    process_file(compressed_file, decompressed_file, DEFAULT_COMPRESSION_LEVEL , UN_COMPRESS);

	//int size = 5, output_size = 0;
	//unsigned char* compressed_data = (unsigned char*)malloc(output_size * sizeof(unsigned char));;
	//if (compressed_data == NULL) {
	//	// Handle memory allocation failure
	//}
	//const char out[28];



	//huffman_encode("april", &compressed_data,  size, &output_size);
	//huffman_decode(&compressed_data, &output_size, out);

	//return 0;

//	const char st[28] = "april";
//	const char res[100];
//	const char out[28];
//	int res_size;
//	huffman_encode(st, res, 27, &res_size);
//	//printf("%s", res);
//	huffman_decode(res, &res_size, out);

}
