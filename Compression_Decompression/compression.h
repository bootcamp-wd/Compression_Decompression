#ifndef COMPRESSION_H
#define COMPRESSION_H

#include <stdlib.h>

unsigned char* compression(unsigned char* input_buffer, unsigned int input_size, unsigned int* output_size,
	unsigned int window_size, unsigned int buffer_search_size);

#endif // !COMPRESSION_H
