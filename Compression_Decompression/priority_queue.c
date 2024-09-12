#include "priority_queue.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/***************************************************************************
 *                            EXTRACT MIN FUNCTION
 * Name         : extract_min - return the node with the minimum frequency
 * Parameters   : min_leaf - pointer to the pointer of the minimum leaf node
 *                min_parent - pointer to the pointer of the minimum parent node
 * Returned     : pointer to the node with the minimum frequency
 * Time : o(1)
 ***************************************************************************/
Huffman_node_t* extract_min(Huffman_node_t** min_leaf, Huffman_node_t** min_parent, Huffman_node_t* first_parent) {
    assert(min_leaf != NULL);
    assert(min_parent != NULL);
    assert(*min_leaf != NULL);
    assert(*min_parent != NULL);

    Huffman_node_t* min_freq_node;

    if (((*min_leaf) != first_parent) && (!((*min_parent)->frequency) || (*min_leaf)->frequency <= (*min_parent)->frequency)) {

        min_freq_node = (*min_leaf);
        (*min_leaf)++;
    }
    else {
        min_freq_node = (*min_parent);
        (*min_parent)++;
    }
    return min_freq_node;
}

/***************************************************************************
 *                            PRIORITY QUEUE FUNCTION
 * Name         : push - insert a node into the priority queue
 * Parameters   : pq - pointer to the priority queue
 *                size - pointer to the size of the priority queue - the last parent
 *                node - pointer to the node to be inserted
 * Returned     : none
 * Time : o(1)
 ***************************************************************************/
void priority_queue_push(Huffman_node_t* pq, int* size, Huffman_node_t* node)
{
    assert(pq != NULL);
    assert(size != NULL);
    assert(node != NULL);
    pq[(*size)++] = *node;
    /*free(node);*/
}