
#ifndef HUFFMAN_CODE_H
#define HUFFMAN_CODE_H

#include "huffman_struct.h"

typedef struct huffman_decode_node {

	unsigned U_08 by_ascii;
	unsigned U_08 left;
	unsigned U_08 right;
}Huffman_decode_node;

U_32 huffman_frequency_compare(const void* elem1, const void* elem2);

void huffman_encode(const unsigned U_08* data_to_compress, unsigned U_08* output_buffer, U_32 input_size, U_32* output_size);
//void huffman_decode(const unsigned U_08* data_to_decompress, unsigned U_08** decompressed_data, U_32* input_size);
Huffman_node_t* huffman_build_tree(Huffman_node_t* nodes, U_32* start_index, U_32* last_index);
void huffman_generate_codes(Huffman_node_t* root);
void generate_codes_recursive(Huffman_node_t* node, unsigned U_32 current_code, U_32 current_length);
void huffman_free_tree(Huffman_node_t* nodes, U_32 last_index);

Huffman_decode_node* rescu_metadata(unsigned U_08* input_buffer, Huffman_decode_node* root);
void huffman_decode(unsigned U_08* input_buffer, U_32* input_size, unsigned U_08* output_buffer,U_32* output_size);
unsigned U_08 find_ascii_in_tree(unsigned U_08* input_poU_32er, Huffman_decode_node* root, U_32* bits_index);
void find_ascii_last_byte(U_08 last_byte, U_08 sum_bites, Huffman_decode_node* root,
	U_32 bites_index, unsigned U_08* output_poU_32er,U_32* output_size);
#endif // !HUFFMAN_CODE_H
