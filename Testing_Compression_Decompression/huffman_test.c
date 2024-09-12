#include <string.h>
#include "test_framework.h"
#include "huffman_code.h"
#include "huffman_test.h"

void test_huffman_frequency_compare(void)
{
    Huffman_node_t node1 = { .frequency = 5, .by_ascii = 'A' };
    Huffman_node_t node2 = { .frequency = 10, .by_ascii = 'B' };
    Huffman_node_t node3 = { .frequency = 5, .by_ascii = 'C' };
    ASSERT(huffman_frequency_compare(&node1, &node2) < 0, "node1 should be less than node2");
    ASSERT(huffman_frequency_compare(&node2, &node1) > 0, "node2 should be greater than node1");
    ASSERT(huffman_frequency_compare(&node1, &node3) < 0, "node1 should be less than node3 (same frequency, lower ASCII)");
}

void test_initialize_nodes(void) 
{
    Huffman_node_t nodes[ASCII_SIZE];
    initialize_nodes(nodes);
    for (U_32 i = 0; i < ASCII_SIZE; i++) {
        ASSERT_EQUAL(i, nodes[i].by_ascii, "Node ASCII value should match index");
        ASSERT_EQUAL(0, nodes[i].frequency, "Node frequency should be initialized to 0");
    }
}

void test_count_frequencies(void) 
{
    Huffman_node_t nodes[ASCII_SIZE] = { 0 };
    initialize_nodes(nodes);
    const U_08 data[] = "AABBBCCCC";
    U_32 input_size = strlen((char*)data);
    count_frequencies(data, input_size, nodes);
    ASSERT_EQUAL(2, nodes['A'].frequency, "Frequency of 'A' should be 2");
    ASSERT_EQUAL(3, nodes['B'].frequency, "Frequency of 'B' should be 3");
    ASSERT_EQUAL(4, nodes['C'].frequency, "Frequency of 'C' should be 4");
}

void test_huffman_encode(void) 
{
    const U_08 input[] = "AABBBCCCC";
    U_32 input_size = strlen((char*)input);
    U_08 output_buffer[1024];
    U_32 output_size;
    huffman_encode(input, output_buffer, input_size, &output_size);
    ASSERT(output_size > 0, "Output size should be greater than 0");
    ASSERT(output_size < input_size, "Compressed size should be less than input size");
}








