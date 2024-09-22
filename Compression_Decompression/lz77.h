#ifndef LZ77_H
#define LZ77_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "general_define.h"
#include "log.h"

typedef struct encoded_sequence
{
	U_16 distance;
	U_16 length;
	U_08 mis_match_byte;
}Encoded_sequence_t;


void lz77_encode(const U_08* input_buffer, size_t input_size, U_08* output_buffer, size_t* output_size,
S_32 compress_level);

U_08* search_in_dictionary(U_08* dict_pointer_first, U_08* dict_pointer_last,
	U_08* buffer_search_pointer_first, U_08* buffer_search_pointer_last);

void size_of_window_according_level(S_32 compress_level, U_32* dictionary_size, U_32* buffer_search_size);

void lz77_decode(const U_08* input_data,const size_t* input_size, U_08* output_data);

Encoded_sequence_t convert_into_encoded_sequence(U_08* input_pointer);

void add_sequence_to_output(U_08* output_pointer, Encoded_sequence_t current_sequence);

U_32 get_size_of_encoded_sequence_struct();

void print_progress(size_t current, size_t total);

#endif // !LZ77_H