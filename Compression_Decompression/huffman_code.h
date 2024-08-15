#ifndef HUFFMAN_CODE_H
#define HUFFMAN_CODE_H

#include "general_defines.c"
typedef struct huffman_decode_node {
	struct huffman_decode_node* left;
	struct huffman_decode_node* right;
	char byte;
}Huffman_decode_node;

unsigned char find_ascii_in_tree(unsigned char** input_pointer, Huffman_decode_node root,
	int* bits_index);

void huffman_decode(unsigned char* data_to_decompress,
	unsigned char** decompressed_data, int* input_size, Huffman_decode_node root);



#endif // !HUFFMAN_CODE_H
