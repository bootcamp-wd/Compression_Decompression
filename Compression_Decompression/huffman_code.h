
#ifndef HUFFMAN_CODE_H
#define HUFFMAN_CODE_H

#include "huffman_struct.h"
#include "general_define.h"

typedef struct huffman_decode_node {

	U_08 by_ascii;
	U_16 left;
	U_16 right;
}Huffman_decode_node;

int huffman_frequency_compare(const void* elem1, const void* elem2);

void huffman_encode(const U_08* data_to_compress, U_08* output_buffer, int input_size, int* output_size);
//void huffman_decode(const U_08* data_to_decompress, U_08** decompressed_data, int* input_size);
Huffman_node_t* huffman_build_tree(Huffman_node_t* nodes, int* start_index, int* last_index);
void huffman_generate_codes(Huffman_node_t* root);
void generate_codes_recursive(Huffman_node_t* node, U_32 current_code, int current_length);
void huffman_free_tree(Huffman_node_t* nodes, int last_index);

Huffman_decode_node* rescu_metadata(U_08* input_buffer, Huffman_decode_node* root);
void huffman_decode(U_08* input_buffer, int* input_size, U_08* output_buffer);
U_08 find_ascii_in_tree(U_08* input_pointer, Huffman_decode_node* root, int* bits_index);
void find_ascii_last_byte(char last_byte, char sum_bites, Huffman_decode_node* root,int bites_index, U_08* output_pointer);

#endif // !HUFFMAN_CODE_H
