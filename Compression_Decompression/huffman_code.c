#include "huffman_code.h"


/***************************************************************************
 *                           HUFFMAN_DECODE FUNCTION
 * Name         : decode the data with huffman decompression algorithm
 * Parameters   : input_buffer - pointer to the input data and meta data buffer
 *                input_size - size of the input data and meta data.
 *                output_buffer - pointer to the output data buffer to fill it
 * Returned     : none
 *
 ***************************************************************************/
void huffman_decode(unsigned char* input_buffer, int* input_size, unsigned char* output_buffer)
{
	Huffman_decode_node* root = NULL;

	int metadata_size;
	metadata_size = rescu_metadata(input_buffer, root);

	*input_size -= metadata_size;

	unsigned char* input_pointer;
	unsigned char* output_pointer;

	input_pointer = input_buffer;
	output_pointer = output_buffer;

	int bits_index = 0;

	while (bits_index < *input_size * 8) {

		unsigned char decompressed_byte = find_ascii_in_tree(input_pointer, root, &bits_index);

		//write to output buffer the decomprresed byte
		*output_pointer = decompressed_byte;
		output_pointer++;
		free(root);//free the tree
	}
}
	/***************************************************************************
 *                           RESCU_DATA FUNCTION
 * Name         : rescu the metadata from the input buffer and return the metadata size;
 * Parameters   : input_buffer - pointer to the input data buffer
 *                root - pointer to array to keep the huffman codes 
 * Returned     : metadata size
 *
 ***************************************************************************/
	int rescu_metadata(unsigned char* input_buffer, Huffman_decode_node* root)
	{
		unsigned int nodes_length = *input_buffer;
		input_buffer += sizeof(int); //move the pointer
		root = (Huffman_decode_node*)malloc((nodes_length * sizeof(Huffman_decode_node)));
		for (unsigned int i = 0; i < nodes_length; i++) {
			memcpy(root+(i*sizeof(Huffman_decode_node)), input_buffer, sizeof(Huffman_decode_node));
			input_buffer += sizeof(Huffman_decode_node);
		}
		return sizeof(int) + nodes_length * sizeof(Huffman_decode_node);//size of the metadata
	}
/***************************************************************************
 *                            FIND_ASCII_IN_TREE FUNCTION
 * Name         : find the ascii codes according to the tree
 * Parameters   : input_pointer - pointer to the input data buffer
 *                root - tree that stores Hoffman's codes according to the ascii characters
 *                bits_index - keep the index of bits in byte
 * Returned     : the found character
 *
 ***************************************************************************/
	unsigned char find_ascii_in_tree(unsigned char* input_pointer, Huffman_decode_node* root,int* bits_index) 
	{
		Huffman_decode_node* nodes;
		nodes = root;
	unsigned char mask;
	//move over the tree according to the received bits until finding leaves where the ascii code 
	while (root->by_ascii == 0) {

		;//Moving the mask according to the index
		mask = 0b10000000 >> (*bits_index % 8);

		if ((*input_pointer & mask) == 0) {
			root = &nodes[root->right-'0'];
		}
		else {
			root = &nodes[root->left-'0'];
		}
		(*bits_index)++;
		//advance the pointer of the bytes
		if (*bits_index % 8 == 0) {
			input_pointer++;
		}
	}

	return root->by_ascii;
}

