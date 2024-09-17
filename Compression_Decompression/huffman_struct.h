#ifndef HUFFMAN_COMMON_H
#define HUFFMAN_COMMON_H

#include "general_define.h"

typedef struct Huffman_node {
    U_08 by_ascii;
    long frequency;//add assert we dont have overflow
    U_08* code; //(bits)
    U_16 code_length;
    struct Huffman_node* left;
    struct Huffman_node* right;
} Huffman_node_t;

#endif // HUFFMAN_COMMON_H