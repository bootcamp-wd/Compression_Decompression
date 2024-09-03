#ifndef HUFFMAN_COMMON_H
#define HUFFMAN_COMMON_H

typedef struct Huffman_node {
    unsigned char by_ascii;
    long frequency;//add assert we dont have overflow
    unsigned char* code; //(bits)
    int code_length;
    struct Huffman_node* left;
    struct Huffman_node* right;
} Huffman_node_t;

#endif // HUFFMAN_COMMON_H