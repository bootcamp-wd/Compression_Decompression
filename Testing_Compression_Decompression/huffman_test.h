#ifndef HUFFMAN_TEST_H
#define HUFFMAN_TEST_H

#include "test_framework.h"
#include "huffman_code.h"
#include <string.h>

#define ASCII_SIZE 256

void test_huffman_frequency_compare();
void test_initialize_nodes();
void  test_count_frequencies();
void test_huffman_encode();
void test_get_tree();
void test_find_ascii_in_tree();
void test_find_ascii_last_byte();
//encod-decode
void test_exactly_bits_in_bytes();
void test_not_exactly_bits_in_bytes();
void test_huffman_long_input();


#endif // !HUFFMAN_TEST_H
