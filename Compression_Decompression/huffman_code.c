
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

void count_frequencies(const U_08* data_to_compress, U_32 input_size, Huffman_node_t* nodes)
{
    for (U_32 index = 0; index < input_size; index++)
    {
        U_08 ascii_byte = data_to_compress[index];
        nodes[ascii_byte].frequency++;
    }
}

//void store_metadata(Huffman_node_t* nodes, U_32 start_index, U_32 last_index, Huffman_metadata* metadata)
//{
//    /*U_08* metadata_2;
//    for (U_32 i = start_index, j = 0; i <= last_index; i++, j++)
//    {
//        *(metadata_2 + j) = (U_08)(nodes[i].by_ascii);
//        j++;
//        *(metadata_2 + j) = nodes[i].left ? (U_16)((nodes[i].left - nodes) - start_index) : INVALID_INDEX;
//        j += 2;
//        *(metadata_2 + j) = nodes[i].right ? (U_16)((nodes[i].right - nodes) - start_index) : INVALID_INDEX;
//    }*/
//
//    for (U_32 i = start_index, md_node_index = 0; i <= last_index; i++, md_node_index++)
//    {
//        metadata->nodes[md_node_index].by_ascii = nodes[i].by_ascii;
//        metadata->nodes[md_node_index].left = nodes[i].left ? (U_16)((nodes[i].left - nodes) - start_index) : INVALID_INDEX;
//        metadata->nodes[md_node_index].right = nodes[i].right ? (U_16)((nodes[i].right - nodes) - start_index) : INVALID_INDEX;
//    }
//}

// Modified store_metadata function
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


void encode_data(const U_08* data_to_compress, U_32 input_size, Huffman_node_t* nodes, U_08* compressed_data, long* compressed_data_bit_index)
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

void finalize_compressed_data(U_08* compressed_data, long compressed_data_bit_index)
{
    compressed_data[compressed_data_bit_index / 8 + 1] = 0;
    // Handle remaining bits in the last byte
    if (compressed_data_bit_index % 8 != 0)
    {
        compressed_data[compressed_data_bit_index / 8 + 1] = (compressed_data_bit_index % 8);
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
void huffman_encode(const U_08* data_to_compress, U_08* output_buffer_p, U_32 input_size, U_32* output_size)
{
    assert(data_to_compress != NULL);
    assert(output_buffer_p != NULL);
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

    //// Calculate the total size needed for metadata and nodes
    //size_t metadata_size = sizeof(Huffman_metadata);
    //size_t nodes_size = sizeof(Huffman_decode_node) * (last_index - start_index + 1);
    //size_t total_size = metadata_size + nodes_size;

    //// Ensure the buffer is large enough
    //assert(total_size <= output_size);//suppose to be *output_size

    //// Point to the start of the buffer for the metadata
    //Huffman_metadata* compression_metadata = (Huffman_metadata*)output_buffer_p;

    //// Point the nodes array to the location right after the metadata in the buffer
    ////compression_metadata->nodes = (Huffman_decode_node*)(output_buffer_p + metadata_size);

    //// Set the tree_length
    //compression_metadata->tree_length = last_index - start_index + 1;

    //store_metadata(nodes, start_index, last_index, compression_metadata);
    //memcpy((output_buffer_p + metadata_size), compression_metadata->nodes, compression_metadata->tree_length * sizeof(Huffman_decode_node));
    //qsort(nodes, ASCII_SIZE, sizeof(Huffman_node_t), huffman_ascii_compare);

    //// calculate the start of compressed data
    //U_08* compressed_data = (U_08*)(compression_metadata->nodes + compression_metadata->tree_length);

    Huffman_metadata* compression_metadata = (Huffman_metadata*)output_buffer_p;
    store_metadata(nodes, start_index, last_index, compression_metadata);

    qsort(nodes, ASCII_SIZE, sizeof(Huffman_node_t), huffman_ascii_compare);

    U_32 metadata_size = sizeof(compression_metadata->tree_length) + sizeof(Huffman_decode_node) * compression_metadata->tree_length;
    //assert(metadata_size <= *output_size);
    // Calculate the start of compressed data
    U_08* compressed_data = output_buffer_p + metadata_size;
    long compressed_data_bit_index = 0;

    encode_data(data_to_compress, input_size, nodes, compressed_data, &compressed_data_bit_index);
    finalize_compressed_data(compressed_data, compressed_data_bit_index);

    *output_size = (compressed_data_bit_index + 7) / 8 + 1 + metadata_size;  // +1 for the remaining bits byte
    //TODO: check the failure here:
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

        if (!min_node2->frequency)
        {
            flag = 0;
        }
        else
        {
            //Huffman_node_t* parent = malloc(sizeof(Huffman_node_t));
            Huffman_node_t parent;
            /*if (parent == NULL)
            {
                perror("Memory allocation failed in compress_data");
            }*/
            parent.left = min_node1;
            parent.right = min_node2;
            parent.frequency = min_node1->frequency + min_node2->frequency;
            parent.code = NULL;

           // printf("Pushing parent node with frequency: %d at index: %d\n", parent.frequency, current_parent_index);
            priority_queue_push(nodes, &current_parent_index, parent);
            //printf("After push, nodes[%d].frequency = %d\n", current_parent_index - 1, nodes[current_parent_index - 1].frequency);
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
        if (node->code == NULL)
        {
            perror("memory allocation failed in Huffman code - generate codes function");
            exit(1);
        }

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
    else {
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
void huffman_decode(const U_08* input_buffer_p, const U_32* input_size, U_08* output_buffer_p, U_32* output_size)
{
    //input_size ����� �� �� ����� �� ����� ������ ���� ���
    *output_size = 0;
    Huffman_decode_node* root = NULL;

    U_32 tree_length;

    //TODO : root returned as null
    root = rescue_metadata(input_buffer_p, &tree_length);

    input_buffer_p += (sizeof(U_16) + (tree_length * sizeof(Huffman_decode_node)));// take head the input_pointer on the struct size 
    //input_buffer_p += sizeof(Huffman_metadata) + tree_length * sizeof(Huffman_decode_node);

    const U_08* input_pointer = input_buffer_p;
    U_08* output_pointer = output_buffer_p;

    U_32 bit_index = 0;

    U_32 data_length = ((*input_size) - (sizeof(U_16) + (tree_length * sizeof(Huffman_decode_node))));
    
 /*   printf("Huffman Decode function without the root: ");
    for (U_32 i = 0; i < data_length; i++) {
        printf("%02x ", input_buffer_p[i]);
    }
    printf("\n");*/
  /*  printf("find_ascii_last_byte: ");
    for (int i =( data_length-2); i <data_length; i++) {
        printf("%c ", input_pointer[i]);
    }*/
    while (bit_index < ((data_length) - 2) * NUM_BITS)
    {
        //assert((bits_index < (data_length)-2) * NUM_BITS);
        U_08 decompressed_byte = find_ascii_in_tree(&input_pointer, root, &bit_index, tree_length);

        printf("decompressed_byte:%c ", decompressed_byte);
        //write to output buffer the decomprresed byte
        *output_pointer = decompressed_byte;
        output_pointer++;
        (*output_size)++;
    }
   /* printf("find_ascii_last_byte: ");
    for (int i = 0; i < 2; i++) {
        printf("%c ", input_pointer[i]);
    }*/
    find_ascii_last_byte(input_pointer, root, bit_index, output_pointer, tree_length, output_size);
}

/***************************************************************************
 *                           RESCUE_METADATA FUNCTION
 * Name         : rescu the metadata from the input buffer and return the metadata size;
 * Parameters   : input_buffer_p - pointer to the input data buffer
 *                root - pointer to array to keep the huffman codes
 * Returned     : metadata size
 *
 ***************************************************************************/
Huffman_decode_node* rescue_metadata(const U_08* input_buffer_p, U_32* tree_length)
{
    //printf("Rescue metadata function: ");
    Huffman_metadata* asMetadData_p = (Huffman_metadata*) input_buffer_p;

    //Huffman_metadata localHuffman_metadata;
    *tree_length = asMetadData_p->tree_length;
    //memcpy(&localHuffman_metadata, input_buffer_p, (sizeof(U_16) + (sizeof(Huffman_decode_node) * (*tree_length))));
 /*   for (int i = 0; i < huffman_metadata.tree_length; i++)
    {
        printf("root:%c, left:%hu, right:%hu\n", huffman_metadata.nodes[i].by_ascii, huffman_metadata.nodes[i].left, huffman_metadata.nodes[i].right);
    }*/
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
U_08 find_ascii_in_tree(const U_08** input_pointer, Huffman_decode_node* root, U_32* bits_index, U_32 tree_length)
{
    Huffman_decode_node* nodes;
    nodes = root + (tree_length - 1);
    U_08 mask;
    //move over the tree according to the received bits until finding leaves where the ascii code 
    while (nodes && nodes->left != 512 && nodes->right != 512)
    {
        //Moving the mask according to the index
        mask = 0b10000000 >> (*bits_index % NUM_BITS);

        if (((**input_pointer) & mask) == 0)
        {
            nodes = &root[nodes->left];
        }
        else
        {
            nodes = &root[nodes->right];
        }
        (*bits_index)++;
        //advance the pointer of the bytes
        if (((*bits_index) % NUM_BITS) == 0)
        {
            (*input_pointer)++;
        }
    }

    return nodes->by_ascii;
}

void find_ascii_last_byte(const U_08* input_pointer, Huffman_decode_node* root, U_32 bites_index, U_08* output_pointer, U_32 tree_length, U_32* output_size)
{

 /*   printf("find_ascii_last_byte: ");
    for (int i = 0; i < 2; i++) {
        printf("%c ", input_pointer[i]);
    }*/
    printf("\n");
    U_08 last_byte = *input_pointer;
    input_pointer++;
    U_08 sum_bits_in_last_byte = (U_08)*input_pointer;
    Huffman_decode_node* nodes;
    nodes = root + (tree_length - 1);
    U_08 mask;

    assert(nodes->left <= 512);
    assert(nodes->right <= 512);

    sum_bits_in_last_byte = sum_bits_in_last_byte == 0 ? 8 : sum_bits_in_last_byte;

    for (U_32 i = (bites_index % NUM_BITS); i < sum_bits_in_last_byte;)
    {
        //move over the tree according to the received bits until finding leaves where the ascii code 
        while (nodes->left != 512 && nodes->right != 512)
        {
            //Moving the mask according to the index
            mask = 0b10000000 >> (i % NUM_BITS);


            if (((last_byte)&mask) == 0)
            {
                nodes = &root[nodes->left];
            }
            else
            {
                nodes = &root[nodes->right];
            }
            i++;

        }
    //    printf("byascii:%c ", nodes->by_ascii);
        *(output_pointer) = nodes->by_ascii;
        output_pointer++;
        (*output_size)++;
        nodes = root + (tree_length - 1);
    }
}


