#include "lz77.h"
#include "lz77.h"


/***************************************************************************
 *                            LZ77_DECODE FUNCTION
 * Name         : decode the data with lz77 decompression algorithm
 * Parameters   : input_data - pointer to the input data buffer
 *                input_size - size of the input data.
 *                output_data - pointer to the output data buffer to fill it
 * Returned     : none
 *
 ***************************************************************************/
void lz77_decode(U_8* input_data, U_32* input_size,
	U_8* output_data)
{
	//pointers to the buffers of the input 
	U_8* input_pointer;
	U_8* output_pointer;

	input_pointer = input_data;
	output_pointer = output_data;

	Encoded_sequence_t current_sequence;

	for (U_32 i = 0; i < (*input_size / sizeof(Encoded_sequence_t)); i++)
	{
		current_sequence = convert_into_encoded_sequence(input_pointer);

		input_pointer += sizeof(Encoded_sequence_t);

		add_sequence_to_output(output_pointer, current_sequence);

		output_pointer += current_sequence.length + 1;//length of the sequence and the mismatch byte
	}
}

/***************************************************************************
 *                            FILL_ENCODED_SEQUENCE FUNCTION
 * Name         : fill the struct of the encoded_sequence according to the input data
 * Parameters   : input_pointer - pointer to the input data buffer
 * Returned     : update struct
 *
 ***************************************************************************/
Encoded_sequence_t convert_into_encoded_sequence(U_8* input_pointer)
{
	Encoded_sequence_t sequence = { -1,-1 ,'NULL' };

	memcpy(&sequence, *input_pointer, sizeof(sequence));

	/*memcpy(&sequence.distance, *input_pointer, sizeof(sequence.distance));

	memcpy(&sequence.length, *input_pointer, sizeof(sequence.length));

	memcpy(sequence.mis_match_byte, *input_pointer, sizeof(sequence.mis_match_byte));*/

	return sequence;
}
/***************************************************************************
 *                            ADD_SEQUANCE_TO_OUTPUT FUNCTION
 * Name         : add the current sequence to output.
 * Parameters   : current_sequence - struct with the data of the sequence to add
 *                output_pointe - pointer to the position to add the sequence
 * Returned     : none
 *
 ***************************************************************************/
void add_sequence_to_output(U_8* output_pointer,
	Encoded_sequence_t current_sequence) {

	U_8* sequence_to_copy = output_pointer - current_sequence.distance;

	memcpy(output_pointer, sequence_to_copy, current_sequence.length);

	memcpy(output_pointer + current_sequence.length, current_sequence.mis_match_byte, sizeof(current_sequence.mis_match_byte));


}


