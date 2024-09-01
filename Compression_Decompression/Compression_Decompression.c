#include <stdio.h>
#include <string.h>
#include "lz77.h"
#include "file_managment.h"
#include "compressor_decompressor.h"
#include "huffman_code.h"


int main()
{
//    const char* original_file = "input.png";
//    const char* compressed_file = "compressed.bin";
//    const char* decompressed_file = "decompressed";
//
//    // Simulate compression
//    printf("Simulating compression...\n");
//    process_file(original_file, compressed_file, 1);
//
//    // Simulate decompression
//    printf("\nSimulating decompression...\n");
//    process_file(compressed_file, decompressed_file, 0);
	int size = 9, output_size = 0;
	unsigned char* compressed_data = (unsigned char*)malloc(output_size * sizeof(unsigned char));;
	if (compressed_data == NULL) {
		// Handle memory allocation failure
	}
	unsigned char* metadata = (char*)malloc(sizeof(char) * 10);
	huffman_encode("abcdefghi", &compressed_data,  size, &output_size);

	return 0;

}
