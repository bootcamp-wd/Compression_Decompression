#ifndef LZ77_H
#define LZ77_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct sequence
{
	short distance;
	short length;
	unsigned char mis_match_byte;
} Encoded_sequence;

void lz77_encode(const unsigned char* input_data, int const input_size, unsigned char* output_data, int* output_size,
	unsigned int const dictionary_size, unsigned int const buffer_search_size);

unsigned char* search_in_dictionary(unsigned char* dict_pointer_first, unsigned char* dict_pointer_last,
	unsigned char* buffer_search_pointer_first, unsigned char* buffer_search_pointer_last);


#endif // !LZ77_H
