#ifndef HUFFMAN_COMMON_H
#define HUFFMAN_COMMON_H

#include "general_define.h"

#define MAX_NODES 511
#define MAX_CODE_LENGTH (MAX_NODES - 1)
#define MAX_CODE_BYTES ((MAX_CODE_LENGTH + 7) / 8)

typedef struct Huffman_node {
    long frequency;//add assert we dont have overflow
    U_08 by_ascii;
    U_08* code; //(bits)
    U_16 code_length;
    struct Huffman_node* left;
    struct Huffman_node* right;
} Huffman_node_t;

#endif // HUFFMAN_COMMON_H