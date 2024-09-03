
#ifndef HUFFMAN_CODE_H
#define HUFFMAN_CODE_H

#include "huffman_struct.h"
#include "general_define.h"

typedef struct huffman_decode_node {
	U_16 left;
	U_16 right;
	U_08 by_ascii;
}Huffman_decode_node;

typedef struct huffman_metadata {
	U_08 tree_length;
	Huffman_decode_node* nodes;
}Huffman_metadata;

int huffman_frequency_compare(const void* elem1, const void* elem2);
int huffman_ascii_compare(const void* elem1, const void* elem2);

void initialize_nodes(Huffman_node_t* nodes);
void count_frequencies(const U_08* data_to_compress, U_32 input_size, Huffman_node_t* nodes);
void store_metadata(Huffman_node_t* nodes, U_32 start_index, U_32 last_index, Huffman_metadata* metadata);
void encode_data(const U_08* data_to_compress, U_32 input_size, Huffman_node_t* nodes, U_08* compressed_data, U_32* compressed_data_bit_index);
void finalize_compressed_data(U_08* compressed_data, U_32 compressed_data_bit_index);

void huffman_encode(const U_08* data_to_compress, U_08* output_buffer, U_32 input_size, U_32* output_size);
Huffman_node_t* huffman_build_tree(Huffman_node_t* nodes, int* start_index, int* last_index);
void huffman_generate_codes(Huffman_node_t* root);
void generate_codes_recursive(Huffman_node_t* node, U_32 current_code, int current_length);
void huffman_free_tree(Huffman_node_t* nodes, int last_index);

Huffman_decode_node* rescu_metadata(U_08* input_buffer,int* tree_length);
void huffman_decode(U_08* input_buffer, int* input_size, U_08* output_buffer);
U_08 find_ascii_in_tree(U_08* input_pointer, Huffman_decode_node* root, int* bits_index,int tree_length);
void find_ascii_last_byte(U_08 last_byte, U_08 sum_bites, Huffman_decode_node* root, U_32 bites_index, U_08* output_pointer);

#endif // !HUFFMAN_CODE_H
