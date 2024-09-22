
#include "huffman_code.h"

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
        nodes[ascii_index].code = NULL;
        nodes[ascii_index].frequency = 0;
    }
}

void count_frequencies(const U_08* data_to_compress_p, size_t input_size, Huffman_node_t* nodes)
{
    for (size_t index = 0; index < input_size; index++)
    {
        U_08 ascii_byte = data_to_compress_p[index];
        nodes[ascii_byte].frequency++;
    }
}

void store_metadata(Huffman_node_t* nodes, U_32 start_index, U_32 last_index, Huffman_metadata* metadata)
{
    metadata->tree_length = last_index - start_index + 1;

    for (U_32 i = start_index, md_node_index = 0; i <= last_index; i++, md_node_index++)
    {
        metadata->nodes[md_node_index].by_ascii = nodes[i].by_ascii;
        metadata->nodes[md_node_index].left = nodes[i].left ? (U_16)((nodes[i].left - nodes) - start_index) : INVALID_INDEX;
        metadata->nodes[md_node_index].right = nodes[i].right ? (U_16)((nodes[i].right - nodes) - start_index) : INVALID_INDEX;
    }
}

void encode_data(const U_08* data_to_compress_p, size_t input_size, Huffman_node_t* nodes, U_08* compressed_data_buffer_p, size_t* compressed_data_bit_index)
{
    for (size_t i = 0; i < input_size; i++)
    {
        U_08* huffman_code = nodes[data_to_compress_p[i]].code;
        U_32 huffman_code_length = nodes[data_to_compress_p[i]].code_length;

        for (U_32 bit_position = 0; bit_position < huffman_code_length; bit_position++)
        {
            // Calculate the byte index in the compressed data buffer
            size_t byte_index_in_compressed_buffer = *compressed_data_bit_index / BITS_IN_BYTE;

            // Initialize a new byte if starting from a new byte boundary
            if (!(*compressed_data_bit_index % BITS_IN_BYTE))
            {
                compressed_data_buffer_p[byte_index_in_compressed_buffer] = 0;
            }

            // Determine which bit of the current Huffman code we are processing
            U_32 byte_index_in_huffman_code = bit_position / BITS_IN_BYTE;
            U_32 bit_offset_in_huffman_byte = bit_position % BITS_IN_BYTE;

            if(huffman_code[byte_index_in_huffman_code] & (1 << (7 - bit_offset_in_huffman_byte)))
            {
                compressed_data_buffer_p[byte_index_in_compressed_buffer] |= (1 << (7 - (*compressed_data_bit_index % BITS_IN_BYTE)));
            }
            (*compressed_data_bit_index)++;
        }
    }
}

void finalize_compressed_data(U_08* compressed_data_buffer_p, size_t compressed_data_bit_index)
{
    U_32 bit_offset_in_byte = compressed_data_bit_index % BITS_IN_BYTE;

    // Handle remaining bits in the last byte
    if (bit_offset_in_byte != 0)
    {
        compressed_data_buffer_p[compressed_data_bit_index / BITS_IN_BYTE + 1] = bit_offset_in_byte;
    }
    else
    {
        compressed_data_buffer_p[compressed_data_bit_index / BITS_IN_BYTE] = 0;
    }
}

/***************************************************************************
 *                            HUFFMAN ENCODE FUNCTION
 * Name         : huffman_encode - encode data using Huffman coding
 * Parameters   : data_to_compress - pointer of the input buffer to compress
 *                output_buffer_p - pointer of the compressed data buffer
 *                input_size - size of the input buffer (data to compress)
 *                output_size - pointer to the final output buffer size
 * Returned     : none
 ***************************************************************************/
void huffman_encode(const U_08* data_to_compress_p, U_08* output_buffer_p, size_t input_size, size_t* output_size)
{
    assert(data_to_compress_p != NULL);
    assert(output_buffer_p != NULL);
    assert(input_size >= 0);

    *output_size = 0;
    if (input_size == 0)
    {
        return;
    }

    Huffman_node_t nodes[NODES_IN_TREE] = { 0 };
    U_32 start_index = 0 ,last_index = 0;
    U_32 metadata_size = 0;
    U_08* compressed_data;
    size_t compressed_data_bit_index = 0;

    initialize_nodes(nodes);
    count_frequencies(data_to_compress_p, input_size, nodes);
    qsort(nodes, ASCII_SIZE, sizeof(Huffman_node_t), huffman_frequency_compare);
   
    Huffman_node_t root = huffman_build_tree(nodes, &start_index, &last_index);
    huffman_generate_codes(&root);

    Huffman_metadata* compression_metadata = (Huffman_metadata*)output_buffer_p;
    store_metadata(nodes, start_index, last_index, compression_metadata);

    qsort(nodes, ASCII_SIZE, sizeof(Huffman_node_t), huffman_ascii_compare);

    metadata_size = sizeof(compression_metadata->tree_length) + sizeof(Huffman_decode_node) * compression_metadata->tree_length;
    compressed_data = output_buffer_p + metadata_size;
    encode_data(data_to_compress_p, input_size, nodes, compressed_data, &compressed_data_bit_index);
    finalize_compressed_data(compressed_data, compressed_data_bit_index);

    size_t compressed_data_size = BITS_TO_BYTES(compressed_data_bit_index) + 1;// +1 for the num of the remaining bits - byte
    *output_size = compressed_data_size + metadata_size;

    // Log compression statistics
    double compression_ratio = (double)*output_size / input_size;
    double metadata_ratio = (double)metadata_size / *output_size;
    printf("Input size from LZ77: %zu bytes\n", input_size);
    printf("Compressed size: %zu bytes\n", *output_size);
    printf("Metadata size: %zu bytes\n", metadata_size);
    printf("Huffman compression ratio: %.2f%%\n", compression_ratio * 100);
    printf("Metadata ratio: %.2f%%\n", metadata_ratio * 100);

    if (metadata_ratio > 0.05) {
        printf("WARNING: Metadata size is more than 5%% of the compressed data.\n");
    }
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
Huffman_node_t huffman_build_tree(Huffman_node_t* nodes, U_32* start_index, U_32* last_index)
{
    Huffman_node_t* min_leaf = nodes;
    Huffman_node_t* min_parent = nodes + ASCII_SIZE;
    U_32 current_parent_index = ASCII_SIZE;

    while (min_leaf->frequency == 0)
    {
        min_leaf++;
        (*start_index)++;
    }

    S_32 flag = 1;
    while (min_parent - nodes < NODES_IN_TREE && flag)
    {
        Huffman_node_t* min_node1 = extract_min(&min_leaf, &min_parent, &nodes[ASCII_SIZE]);

        Huffman_node_t* min_node2 = extract_min(&min_leaf, &min_parent, &nodes[ASCII_SIZE]);

        if (!min_node2->frequency || min_node2->frequency<0)
        {
            flag = 0;
        }
        else
        {
            Huffman_node_t parent;
            parent.left = min_node1;
            parent.right = min_node2;
            parent.frequency = min_node1->frequency + min_node2->frequency;
            parent.code = NULL;
       //     printf("Pushing parent node with frequency: %d at index: %d\n", parent.frequency, current_parent_index);
            priority_queue_push(nodes, &current_parent_index, parent);
       //     printf("After push, nodes[%d].frequency = %d\n", current_parent_index - 1, nodes[current_parent_index - 1].frequency);

        }
    }
    *last_index = current_parent_index - 1;
    return nodes[*last_index];
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
    assert(current_length < MAX_CODE_LENGTH);

    if (!node->left && !node->right) // Leaf node
    {
        // Calculate the number of bytes needed to store the bits
        U_32 num_bytes = BITS_TO_BYTES(current_length);
        node->code = malloc(num_bytes);
        if (node->code == NULL)
        {
            perror("memory allocation failed in Huffman code - generate codes function");
            exit(1);
        }

        // Initialize code memory
        memset(node->code, 0, num_bytes);

        // Store the bits of current_code into node->code
        for (U_32 bit_position = 0; bit_position < current_length; bit_position++)
        {
            U_32 bit_offset_in_code = current_length - bit_position - 1;
            U_32 byte_index_in_code = bit_position / BITS_IN_BYTE;
            U_32 bit_index_in_byte = bit_position % BITS_IN_BYTE;

            if (current_code & (1 << (bit_offset_in_code)))
            {
                //the sequnce of the relevant bits will start from left
                node->code[byte_index_in_code] |= (1 << (7 - bit_index_in_byte));
            }
        }

        node->code_length = current_length;
        return;
    }
    else 
    {
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

}

void huffman_free_tree(Huffman_node_t* nodes, U_32 last_index)
{
    for (U_32 i = 0; i < ASCII_SIZE; i++)
    {
        if (nodes[i].code)
        {
            free(nodes[i].code);
            nodes[i].code = NULL;
        }
    }
}

/***************************************************************************
 *                           HUFFMAN_DECODE FUNCTION
 * Name         : decode the data with huffman decompression algorithm
 * Parameters   : input_buffer_p - pointer to the input data and meta data buffer
 *                input_size - size of the input data and meta data.
 *                output_buffer_p - pointer to the output data buffer to fill it
 * Returned     : none
 *
 ***************************************************************************/
void huffman_decode(const U_08* input_buffer_p, const size_t* input_size, U_08* output_buffer_p, size_t* output_size)
{
    *output_size = 0;
    Huffman_decode_node* nodes = NULL;
    U_32 tree_length;

    nodes = get_tree(input_buffer_p, &tree_length);
    U_32 meta_data_size = (sizeof(U_16) + (tree_length * sizeof(Huffman_decode_node)));


    input_buffer_p += meta_data_size;// take head the input_pointer 
    size_t data_length = *input_size - meta_data_size;

    const U_08* input_pointer = input_buffer_p;
    U_08* output_pointer = output_buffer_p;

    size_t bit_index = 0;
    U_32 last_bytes = 2;
    U_32 data_bits_length_without_lasts_bytes = (data_length - last_bytes) * BITS_IN_BYTE;

    while (bit_index < data_bits_length_without_lasts_bytes)
    {
        assert(bit_index < data_bits_length_without_lasts_bytes);

        U_08 decompressed_byte = find_ascii_in_tree(&input_pointer, nodes, &bit_index, tree_length);

        //printf("decompressed_byte:%c ", decompressed_byte);
        //write to output buffer the decomprresed byte
        *output_pointer = decompressed_byte;
        output_pointer++;
        (*output_size)++;
    }

    find_ascii_last_byte(input_pointer, nodes, bit_index, output_pointer, tree_length, output_size);
}

/***************************************************************************
 *                           get_tree FUNCTION
 * Name         : rescu the metadata from the input buffer and return the metadata size;
 * Parameters   : input_buffer_p - pointer to the input data buffer
 *                root - pointer to array to keep the huffman codes
 * Returned     : metadata size
 *
 ***************************************************************************/
Huffman_decode_node* get_tree(const U_08* input_buffer_p, U_32* tree_length)
{
    Huffman_metadata* asMetadData_p = (Huffman_metadata*)input_buffer_p;
    *tree_length = asMetadData_p->tree_length;

    return (asMetadData_p->nodes);
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
U_08 find_ascii_in_tree(const U_08** input_pointer, Huffman_decode_node* nodes, U_32* bit_index, U_32 tree_length)
{
    Huffman_decode_node* current_node;
    current_node = nodes + (tree_length - 1);
    U_08 mask;
    U_08 index_bit_in_byte;
    //move over the tree according to the received bits until finding leaves where the ascii code 
    while (current_node && current_node->left != 512 && current_node->right != 512)
    {
        index_bit_in_byte = (*bit_index) % BITS_IN_BYTE;
        //Moving the mask according to the index
        mask = 0x80 >> index_bit_in_byte;

        if (((**input_pointer) & mask) == 0)
        {
            current_node = &nodes[current_node->left];
        }
        else
        {
            current_node = &nodes[current_node->right];
        }
        (*bit_index)++;
        //advance the pointer of the bytes
        if (((*bit_index) % BITS_IN_BYTE) == 0)
        {
            (*input_pointer)++;
        }
    }

    return current_node->by_ascii;
}

void find_ascii_last_byte(const U_08* input_pointer, Huffman_decode_node* nodes, size_t bit_index, U_08* output_pointer, U_32 tree_length, size_t* output_size)
{
    U_08 last_byte = *input_pointer;
    input_pointer++;
    U_08 sum_bits_in_last_byte = (U_08)*input_pointer;
    Huffman_decode_node* current_node;
    current_node = nodes + (tree_length - 1);
    U_08 mask;
    U_08 index_bit_in_byte;

    assert(current_node->left <= 512);
    assert(current_node->right <= 512);

    sum_bits_in_last_byte = sum_bits_in_last_byte == 0 ? 8 : sum_bits_in_last_byte;
    index_bit_in_byte = (bit_index) % BITS_IN_BYTE;

    for (U_32 i = index_bit_in_byte; i < sum_bits_in_last_byte;)
    {
        //move over the tree according to the received bits until finding leaves where the ascii code 
        while (current_node->left != 512 && current_node->right != 512)
        {
            index_bit_in_byte = i;
            //Moving the mask according to the index
            mask = 0x80 >> index_bit_in_byte;

            if (((last_byte)&mask) == 0)
            {
                current_node = &nodes[current_node->left];
            }
            else
            {
                current_node = &nodes[current_node->right];
            }
            i++;

        }
      //  printf("by_ascii:%c ", current_node->by_ascii);

        *(output_pointer) = current_node->by_ascii;
        output_pointer++;
        (*output_size)++;
        current_node = nodes + (tree_length - 1);
    }
}



