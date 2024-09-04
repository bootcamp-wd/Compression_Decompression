
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

	int output_size = 0;
	unsigned char* compressed_data = (unsigned char*)malloc(output_size * sizeof(unsigned char));;
	if (compressed_data == NULL) {
		// Handle memory allocation failure
	}
	//U_08* out = (U_08*)malloc(28 * sizeof(U_08));
	U_08 out[28];
	unsigned char* input_1 = "1234c fdsa";
	int size = strlen(input_1);
	huffman_encode(input_1, &compressed_data,  size, &output_size);
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






