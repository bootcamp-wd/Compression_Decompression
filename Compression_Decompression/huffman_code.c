#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "huffman_code.h"
#include "priority_queue.h"

#define ASCII_SIZE 256

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
    if (a->frequency != b->frequency) {
        return a->frequency - b->frequency;
    }

    // If frequencies are equal, compare by ASCII value
    return a->by_ascii - b->by_ascii;
}


int huffman_ascii_compare(const void* elem1, const void* elem2) {
    Huffman_node_t* nodes_to_compare[2] = { (Huffman_node_t*)elem1, (Huffman_node_t*)elem2 };
    return nodes_to_compare[0]->by_ascii - nodes_to_compare[1]->by_ascii;
}

/***************************************************************************
 *                            HUFFMAN ENCODE FUNCTION
 * Name         : huffman_encode - encode data using Huffman coding
 * Parameters   : data_to_compress - pointer of the buffer to compress
 *                compressed_data - pointer of the compressed data buffer
 *                compression_metadata - pointer of the compression metadata buffer
 *                input_size - size of the input data (data to compress)
 * Returned     : none
 ***************************************************************************/
//output_buffer//
void huffman_encode(const unsigned char* data_to_compress, unsigned char* output_buffer, int input_size , int* output_size)
{
    assert(data_to_compress != NULL);
    assert(output_buffer != NULL);
    assert(input_size >= 0);

    *output_size = 0;
    if (input_size == 0) {
        return;
    }

    Huffman_node_t nodes[511] = {0};
    int start_index = 0;
    int last_index = 0;

    // Initialize ascii codes
    for (int ascii_index = 0; ascii_index < ASCII_SIZE; ascii_index++)
    {
        nodes[ascii_index].by_ascii = ascii_index;
    }
    // Add ascii frequencies
    for (int index = 0; index < input_size; index++)
    {
        int ascii_byte = data_to_compress[index];
        nodes[ascii_byte].frequency++;
    }
    // Sort ascii chars depending on frequency
    qsort(nodes, ASCII_SIZE, sizeof(Huffman_node_t), huffman_frequency_compare);
    Huffman_node_t* root = huffman_build_tree(nodes,&start_index,&last_index);
    huffman_generate_codes(root);

    unsigned char* compression_metadata = output_buffer;
    *compression_metadata = last_index-start_index + 1;//it is byte because we know it is aximum 511
    compression_metadata += sizeof(unsigned char);

    //by_ascii , left , right
    
    printf("Before memcpy: input_size = %d\n", input_size);
    printf("compression_metadata: %p\n", (void*)compression_metadata);
    // Store Huffman tree nodes in metadata
    for (int i = start_index; i <= last_index; i++) {
        //memcpy(compression_metadata, &nodes[i], sizeof(Huffman_node_t));
        //compression_metadata += sizeof(Huffman_node_t);

        *compression_metadata = nodes[i].by_ascii;
        compression_metadata += sizeof(unsigned char);

        *compression_metadata = (nodes[i].left - nodes) - start_index;
        compression_metadata += sizeof(unsigned char);

        *compression_metadata = (nodes[i].right - nodes) - start_index;
        compression_metadata += sizeof(unsigned char);

    }
    printf("After memcpy: input_size = %d\n", input_size);

    // Sort nodes by ASCII value for efficient encoding
    qsort(nodes, ASCII_SIZE, sizeof(Huffman_node_t), huffman_ascii_compare);

    unsigned char* compressed_data = compression_metadata;
    int compressed_data_bit_index = 0;
    // Encode the data to compressed_data buffer
    for (int i = 0; i < input_size; i++) {
        
        unsigned char* code = nodes[data_to_compress[i]].code;
        int code_length = nodes[data_to_compress[i]].code_length;

        for (int j = 0; j < code_length; j++) {
            if (!(compressed_data_bit_index % 8)) {
                compressed_data[compressed_data_bit_index / 8] = 0;
            }
            if (code[j / 8] & (1 << (7 - (j % 8)))) {
                compressed_data[compressed_data_bit_index / 8] |= (1 << (7 - (compressed_data_bit_index % 8)));
            }
            compressed_data_bit_index++;
        }
    }
    //Add the remaining bit
    compressed_data[(compressed_data_bit_index - 1) / 8 + 1] = (compressed_data_bit_index ) % 8 + '0';
   
    // Print the compressed data as bits
    for (int i = 0; i < compressed_data_bit_index; i++) {
        if (compressed_data[i / 8] & (1 << (7 - (i % 8)))) {
            printf("1");
        }
        else {
            printf("0");
        }
    }
    printf("\n");

    *output_size = (compressed_data_bit_index + 7) / 8 + 1;  // +1 for the remaining bits byte
    huffman_free_tree(nodes , last_index);
}

void huffman_decode(const unsigned char* data_to_decompress, unsigned char** decompressed_data, int* input_size)
{
}

/***************************************************************************
 *                            BUILD TREE FUNCTION
 * Name         : build_tree - build the Huffman tree
 * Parameters   : nodes - array of Huffman nodes combine the priority queue
 * Returned     : pointer to the root of the Huffman tree
 ***************************************************************************/
Huffman_node_t* huffman_build_tree(Huffman_node_t* nodes ,int* start_index , int* last_index)
{
    Huffman_node_t* min_leaf = nodes;//add the frequncy to the name
    Huffman_node_t* min_parent = nodes + ASCII_SIZE ;//add the frequncy to the name
    int current_parent_index = ASCII_SIZE;

    while (min_leaf->frequency == 0) {
        min_leaf++;
        (*start_index)++;
    }

    int flag = 1;
    while ( min_parent - nodes < 511 && flag)
    {
        Huffman_node_t* min_node1 = extract_min(&min_leaf,&min_parent,&nodes[ASCII_SIZE]);
        Huffman_node_t* min_node2 = extract_min(&min_leaf,&min_parent,&nodes[ASCII_SIZE]);

        if (!min_node2->frequency) {
            flag = 0;
        }
        else {
            Huffman_node_t* parent = malloc(sizeof(Huffman_node_t));
            parent->left = min_node1;
            parent->right = min_node2;
            parent->frequency = min_node1->frequency + min_node2->frequency;

            printf("Pushing parent node with frequency: %d at index: %d\n", parent->frequency, current_parent_index);
            priority_queue_push(nodes, &current_parent_index, parent);
            printf("After push, nodes[%d].frequency = %d\n", current_parent_index-1, nodes[current_parent_index-1].frequency);
        }
    }
    *last_index = current_parent_index-1;
    return &nodes[current_parent_index - 1];
}

/***************************************************************************
 *                            GENERATE CODES FUNCTION
 * Name         : generate_codes - generate Huffman codes for each character
 * Parameters   : root - pointer to the root of the Huffman tree
 * Returned     : none
 ***************************************************************************/
void huffman_generate_codes(Huffman_node_t* root) {
    generate_codes_recursive(root, 0, 0);
}

void generate_codes_recursive(Huffman_node_t* node, unsigned int current_code, int current_length) {
    assert(node != NULL);

    if (!node->left && !node->right) {  // Leaf node
        // Calculate the number of bytes needed to store the bits
        int num_bytes = (current_length + 7) / 8;
        node->code = malloc(num_bytes);
        assert(node->code != NULL);

        // Initialize code memory
        memset(node->code, 0, num_bytes);

        // Store the bits of current_code into node->code
        for (int i = 0; i < current_length; i++) {
            if (current_code & (1 << (current_length - i - 1))) {
                //the sequnce of the relevant bits will start from left
                node->code[i / 8] |= (1 << (7 - (i % 8)));
            }
        }

        node->code_length = current_length;  
        return;
    }

    // Traverse left (append '0' bit)
    if (node->left) {
        generate_codes_recursive(node->left, current_code << 1, current_length + 1);
    }

    // Traverse right (append '1' bit)
    if (node->right) {
        generate_codes_recursive(node->right, (current_code << 1) | 1, current_length + 1);
    }
}

void huffman_free_tree(Huffman_node_t* nodes, int last_index) {
    for (int i = ASCII_SIZE; i <= last_index; i++) {
        if (nodes[i].left != NULL || nodes[i].right != NULL) {
            //free(nodes[i].code);  // Free the code allocated in `generate_codes_recursive`
            free(nodes+i);      // Free the parent node itself
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
void huffman_decode(unsigned char* input_buffer, int* input_size, unsigned char* output_buffer)
{
	Huffman_decode_node* root = NULL;

	int metadata_size;
	metadata_size = rescu_metadata(input_buffer, root);

	*input_size -= metadata_size;

	unsigned char* input_pointer;
	unsigned char* output_pointer;

	input_pointer = input_buffer;
	output_pointer = output_buffer;

	int bits_index = 0;

	while (bits_index < *input_size * 8) {

		unsigned char decompressed_byte = find_ascii_in_tree(input_pointer, root, &bits_index);

		//write to output buffer the decomprresed byte
		*output_pointer = decompressed_byte;
		output_pointer++;
		free(root);//free the tree
	}
}
	/***************************************************************************
 *                           RESCU_DATA FUNCTION
 * Name         : rescu the metadata from the input buffer and return the metadata size;
 * Parameters   : input_buffer - pointer to the input data buffer
 *                root - pointer to array to keep the huffman codes 
 * Returned     : metadata size
 *
 ***************************************************************************/
	int rescu_metadata(unsigned char* input_buffer, Huffman_decode_node* root)
	{
		unsigned int nodes_length = *input_buffer;
		input_buffer += sizeof(int); //move the pointer
		root = (Huffman_decode_node*)malloc((nodes_length * sizeof(Huffman_decode_node)));
		for (unsigned int i = 0; i < nodes_length; i++) {
			memcpy(root+(i*sizeof(Huffman_decode_node)), input_buffer, sizeof(Huffman_decode_node));
			input_buffer += sizeof(Huffman_decode_node);
		}
		return sizeof(int) + nodes_length * sizeof(Huffman_decode_node);//size of the metadata
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
	unsigned char find_ascii_in_tree(unsigned char* input_pointer, Huffman_decode_node* root,int* bits_index) 
	{
		Huffman_decode_node* nodes;
		nodes = root;
	    unsigned char mask;
	    //move over the tree according to the received bits until finding leaves where the ascii code 
	    while (root->by_ascii == 0) 
        {
		    //Moving the mask according to the index
		    mask = 0b10000000 >> (*bits_index % 8);

		    if ((*input_pointer & mask) == 0) {
			    root = &nodes[root->right-'0'];
		    }
		    else {
			    root = &nodes[root->left-'0'];
		    }
		    (*bits_index)++;
		    //advance the pointer of the bytes
		    if (*bits_index % 8 == 0) {
			    input_pointer++;
		    }
	}

	return root->by_ascii;
}

