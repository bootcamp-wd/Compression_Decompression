#ifndef LZ77_H
#define LZ77_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "general_define.h"

typedef struct encoded_sequence
{
	U_16 distance;
	U_16 length;
	U_8 mis_match_byte;
}Encoded_sequence_t;


void lz77_encode(const U_8* const input_buffer, U_32 const input_size, U_8* output_buffer, U_32* output_size,
	U_32 const dictionary_size, U_32 const buffer_search_size);

unsigned char* search_in_dictionary(U_8* dict_pointer_first, U_8* dict_pointer_last,
	U_8* buffer_search_pointer_first, U_8* buffer_search_pointer_last);

void lz77_decode(U_8* input_data, U_32* input_size, U_8* output_data);

Encoded_sequence_t convert_into_encoded_sequence(U_8* input_pointer);

void add_sequence_to_output(U_8* output_pointer, Encoded_sequence_t current_sequence);

U_32 get_size_of_encoded_sequence_struct();

#endif // !LZ77_H
