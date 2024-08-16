

typedef struct huffman_decode_node {

	unsigned char by_ascii;
	unsigned char left;
	unsigned char right;
}Huffman_decode_node;




#ifndef HUFFMAN_CODE_H
#define HUFFMAN_CODE_H



#ifndef HUFFMAN_H
#define HUFFMAN_H

#include "huffman_struct.h"

int huffman_frequency_compare(const void* elem1, const void* elem2);

void huffman_encode(const unsigned char* data_to_compress, unsigned char* output_buffer, int input_size, int* output_size);
//void huffman_decode(const unsigned char* data_to_decompress, unsigned char** decompressed_data, int* input_size);
Huffman_node_t* huffman_build_tree(Huffman_node_t* nodes, int* start_index, int* last_index);
void huffman_generate_codes(Huffman_node_t* root);
void generate_codes_recursive(Huffman_node_t* node, unsigned int current_code, int current_length);
void huffman_free_tree(Huffman_node_t* nodes, int last_index);

int rescu_metadata(unsigned char* input_buffer, Huffman_decode_node* root);
void huffman_decode(unsigned char* input_buffer, int* input_size, unsigned char* output_buffer);
unsigned char find_ascii_in_tree(unsigned char* input_pointer, Huffman_decode_node* root, int* bits_index);

#endif // HUFFMAN_H
#endif // !HUFFMAN_CODE_H
