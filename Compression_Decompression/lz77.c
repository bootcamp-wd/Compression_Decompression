#include "lz77.h"

/**************************************************************************
*						            LZ77 ENCODE FUNCTION
* Name			: lz77_encode - encodes data according to LZ77 algorithm
* Parameters	: input_data - pointer of buffer to encode, input_size - size of the input
*				  output_data - data to store the result, output_size - var to store the output size
*				  dictionary_size - the largest size that the dictionary can be
*				  buffer_search_size - the largest size that the buffer_search can be
* Returned		: none
* *************************************************************************/
void lz77_encode(U_08* const input_buffer, U_32 const input_size, U_08* output_buffer, U_32* output_size,
	U_32 const compress_level)
{
	log_debug("Starting Lz77_encode function");

	U_08* dict_pointer_first;
	U_08* dict_pointer_last;
	U_08* buffer_search_pointer_first;
	U_08* buffer_search_pointer_last;
	U_32 const dictionary_size;
	U_32 const buffer_search_size;

	size_of_window_according_level(compress_level, &dictionary_size, &buffer_search_size);
	log_info("Window size set according to the compression level");

	*output_size = 0;
	if (input_size == 0)
	{
		log_warn("Input size is 0, nothing to encode");
		return;
	}

	Encoded_sequence_t* cur_seq = (Encoded_sequence_t*)malloc(sizeof(Encoded_sequence_t));
	if (cur_seq == NULL)
	{
		log_fatal("Memory allocation failed in LZ77_encode");
		exit(1);
	}
	//the cur_index_seq keeps the index of the current byte that stands on
	U_32 cur_index_seq = 0;

	//initialize the first byte - that not exist sure 
	cur_seq->distance = 0;
	cur_seq->length = 0;
	cur_seq->mis_match_byte = *input_buffer;

	memcpy(output_buffer + (*output_size), cur_seq, sizeof(Encoded_sequence_t));
	(*output_size) += sizeof(Encoded_sequence_t);
	cur_index_seq++;

	dict_pointer_first = dict_pointer_last = input_buffer;
	buffer_search_pointer_first = buffer_search_pointer_last = input_buffer + 1;

	//if there is more byte to input entering to the loop
	while (cur_index_seq < input_size)
	{
		log_trace("Encoding byte");
		//if its the last byte of the input or the size of the buffer_search entering the current byte to res
		if (cur_index_seq == input_size - 1 || (buffer_search_pointer_last -
			buffer_search_pointer_first + 1) == buffer_search_size)
		{
			log_info("Last byte or buffer search size reached");

			cur_seq->mis_match_byte = *buffer_search_pointer_last;
			memcpy(output_buffer + (*output_size), cur_seq, sizeof(Encoded_sequence_t));
			(*output_size) += sizeof(Encoded_sequence_t);
			//initialize the vars from begin to the next sequence
			cur_seq->distance = 0;
			cur_seq->length = 0;
			buffer_search_pointer_first = ++buffer_search_pointer_last;
		}
		//if its not the last byte of the input or the size of the buffer_search
		else
		{
			//the start_seq_pointer points to the begining of the sequence that equals to the 
			//current buffer_search that includes in the dictionary, if there isn't it's NULL
			U_08* start_seq_pointer = search_in_dictionary(dict_pointer_first, dict_pointer_last,
				buffer_search_pointer_first, buffer_search_pointer_last);
			//if there is equal sequence in the dictionary
			if (start_seq_pointer)
			{
				log_debug("Match found in dictionary");

				cur_seq->distance = buffer_search_pointer_first - start_seq_pointer;
				cur_seq->length = buffer_search_pointer_last - buffer_search_pointer_first + 1;
				buffer_search_pointer_last++;
			}
			//if there isn't equal sequence in the dictionary
			else
			{
				log_warn("No match found, adding mismatch byte");
				cur_seq->mis_match_byte = *(buffer_search_pointer_last);
				memcpy(output_buffer + (*output_size), cur_seq, sizeof(Encoded_sequence_t));
				(*output_size) += sizeof(Encoded_sequence_t);

				cur_seq->distance = 0;
				cur_seq->length = 0;
				buffer_search_pointer_first = ++buffer_search_pointer_last;
			}
		}
		//if this is the size of the dictionary the first goes forwards
		if ((dict_pointer_last - dict_pointer_first + 1) == dictionary_size)\
		{
			dict_pointer_first++;
		}
		dict_pointer_last++;
		cur_index_seq++;
	}
	log_info("Encoding finished successfully");
	free(cur_seq);
}

/**************************************************************************
*						            SEARCH IN DICTIONARY FUNCTION
* Name			: search_in_dictionary - check if the buffer_search is includes in the dictionary
* Parameters	: dict_pointer_first - pointer to the begining of the dictionary
*				  dict_pointer_last -  pointer to the end of the dictionary
*				  buffer_search_pointer_first - pointer to the begining of the buffer search
*				  buffer_search_pointer_last -  pointer to the end of the buffer search
* Returned		: pointer to the begining of the similiar sequence, if not exist - NULL
* *************************************************************************/
U_08* search_in_dictionary(U_08* dict_pointer_first, U_08* dict_pointer_last,
	U_08* buffer_search_pointer_first, U_08* buffer_search_pointer_last)
{
	log_debug("Starting search_in_dictionary function");

	U_08* start_seq_pointer = NULL;
	U_08 loc_buffer_search = 0;
	U_32 size_buffer_search = buffer_search_pointer_last - buffer_search_pointer_first;

	log_info("Size of buffer search is %u", size_buffer_search);

	//loop that moves all the dictionary
	for (U_32 i = 0; (dict_pointer_first + i) <= dict_pointer_last; i++)
	{
		log_trace("Checking dictionary byte at position %u", i);

		//if the current byte in the buffer_search is equal to the current in the dictionary
		if (buffer_search_pointer_first[loc_buffer_search] == *(dict_pointer_first + i))
		{
			if (loc_buffer_search == 0)
			{
				start_seq_pointer = dict_pointer_first + i;
				log_info("Start sequence pointer set at index %u", i);
			}

			//the buffer_search is exist in the dictionary
			if (loc_buffer_search == size_buffer_search / sizeof(U_08))
			{
				log_info("Buffer search found in dictionary starting at %p", start_seq_pointer);
				return start_seq_pointer;
			}
			loc_buffer_search++;
		}
		//if the current byte in the buffer_search is not equal to the current in the dictionary
		else
		{
			//the buffer begins from begining
			loc_buffer_search = 0;
		}
	}    log_info("Search complete. No matching sequence found.");
	return NULL;
}

/**************************************************************************
*						            SIZE OF WINDOW ACCORDING LEVEL FUNCTION
* Name			: size_of_window_according_level - calculates the size of the dictionary_size and the
*				  buffer_search_size according to the level of compressing
* Parameters	: compress_level - desired compression level
*				  dictionary_size - a pointer to store the largest size that the dictionary can be
*				  according to the compress_level param
*				  buffer_search_size - the largest size that the buffer_search can be
*				  according to the compress_level param
* Returned		: none
* *************************************************************************/
void size_of_window_according_level(U_32 compress_level, U_32* dictionary_size, U_32* buffer_search_size)
{
	log_info("Calculating window size for compress level %u", compress_level);

	switch (compress_level)
	{
	case 0:
		*dictionary_size = 512;
		*buffer_search_size = 256;
		break;
	case 1:
		*dictionary_size = 1024;
		*buffer_search_size = 512;
		break;
	case 2:
		*dictionary_size = 2048;
		*buffer_search_size = 1024;
		break;
	case 3:
		*dictionary_size = 4096;
		*buffer_search_size = 2048;
		break;
	case 4:
		*dictionary_size = 8192;
		*buffer_search_size = 4096;
		break;
	case 5:
		*dictionary_size = 16384;
		*buffer_search_size = 8192;
		break;
	case 6:
		*dictionary_size = 32767;
		*buffer_search_size = 16384;
		break;
	default:
		*dictionary_size = 4096;
		*buffer_search_size = 2048;
		break;
	}

	log_info("Dictionary size: %u, Buffer search size: %u", *dictionary_size, *buffer_search_size);
}

/***************************************************************************
 *                            LZ77_DECODE FUNCTION
 * Name         : decode the data with lz77 decompression algorithm
 * Parameters   : input_data - pointer to the input data buffer
 *                input_size - size of the input data.
 *                output_data - pointer to the output data buffer to fill it
 * Returned     : none
 *
 ***************************************************************************/
void lz77_decode(U_08* input_data, U_32* input_size, U_08* output_data)
{
	log_info("Starting LZ77_decode function.");

	//pointers to the buffers of the input 
	U_08* input_pointer;
	U_08* output_pointer;

	input_pointer = input_data;
	output_pointer = output_data;

	Encoded_sequence_t current_sequence;

	for (U_32 i = 0; i < (*input_size / sizeof(Encoded_sequence_t)); i++)
	{
		current_sequence = convert_into_encoded_sequence(input_pointer);

		log_debug("Decoded sequence at index %u: length=%u, distance=%u", i, current_sequence.length, current_sequence.distance);

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
Encoded_sequence_t convert_into_encoded_sequence(U_08* input_pointer)
{
	log_debug("Filling encoded sequence from input data.");

	Encoded_sequence_t sequence;
	memcpy(&sequence, input_pointer, sizeof(Encoded_sequence_t));

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
void add_sequence_to_output(U_08* output_pointer, Encoded_sequence_t current_sequence)
{
	U_08 byte_to_copy;
	U_08* sequence_to_copy = output_pointer - current_sequence.distance;
	if (current_sequence.length > 0)
	{
		for (U_32 i = 0; i < current_sequence.length; i++)
		{
			byte_to_copy = *(sequence_to_copy + i);
			(*output_pointer) = byte_to_copy;
			output_pointer++;
		}
	}
	log_debug("Adding mismatch byte: %u", current_sequence.mis_match_byte);
	(*output_pointer) = current_sequence.mis_match_byte;
	output_pointer++;
}

U_32 get_size_of_encoded_sequence_struct()
{
	return sizeof(Encoded_sequence_t);
}