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
void test_rescue_metadata();
void test_find_ascii_in_tree();
void test_find_ascii_last_byte();

//all
//ELOOLE exactly bites in bytes

#endif // !HUFFMAN_TEST_H
