#include "decompression.h"

#define WINDOW_SIZE 32000
#define BUFFER_SEARCH_SIZE 8000

unsigned char** decompression(unsigned char* input_buffer, int input_size, int* output_size, int window_size,
	int buffer_search_size)
{
	if (window_size == NULL || buffer_search_size == NULL) {
		window_size = WINDOW_SIZE;
		buffer_search_size = BUFFER_SEARCH_SIZE;
	}

	int size_encoded_seq_struct = sizeof(short) + sizeof(short) + sizeof(unsigned char);
	unsigned char** res = (unsigned char**)malloc(input_size * size_encoded_seq_struct * sizeof(unsigned char*));
	//lz77_decode(input_buffer, input_size, res, output_size, window_size, buffer_search_size);

	//calling to huffman and treat
}
