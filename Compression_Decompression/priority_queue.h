#ifndef PRIORITY_QUEUUE
#define PRIORITY_QUEUUE

#include "huffman_struct.h"

Huffman_node_t* extract_min(Huffman_node_t** min_leaf, Huffman_node_t** min_parent, Huffman_node_t* first_parent);
void priority_queue_push(Huffman_node_t* pq, int* size, Huffman_node_t node);


#endif // PRIORITY_QUEUUE