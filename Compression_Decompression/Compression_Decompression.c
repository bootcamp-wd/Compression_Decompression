#include "compressor_decompressor.h"
#include "huffman_code.h"

#define COMPRESS 1
#define UN_COMPRESS 1
#define DEFAULT_COMPRESSION_LEVEL 1

int main()
{
	const S_08* original_file = "input.txt";
	const S_08* compressed_file = "compressed.bin";
	const S_08* decompressed_file = "decompressed";
	
	// Simulate compression
	printf("Simulating compression...\n");
	process_file(original_file, compressed_file, 1, 1);
	//void process_file(const char* input_path, const char* output_path, int compress_level, int compress);
	//printf("compression...\n");
	//process_file(original_file, compressed_file, DEFAULT_COMPRESSION_LEVEL, COMPRESS);
	//
	// Simulate decompression
	printf("\nSimulating decompression...\n");
	process_file(compressed_file, decompressed_file, 1, 0);
	/*printf("\ndecompression...\n");
	process_file(compressed_file, decompressed_file, DEFAULT_COMPRESSION_LEVEL, UN_COMPRESS);
	*/
	//int size = 5, output_size = 0;
	//unsigned char* compressed_data = (unsigned char*)malloc(output_size * sizeof(unsigned char));;

}
