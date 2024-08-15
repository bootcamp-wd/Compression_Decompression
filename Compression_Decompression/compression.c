#include "compression.h"
#include "lz77.h"

#define WINDOW_SIZE 32000
#define BUFFER_SEARCH_SIZE 8000
//#include "huffman.h"

unsigned char* compression(unsigned char* input_buffer, unsigned int input_size, unsigned int* output_size,
	unsigned int window_size, unsigned int buffer_search_size)
{
	if (window_size == 0 || buffer_search_size == 0)
	{
		window_size = WINDOW_SIZE;
		buffer_search_size = BUFFER_SEARCH_SIZE;
	}

	int size_encoded_seq_struct = sizeof(short) + sizeof(short) + sizeof(unsigned char);
	unsigned char** res = (unsigned char**)malloc(input_size * size_encoded_seq_struct * sizeof(unsigned char*));
	//lz77_encode(input_buffer, input_size, res, output_size, window_size, buffer_search_size);


	//calling to huffman and treat

	free(res + (*output_size));
}
