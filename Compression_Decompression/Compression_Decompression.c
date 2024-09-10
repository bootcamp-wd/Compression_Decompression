#define _CRT_SECURE_NO_WARNINGS
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


    char* input_data = "Hello! I";
    char* output_data;
    int window_size = 5;
    int input_size = strlen(input_data) * sizeof(char), output_size;
    lz77_encode(input_data, input_size, &output_data, &output_size, 12, 12);

    for (int i = 0; i < output_size; i++)
    {
        printf("%hi", output_data[i]);
        i += 2;
        printf("%hi", output_data[i]);
        i += 2;
        printf("%c", output_data[i]);
    }
	int size = 5;
	unsigned char* compressed_data = (unsigned char*)malloc(output_size * sizeof(unsigned char));;
	if (compressed_data == NULL) {
		// Handle memory allocation failure
	}
	const char out[28];
	huffman_encode("april", &compressed_data,  size, &output_size);
	huffman_decode(&compressed_data, &output_size, out);

	return 0;

//	const char st[28] = "april";
//	const char res[100];
//	const char out[28];
//	int res_size;
//	huffman_encode(st, res, 27, &res_size);
//	//printf("%s", res);
//	huffman_decode(res, &res_size, out);
}






