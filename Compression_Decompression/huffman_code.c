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
 * Parameters   : data_to_compress - pointer of the input buffer to compress
 *                output_buffer - pointer of the compressed data buffer
 *                input_size - size of the input buffer (data to compress)
 *                output_size - pointer to the final output buffer size
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
    //numberr of nodes
    *compression_metadata = last_index-start_index + 1;// byte because known it is maximum 511
    compression_metadata += sizeof(unsigned char);
    
    printf("Before memcpy: input_size = %d\n", input_size);
    printf("compression_metadata: %p\n", (void*)compression_metadata);

    // Store Huffman tree nodes in metadata
    for (int i = start_index; i <= last_index; i++) {
        // Store the ASCII value of the current node
        *compression_metadata = nodes[i].by_ascii;
        compression_metadata += sizeof(unsigned char);

        // Store the left child index or NULL if there is no left child
        if (nodes[i].left) {
            *compression_metadata = (unsigned char)((nodes[i].left - nodes) - start_index);
        }
        else {
            *compression_metadata = NULL;
        }
        compression_metadata += sizeof(unsigned char);

        // Store the right child index or NULL if there is no right child
        if (nodes[i].right) {
            *compression_metadata = (unsigned char)((nodes[i].right - nodes) - start_index);
        }
        else {
            *compression_metadata = NULL;
        }
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
    //failure here:
    huffman_free_tree(nodes , last_index);
}

//void huffman_decode(const unsigned char* data_to_decompress, unsigned char** decompressed_data, int* input_size)
//{
//}

/***************************************************************************
 *                            BUILD TREE FUNCTION
 * Name         : build_tree - build the Huffman tree
 * Parameters   : nodes - array of Huffman nodes combine the priority queue
 *                start_index - pointer to index of the first value (contain values) in the nodes array
 *                last_index - pointer to index of the last value (contain values) in the nodes array
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
        //    free(nodes+i);      // Free the parent node itself
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
	root = rescu_metadata(input_buffer, root);

//check
   // printf("root,%c", root->by_ascii);


    printf("%d input_size",*input_size);
	//*input_size -= metadata_size;

	unsigned char* input_pointer;
	unsigned char* output_pointer;

	input_pointer = input_buffer;
	output_pointer = output_buffer;

	int bits_index = 0;

	while (bits_index < ((*input_size)-2) * 8) {

		unsigned char decompressed_byte = find_ascii_in_tree(input_pointer, root, &bits_index);

		//write to output buffer the decomprresed byte
		*output_pointer = decompressed_byte;
		output_pointer++;
	}
    //last byte
    input_pointer++;
    int sum_bites_in_last_byte = *input_pointer-'0';
    input_pointer--;
    find_ascii_in_tree(input_pointer, root, &bits_index);
    free(root);//free the tree
}
	/***************************************************************************
 *                           RESCU_METADATA FUNCTION
 * Name         : rescu the metadata from the input buffer and return the metadata size;
 * Parameters   : input_buffer - pointer to the input data buffer
 *                root - pointer to array to keep the huffman codes 
 * Returned     : metadata size
 *
 ***************************************************************************/
Huffman_decode_node* rescu_metadata(unsigned char* input_buffer, Huffman_decode_node* root)
	{
        int decode_node_size = sizeof(Huffman_decode_node);
		unsigned int nodes_length = *input_buffer;
        printf("%c",*input_buffer );
        input_buffer=input_buffer+1;
        printf("%hi", *input_buffer);
        //move the pointer to the end of the input_file
		input_buffer += sizeof(int)+ (nodes_length* decode_node_size);
		root = (Huffman_decode_node*)malloc((nodes_length * decode_node_size));
        for (unsigned int i = nodes_length,j=0; i  > 0; i--,j+= decode_node_size) {
			memcpy(root+j, input_buffer- decode_node_size, decode_node_size);
			input_buffer -= decode_node_size;
            printf("root:%c", (root+j)->by_ascii);
            printf("left:%c", (root +j+1 )->left -'0');
            printf("right:%c\n",(root +j+2)->right -'0');
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
	unsigned char find_ascii_in_tree(unsigned char* input_pointer, Huffman_decode_node* root,int* bits_index) 
	{
		Huffman_decode_node* nodes;
		nodes = root;
	    unsigned char mask;
	    //move over the tree according to the received bits until finding leaves where the ascii code 
        while (nodes->left == NULL && nodes->right == NULL)
        {
		    //Moving the mask according to the index
		    mask = 0b10000000 >> (*bits_index % 8);

		    if ((*input_pointer & mask) == 0) {
			    nodes = &root[nodes->right-'0'];
		    }
		    else {
			    nodes = &root[nodes->left-'0'];
		    }
		    (*bits_index)++;
		    //advance the pointer of the bytes
		    if (*bits_index % 8 == 0) {
			    input_pointer++;
	 	    }
	    }
	return nodes->by_ascii;
}

    void find_ascii_last_byte(char last_byte, char sum_bites, Huffman_decode_node* root, int bites_index, unsigned char* output_pointer)
    {
        int sum_bites_in_last_byte = sum_bites - '0';
        Huffman_decode_node* nodes;
        nodes = root;
        unsigned char mask;
        for (int i = bites_index % 8; i < sum_bites_in_last_byte;) {
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

