#include <stdio.h>
#include <string.h>
#include "lz77_test.c"
#include "file_managment.h"
#include "compressor_decompressor.h"
#include "huffman_code.h"


int main()
{
    const char* original_file = "input.txt";
    const char* compressed_file = "compressed.bin";
    const char* decompressed_file = "decompressed";

    // Simulate compression
    printf("Simulating compression...\n");
    process_file(original_file, compressed_file, 1, 1);
	//void process_file(const char* input_path, const char* output_path, int compress_level, int compress);

    // Simulate decompression
    printf("\nSimulating decompression...\n");
    process_file(compressed_file, decompressed_file, 1, 0);

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
