#include "huffman_test.h"

//encode
void test_huffman_frequency_compare(void) {
    Huffman_node_t node1 = { .frequency = 5, .by_ascii = 'A' };
    Huffman_node_t node2 = { .frequency = 10, .by_ascii = 'B' };
    Huffman_node_t node3 = { .frequency = 5, .by_ascii = 'C' };
    ASSERT(huffman_frequency_compare(&node1, &node2) < 0, "node1 should be less than node2");
    ASSERT(huffman_frequency_compare(&node2, &node1) > 0, "node2 should be greater than node1");
    ASSERT(huffman_frequency_compare(&node1, &node3) < 0, "node1 should be less than node3 (same frequency, lower ASCII)");
}
void test_initialize_nodes(void) {
    Huffman_node_t nodes[ASCII_SIZE];
    initialize_nodes(nodes);
    for (U_32 i = 0; i < ASCII_SIZE; i++) {
        ASSERT_EQUAL(i, nodes[i].by_ascii, "Node ASCII value should match index");
        ASSERT_EQUAL(0, nodes[i].frequency, "Node frequency should be initialized to 0");
    }
}
void test_count_frequencies(void) {
    Huffman_node_t nodes[ASCII_SIZE] = { 0 };
    initialize_nodes(nodes);
    const U_08 data[] = "AABBBCCCC";
    U_32 input_size = strlen((char*)data);
    count_frequencies(data, input_size, nodes);
    ASSERT_EQUAL(2, nodes['A'].frequency, "Frequency of 'A' should be 2");
    ASSERT_EQUAL(3, nodes['B'].frequency, "Frequency of 'B' should be 3");
    ASSERT_EQUAL(4, nodes['C'].frequency, "Frequency of 'C' should be 4");
}
void test_huffman_encode(void) {
    const U_08 input[] = "AABBBCCCC";
    U_32 input_size = strlen((char*)input);
    U_08 output_buffer[1024];
    U_32 output_size;
    huffman_encode(input, output_buffer, input_size, &output_size);
    ASSERT(output_size > 0, "Output size should be greater than 0");
    ASSERT(output_size < input_size, "Compressed size should be less than input size");
}
//decode
void test_rescue_metadata()
{
    U_08 output_buffer[100];
    U_32 output_size;
    U_32 tree_length;
    Huffman_decode_node* root,*tmp_root,*nodes;

    huffman_encode("hello", output_buffer, 5, &output_size);

    root = rescue_metadata(output_buffer, &tree_length);
    nodes = root;
    tmp_root= root + (tree_length - 1);//move the pointer to the root of the tree
    root = tmp_root;
   
    //check size
    ASSERT_EQUAL(tree_length, 7, "The tree length wrong");

    //check tree
    ASSERT_EQUAL(root->left,4, "The tree is not accurate");
    ASSERT_EQUAL(root->right, 5, "The tree is not accurate");

    root = &(nodes[root->left]);
    ASSERT_EQUAL(root->left, 0, "The tree is not accurate");
    ASSERT_EQUAL(root->right, 1, "The tree is not accurate");

    root = &(nodes[root->left]);
    ASSERT_EQUAL(root->left, 512, "The tree is not accurate");
    ASSERT_EQUAL(root->right,512, "The tree is not accurate");
    ASSERT_EQUAL(root->by_ascii, 'e', "The tree is not accurate");
 
    root = tmp_root;

    root = &(nodes[root->left]);
    root = &(nodes[root->right]);
    ASSERT_EQUAL(root->left, 512, "The tree is not accurate");
    ASSERT_EQUAL(root->right, 512, "The tree is not accurate");
    ASSERT_EQUAL(root->by_ascii, 'h', "The tree is not accurate");

    root = tmp_root;

    root = &(nodes[root->right]);
    ASSERT_EQUAL(root->left, 2, "The tree is not accurate");
    ASSERT_EQUAL(root->right, 3, "The tree is not accurate");

    root = &(nodes[root->right]);
    ASSERT_EQUAL(root->left, 512, "The tree is not accurate");
    ASSERT_EQUAL(root->right, 512, "The tree is not accurate");
    ASSERT_EQUAL(root->by_ascii, 'l', "The tree is not accurate");

    root = tmp_root;

    root = &(nodes[root->right]);
    root = &(nodes[root->left]);
    ASSERT_EQUAL(root->left, 512, "The tree is not accurate");
    ASSERT_EQUAL(root->right, 512, "The tree is not accurate");
    ASSERT_EQUAL(root->by_ascii, 'o', "The tree is not accurate");
}

void test_find_ascii_in_tree()
{
    U_08* output_buffer = (U_08*)malloc(100);
    U_32 output_size;
    U_32 tree_length;
    Huffman_metadata* metadata;
    Huffman_decode_node* root;
    U_08 ascii;
    U_32 num_bits=0;
    huffman_encode("green", output_buffer, 5, output_buffer);
    root = rescue_metadata(output_buffer, &tree_length);
    output_buffer += (sizeof(metadata->tree_length) + sizeof(Huffman_decode_node) * tree_length);
    ascii= find_ascii_in_tree(output_buffer,root,&num_bits,tree_length);
    ASSERT_EQUAL(ascii, 'g', "The found ascii is wrong");
    ASSERT_EQUAL(num_bits, 2, "The num of bits is wrong");
    free(output_buffer);
}
