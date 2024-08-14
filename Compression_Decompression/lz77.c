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
void lz77_encode(const unsigned char* input_buffer, int const input_size, unsigned char* output_buffer, int* output_size,
	unsigned int const dictionary_size, unsigned int const buffer_search_size)
{
	unsigned char* dict_pointer_first;
	unsigned char* dict_pointer_last;
	unsigned char* buffer_search_pointer_first;
	unsigned char* buffer_search_pointer_last;

	*output_size = 0;
	if (input_size == 0)
	{
		return;
	}
	Encoded_sequence* cur_seq;
	cur_seq = (Encoded_sequence*)malloc(sizeof(Encoded_sequence));
	//the cur_index_seq keeps the index of the current byte that stands on
	int cur_index_seq = 0;

	//initialize the first byte - that not exist sure 
	cur_seq->distance = 0;
	cur_seq->length = 0;
	cur_seq->mis_match_byte = *input_buffer;

	memcpy(output_buffer + (*output_size), cur_seq, sizeof(Encoded_sequence));
	(*output_size) += sizeof(Encoded_sequence);
	cur_index_seq++;

	dict_pointer_first = dict_pointer_last = input_buffer;
	buffer_search_pointer_first = buffer_search_pointer_last = input_buffer + 1;

	//if there is more byte to input entering to the loop
	while (cur_index_seq < input_size)
	{
		//if its the last byte of the input or the size of the buffer_search entering the current byte to res
		if (cur_index_seq == input_size - 1 || (buffer_search_pointer_last -
			buffer_search_pointer_first + 1) == buffer_search_size)
		{
			cur_seq->mis_match_byte = *buffer_search_pointer_last;
			memcpy(output_buffer + (*output_size), cur_seq, sizeof(Encoded_sequence));
			(*output_size) += sizeof(Encoded_sequence);
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
			unsigned char* start_seq_pointer = search_in_dictionary(dict_pointer_first, dict_pointer_last,
				buffer_search_pointer_first, buffer_search_pointer_last);
			//if there is equal sequence in the dictionary
			if (start_seq_pointer)
			{
				cur_seq->distance = buffer_search_pointer_first - start_seq_pointer;
				cur_seq->length = buffer_search_pointer_last - buffer_search_pointer_first + 1;
				buffer_search_pointer_last++;
			}
			//if there isn't equal sequence in the dictionary
			else
			{
				cur_seq->mis_match_byte = *(buffer_search_pointer_last);
				memcpy(output_buffer + (*output_size), cur_seq, sizeof(Encoded_sequence));
				(*output_size) += sizeof(Encoded_sequence);

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
unsigned char* search_in_dictionary(unsigned char* dict_pointer_first, unsigned char* dict_pointer_last,
	unsigned char* buffer_search_pointer_first, unsigned char* buffer_search_pointer_last)
{
	unsigned char* start_seq_pointer = NULL;
	unsigned char loc_buffer_search = 0;
	unsigned int size_buffer_search = buffer_search_pointer_last - buffer_search_pointer_first;
	//loop that moves all the dictionary
	for (unsigned int i = 0; (dict_pointer_first + i) <= dict_pointer_last; i++)
	{
		//if the current byte in the buffer_search is equal to the current in the dictionary
		if (buffer_search_pointer_first[loc_buffer_search] == *(dict_pointer_first + i))
		{
			if (loc_buffer_search == 0)
			{
				start_seq_pointer = dict_pointer_first + i;
			}

			//the buffer_search is exist in the dictionary
			if (loc_buffer_search == size_buffer_search / sizeof(unsigned char))
			{
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
	}
	return NULL;
}
