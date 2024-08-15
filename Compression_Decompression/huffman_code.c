#include "huffman_code.h"
#

/***************************************************************************
 *                            FIND_ASCII_IN_TREE FUNCTION
 * Name         : find the ascii codes according to the tree
 * Parameters   : input_pointer - pointer to the input data buffer
 *                root - tree that stores Hoffman's codes according to the ascii characters
 *                bits_index - keep the index of bits in byte
 * Returned     : the found character
 *
 ***************************************************************************/
unsigned char find_ascii_in_tree(unsigned char** input_pointer, Huffman_decode_node root,
	int* bits_index) {

	unsigned char mask;
	//move over the tree according to the received bits until finding leaves where the ascii code 
	while (root.byte == 0) {

		;//Moving the mask according to the index
		mask = 0b10000000 >> (*bits_index % 8);

		if ((**input_pointer & mask) == 0) {
			root = *(root.right);
		}
		else {
			root = *(root.left);
		}
		(*bits_index)++;
		//advance the pointer of the bytes
		if (*bits_index % 8 == 0) {
			(*input_pointer)++;
		}
	}

	return root.byte;
}
/***************************************************************************
 *                           HUFFMAN_DECODE FUNCTION
 * Name         : decode the data with huffman decompression algorithm
 * Parameters   : data_to_decompress - pointer to the input data buffer
 *                input_size - size of the input data.
 *                decompressed_data - pointer to the output data buffer to fill it
 *                root - tree that stores Hoffman's codes according to the ascii characters
 * Returned     : none
 *
 ***************************************************************************/
void huffman_decode(unsigned char* data_to_decompress, unsigned char** decompressed_data, int* input_size,
	Huffman_decode_node root)
{
	unsigned char* input_pointer;
	unsigned char* output_pointer;

	input_pointer = data_to_decompress;
	output_pointer = decompressed_data;

	int bits_index = 0;

	while (bits_index < *input_size * 8) {

		unsigned char decompressed_byte = find_ascii_in_tree(&input_pointer, root, &bits_index);

		//write to output buffer the decomprresed byte
		*output_pointer = decompressed_byte;
		output_pointer++;

	}

}

