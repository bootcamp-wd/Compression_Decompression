#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "huffman_code.h"
#include "priority_queue.h"

#define ASCII_SIZE 256
#define INVALID_INDEX 512
#define NODES_IN_TREE 511

/***************************************************************************
 *                           FREQUENCY COMPARE FUNCTION
 * Name         : frequency_compare - compare the frequency of two Huffman nodes
 * Parameters   : elem1 - pointer to the first element
 *                elem2 - pointer to the second element
 * Returned     : comparison result (compare by ascii if equal, 1 if elem1 < elem2, -1 otherwise)
 ***************************************************************************/
int huffman_frequency_compare(const void* elem1, const void* elem2)
{
    assert(elem1 != NULL);
    assert(elem2 != NULL);

    const Huffman_node_t* a = (const Huffman_node_t*)elem1;
    const Huffman_node_t* b = (const Huffman_node_t*)elem2;

    // First, compare by frequency
    if (a->frequency != b->frequency)
    {
        return a->frequency - b->frequency;
    }

    // If frequencies are equal, compare by ASCII value
    return a->by_ascii - b->by_ascii;
}

int huffman_ascii_compare(const void* elem1, const void* elem2)
{
    Huffman_node_t* nodes_to_compare[2] = { (Huffman_node_t*)elem1, (Huffman_node_t*)elem2 };
    return nodes_to_compare[0]->by_ascii - nodes_to_compare[1]->by_ascii;
}

void initialize_nodes(Huffman_node_t* nodes)
{
    for (U_32 ascii_index = 0; ascii_index < ASCII_SIZE; ascii_index++)
    {
        nodes[ascii_index].by_ascii = ascii_index;
    }
}

void count_frequencies(const U_08* data_to_compress, U_32 input_size, Huffman_node_t* nodes)
{
    for (U_32 index = 0; index < input_size; index++)
    {
        U_32 ascii_byte = data_to_compress[index];
        nodes[ascii_byte].frequency++;
    }
}

void store_metadata(Huffman_node_t* nodes, U_32 start_index, U_32 last_index, Huffman_metadata* metadata)
{
    for (U_32 i = start_index, j = 0; i <= last_index; i++, j++)
    {
        metadata->nodes[j].by_ascii = nodes[i].by_ascii;
        metadata->nodes[j].left = nodes[i].left ? (U_16)((nodes[i].left - nodes) - start_index) : INVALID_INDEX;
        metadata->nodes[j].right = nodes[i].right ? (U_16)((nodes[i].right - nodes) - start_index) : INVALID_INDEX;
    }
}

void encode_data(const U_08* data_to_compress, U_32 input_size, Huffman_node_t* nodes, U_08* compressed_data, U_32* compressed_data_bit_index)
{
    for (U_32 i = 0; i < input_size; i++)
    {
        U_08* code = nodes[data_to_compress[i]].code;
        U_32 code_length = nodes[data_to_compress[i]].code_length;

        for (U_32 j = 0; j < code_length; j++)
        {
            if (!(*compressed_data_bit_index % 8))
            {
                compressed_data[*compressed_data_bit_index / 8] = 0;
            }
            if (code[j / 8] & (1 << (7 - (j % 8))))
            {
                compressed_data[*compressed_data_bit_index / 8] |= (1 << (7 - (*compressed_data_bit_index % 8)));
            }
            (*compressed_data_bit_index)++;
        }
    }
}

void finalize_compressed_data(U_08* compressed_data, U_32 compressed_data_bit_index)
{
    compressed_data[compressed_data_bit_index / 8 + 1] = 0;
    // Handle remaining bits in the last byte
    if (compressed_data_bit_index % 8 != 0)
    {
        compressed_data[compressed_data_bit_index / 8 + 1] = (compressed_data_bit_index % 8);
    }


    // Print the compressed data as bits (for debugging)
    for (U_32 i = 0; i < (compressed_data_bit_index + 12); i++)
    {
<<<<<<< HEAD
        printf("%d", !!(compressed_data[i / 8] & (1 << (7 - (i % 8)))) );
=======
        printf("%d", !!(compressed_data[i / 8] & (1 << (7 - (i % 8)))));
>>>>>>> b505dfd715dce8a4402a2871848419b66d97e7dd
    }
}

/***************************************************************************
 *                            HUFFMAN ENCODE FUNCTION
 * Name         : huffman_encode - encode data using Huffman coding
 * Parameters   : data_to_compress - pointer of the input buffer to compress
 *                output_buffer - pointer of the compressed data buffer
 *                input_size - size of the input buffer (data to compress)
 *                output_size - pointer to the final output buffer size
 * Returned     : none
 ***************************************************************************/
void huffman_encode(const U_08* data_to_compress, U_08* output_buffer, U_32 input_size, U_32* output_size)
{
    assert(data_to_compress != NULL);
    assert(output_buffer != NULL);
    assert(input_size >= 0);

    *output_size = 0;
    if (input_size == 0)
    {
        return;
    }

    Huffman_node_t nodes[NODES_IN_TREE] = { 0 };
    U_32 start_index = 0;
    U_32 last_index = 0;

    initialize_nodes(nodes);
    count_frequencies(data_to_compress, input_size, nodes);
    qsort(nodes, ASCII_SIZE, sizeof(Huffman_node_t), huffman_frequency_compare);

    Huffman_node_t* root = huffman_build_tree(nodes, &start_index, &last_index);
    huffman_generate_codes(root);

    // Calculate the total size needed for metadata and nodes
    size_t metadata_size = sizeof(Huffman_metadata);
    size_t nodes_size = sizeof(Huffman_decode_node) * (last_index - start_index + 1);
    size_t total_size = metadata_size + nodes_size;

    // Ensure the buffer is large enough
    assert(total_size <= output_size);//suppose to be *output_size

    // Point to the start of the buffer for the metadata
    Huffman_metadata* compression_metadata = (Huffman_metadata*)output_buffer;

    // Point the nodes array to the location right after the metadata in the buffer
    compression_metadata->nodes = (Huffman_decode_node*)(output_buffer + metadata_size);

    // Set the tree_length
    compression_metadata->tree_length = last_index - start_index + 1;

    store_metadata(nodes, start_index, last_index, compression_metadata);
    qsort(nodes, ASCII_SIZE, sizeof(Huffman_node_t), huffman_ascii_compare);

    //Calculate the start of compressed data
    U_08* compressed_data = (U_08*)(compression_metadata->nodes + compression_metadata->tree_length);
    U_32 compressed_data_bit_index = 0;

    encode_data(data_to_compress, input_size, nodes, compressed_data, &compressed_data_bit_index);
    finalize_compressed_data(compressed_data, compressed_data_bit_index);

    *output_size = (compressed_data_bit_index + 7) / 8 + 1;  // +1 for the remaining bits byte
    //failure here:
    huffman_free_tree(nodes, last_index);
}

/***************************************************************************
 *                            BUILD TREE FUNCTION
 * Name         : build_tree - build the Huffman tree
 * Parameters   : nodes - array of Huffman nodes combine the priority queue
 *                start_index - pointer to index of the first value (contain values) in the nodes array
 *                last_index - pointer to index of the last value (contain values) in the nodes array
 * Returned     : pointer to the root of the Huffman tree
 ***************************************************************************/
Huffman_node_t* huffman_build_tree(Huffman_node_t* nodes, U_32* start_index, U_32* last_index)
{
    Huffman_node_t* min_leaf = nodes;//add the frequncy to the name
    Huffman_node_t* min_parent = nodes + ASCII_SIZE;//add the frequncy to the name
    U_32 current_parent_index = ASCII_SIZE;

    while (min_leaf->frequency == 0)
    {
        min_leaf++;
        (*start_index)++;
    }

    int flag = 1;
    while (min_parent - nodes < NODES_IN_TREE && flag)
    {
        Huffman_node_t* min_node1 = extract_min(&min_leaf, &min_parent, &nodes[ASCII_SIZE]);
        Huffman_node_t* min_node2 = extract_min(&min_leaf, &min_parent, &nodes[ASCII_SIZE]);

        if (!min_node2->frequency)
        {
            flag = 0;
        }
        else
        {
            Huffman_node_t* parent = malloc(sizeof(Huffman_node_t));
            parent->left = min_node1;
            parent->right = min_node2;
            parent->frequency = min_node1->frequency + min_node2->frequency;

            printf("Pushing parent node with frequency: %d at index: %d\n", parent->frequency, current_parent_index);
            priority_queue_push(nodes, &current_parent_index, parent);
            printf("After push, nodes[%d].frequency = %d\n", current_parent_index - 1, nodes[current_parent_index - 1].frequency);
        }
    }
    *last_index = current_parent_index - 1;
    return &nodes[current_parent_index - 1];
}

/***************************************************************************
 *                            GENERATE CODES FUNCTION
 * Name         : generate_codes - generate Huffman codes for each character
 * Parameters   : root - pointer to the root of the Huffman tree
 * Returned     : none
 ***************************************************************************/
void huffman_generate_codes(Huffman_node_t* root)
{
    generate_codes_recursive(root, 0, 0);
}

void generate_codes_recursive(Huffman_node_t* node, U_32 current_code, U_32 current_length)
{
    assert(node != NULL);

    if (!node->left && !node->right) // Leaf node
    {
        // Calculate the number of bytes needed to store the bits
        U_32 num_bytes = (current_length + 7) / 8;
        node->code = malloc(num_bytes);
        assert(node->code != NULL);

        // Initialize code memory
        memset(node->code, 0, num_bytes);

        // Store the bits of current_code into node->code
        for (U_32 i = 0; i < current_length; i++)
        {
            if (current_code & (1 << (current_length - i - 1)))
            {
                //the sequnce of the relevant bits will start from left
                node->code[i / 8] |= (1 << (7 - (i % 8)));
            }
        }

        node->code_length = current_length;
        return;
    }

    // Traverse left (append '0' bit)
    if (node->left)
    {
        generate_codes_recursive(node->left, current_code << 1, current_length + 1);
    }

    // Traverse right (append '1' bit)
    if (node->right)
    {
        generate_codes_recursive(node->right, (current_code << 1) | 1, current_length + 1);
    }
}

void huffman_free_tree(Huffman_node_t* nodes, U_32 last_index)
{
    //free(nodes[i].code);  // Free the code allocated in `generate_codes_recursive`
    //    free(nodes+i);      // Free the parent node itself
    for (U_32 i = ASCII_SIZE; i <= last_index; i++)
    {
        if (nodes[i].code)
        {
            free(nodes[i].code);
        }
        else
        {
            free(nodes + i);
        }
    }
}

/***************************************************************************
 *                           HUFFMAN_DECODE FUNCTION
 * Name         : decode the data with huffman decompression algorithm
 * Parameters   : input_buffer - pointer to the input data and meta data buffer
 *                input_size - size of the input data and meta data.
 *                output_buffer - pointer to the output data buffer to fill it
 * Returned     : none
 *
 ***************************************************************************/
void huffman_decode(U_08* input_buffer, int* input_size, U_08* output_buffer)
{
    Huffman_decode_node* root = NULL;

    int metadata_size;
    root = rescu_metadata(input_buffer, root);

    //check
       // printf("root,%c", root->by_ascii);


    printf("%d input_size", *input_size);
    //*input_size -= metadata_size;

    U_08* input_pointer;
    U_08* output_pointer;

    input_pointer = input_buffer;
    output_pointer = output_buffer;

    int bits_index = 0;

    while (bits_index < ((*input_size) - 2) * 8)
    {

        U_08 decompressed_byte = find_ascii_in_tree(input_pointer, root, &bits_index);

        //write to output buffer the decomprresed byte
        *output_pointer = decompressed_byte;
        output_pointer++;
    }
    find_ascii_last_byte(*input_pointer, *(++input_pointer), root, bits_index, output_pointer);
    /*free(root)*/;//free the tree
}

/***************************************************************************
 *                           RESCU_METADATA FUNCTION
 * Name         : rescu the metadata from the input buffer and return the metadata size;
 * Parameters   : input_buffer - pointer to the input data buffer
 *                root - pointer to array to keep the huffman codes
 * Returned     : metadata size
 *
 ***************************************************************************/
Huffman_decode_node* rescu_metadata(U_08* input_buffer, Huffman_decode_node* root)
{
    U_32 decode_node_size = sizeof(Huffman_decode_node);
    U_32 nodes_length = *input_buffer;
    printf("%c", *input_buffer);
    input_buffer += 1;
    printf("%ui", *input_buffer);
    //move the pointer to the end of the input_file
    input_buffer += sizeof(U_32) + (nodes_length * decode_node_size);

    root = (Huffman_decode_node*)malloc((nodes_length * decode_node_size));
    for (U_32 i = nodes_length, j = 0; i > 0; i--, j += decode_node_size) {
        memcpy(root + j, input_buffer - decode_node_size, decode_node_size);
        input_buffer -= decode_node_size;
        printf("root:%c", (root + j)->by_ascii);
        printf("left:%c", (root + j)->left);
        printf("right:%c\n", (root + j)->right);
    }

    return root;
}

/***************************************************************************
 *                            FIND_ASCII_IN_TREE FUNCTION
 * Name         : find the ascii codes according to the tree
 * Parameters   : input_pointer - pointer to the input data buffer
 *                root - tree that stores Hoffman's codes according to the ascii characters
 *                bits_index - keep the index of bits in byte
 * Returned     : the found character
 *
 ***************************************************************************/
U_08 find_ascii_in_tree(U_08* input_pointer, Huffman_decode_node* root, U_32* bits_index)
{
    Huffman_decode_node* nodes;
    nodes = root;
    U_08 mask;
    //move over the tree according to the received bits until finding leaves where the ascii code 
    while (nodes->left == NULL && nodes->right == NULL)
    {
        //Moving the mask according to the index
        mask = 0b10000000 >> (*bits_index % 8);

        if ((*input_pointer & mask) == 0) {
            nodes = &root[nodes->right - '0'];
        }
        else {
            nodes = &root[nodes->left - '0'];
        }
        (*bits_index)++;
        //advance the pointer of the bytes
        if (*bits_index % 8 == 0) {
            input_pointer++;
        }
    }
    printf("the ascii:%c\n", (nodes->by_ascii));
    return nodes->by_ascii;
}

void find_ascii_last_byte(U_08 last_byte, U_08 sum_bites, Huffman_decode_node* root, U_32 bites_index, U_08* output_pointer)
{
    U_32 sum_bites_in_last_byte = sum_bites - '0';
    Huffman_decode_node* nodes;
    nodes = root;
    U_08 mask;
    for (U_32 i = bites_index % 8; i < sum_bites_in_last_byte;)
    {
        //move over the tree according to the received bits until finding leaves where the ascii code 
        while (nodes->left == NULL && nodes->right == NULL)
        {
            //Moving the mask according to the index
            mask = 0b10000000 >> (bites_index % 8);

            if ((last_byte & mask) == 0) {
                nodes = &root[nodes->right - '0'];
            }
            else {
                nodes = &root[nodes->left - '0'];
            }
            i++;
        }
        output_pointer = nodes->by_ascii;
        output_pointer++;
    }
}