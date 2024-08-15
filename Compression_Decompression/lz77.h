#ifndef LZ77_H
#define LZ77_H

#include "general_define.h"

typedef struct encoded_sequence
{
	U_16 distance;
	U_16 length;
	U_8 mis_match_byte;
}Encoded_sequence_t;


void lz77_decode(U_8* input_data, U_32* input_size, U_8* output_data);

Encoded_sequence_t convert_into_encoded_sequence(U_8* input_pointer);

void add_sequence_to_output(U_8* output_pointer, Encoded_sequence_t current_sequence);




#endif // !LZ77_H

