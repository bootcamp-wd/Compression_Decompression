#include "compressor_decompressor.h"
#include "huffman_code.h"
#include "general_define.h"

#define COMPRESS 1
#define UN_COMPRESS 1
#define DEFAULT_COMPRESSION_LEVEL 1

int main()
{
	const U_08 st[28] = "april";
	const U_08 res[100];
	const U_08 out[28];
	int res_size;
	huffman_encode(st, res, 27, &res_size);
	printf("%s", res);
	huffman_decode(res, &res_size, out);
	printf("%s", out);

	const U_08* original_file = "input.txt";
	const U_08* compressed_file = "compressed.bin";
	const U_08* decompressed_file = "decompressed";
	
	// Simulate compression
	printf("Simulating compression...\n");
	process_file(original_file, compressed_file, 1, 1);
	//void process_file(const U_08* input_path, U_08* output_path, S_32 compress_level, S_32 compress)
	printf("compression...\n");
	process_file(original_file, compressed_file, DEFAULT_COMPRESSION_LEVEL, COMPRESS);
	
	// Simulate decompression
	printf("\nSimulating decompression...\n");
	process_file(compressed_file, decompressed_file, 1, 0);
	printf("\ndecompression...\n");
	process_file(compressed_file, decompressed_file, DEFAULT_COMPRESSION_LEVEL, UN_COMPRESS);
	
	//int size = 5, output_size = 0;
	//unsigned char* compressed_data = (unsigned char*)malloc(output_size * sizeof(unsigned char));;

}
