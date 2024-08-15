#ifndef DECOMPRESSION_H
#define DECOMPRESSION_H

#include <stdlib.h>

unsigned char** decompression(const unsigned char* input_buffer, int input_size, int* output_size, int window_size,
	int buffer_search_size);

#endif // !DECOMPRESSION_H
