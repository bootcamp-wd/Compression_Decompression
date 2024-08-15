#ifndef HUFFMAN_CODE_H
#define HUFFMAN_CODE_H

#include <stdlib.h>
#include <string.h>

typedef struct huffman_decode_node {

	unsigned char by_ascii;
	unsigned char left;
	unsigned char right;
}Huffman_decode_node;

int rescu_metadata(unsigned char* input_buffer, Huffman_decode_node* root);

void huffman_decode(unsigned char* input_buffer, int* input_size, unsigned char* output_buffer);

unsigned char find_ascii_in_tree(unsigned char* input_pointer, Huffman_decode_node* root,
	int* bits_index);


#endif // !HUFFMAN_CODE_H
