#include <stdio.h>
#include <time.h>
#include "test_framework.h"
#include "lz77_test.h"
#include "file_test.h"
#include "end_to_end_test.h"
#include "algorithms_test.h"
#include "huffman_test.h"

int main()
{
    srand(time(0));
    //lz77
    add_test("LZ77 TEST REGULAR SIZE", test_lz77_regular_size);
    add_test("LZ77 TEST IDENTICAL CHARACTERS", test_lz77_identical_characters);
    add_test("LZ77 TEST EMPTY", test_lz77_empty);
    add_test("LZ77 TEST MANY REPEAT", test_lz77_many_repeat);
    add_test("TEST LZ77 SEARCH DICTIONARY EXIST IN MIDDLE", test_lz77_search_dictionary_exist_in_middle);
    add_test("TEST LZ77 SEARCH DICTIONARY DONT EXIST", test_lz77_search_dictionary_dont_exist);
    add_test("TEST LZ77 SEARCH DICTIONARY IN BEGIN", test_lz77_search_dictionary_in_begin);
    add_test("TEST LZ77 SEARCH DICTIONARY IN END", test_lz77_search_dictionary_in_end);
    add_test("TEST LZ77 SIZE WINDOW REGULAR", test_lz77_size_window_regular);
    add_test("TEST LZ77 SIZE WINDOW NOT IN RANGE", test_lz77_size_window_not_in_range);
    add_test("TEST LZ77 ENCODE NO REAPETS", test_lz77_encode_no_reapets);
    add_test("TEST LZ77 ENCODE MANY REPEATS", test_lz77_encode_many_reapets);
    add_test("TEST LZ77 ENCODE SAME CHARACTERS", test_lz77_encode_same_characters);
    add_test("TEST LZ77 DECODE NO REAPETS", test_lz77_decode_no_reapets);
    add_test("TEST LZ77 DECODE MANY REAPETS", test_lz77_decode_many_reapets);
    add_test("TEST LZ77 DECODE SAME CHARACTERS", test_lz77_decode_same_characters);
    //file
    add_test("TEST READ & WRITE TO FILE", files_test);
    //huffman
    add_test("Huffman Frequency Compare", test_huffman_frequency_compare);
    add_test("Initialize Nodes", test_initialize_nodes);
    add_test("Count Frequencies", test_count_frequencies);
    add_test("Huffman Encode", test_huffman_encode);
    add_test("Rescue metadata", test_rescue_metadata);
    add_test("Found the ascii char in tree", test_find_ascii_in_tree);
    add_test("Found the last ascii in by(te", test_find_ascii_last_byte);
    add_test("exactly num bits in bytes", test_exactly_bits_in_bytes);
    add_test("not exactly num bits in bytes", test_not_exactly_bits_in_bytes);
    add_test("test_huffman_exactly_bits_in_bytes_long_input", test_huffman_exactly_bits_in_bytes_long_input);

   add_test("test_algorithms_lz77_and_huffman", test_algorithms_lz77_and_huffman);
    //end to end
    //size
    add_test("TEST REGULAR SIZE FILE", regular_size_file_test);
    add_test("TEST SHORT FILE", short_file_test);
    add_test("TEST LONG FILE", long_file_test);
    add_test("TEST EMPTY FILE", empty_file_test);
    add_test("TEST SINGLE CHARACTER", single_character);
    //suffix
    add_test("TEST TEXT FILE", Text_file_test);
    add_test("TEST IMAGE FILE", Image_file_test);
    add_test("TEST AODIO FILE", Audio_file_test);
    add_test("TEST PDF FILE", PDF_file_test);
    add_test("TEST UNKNOWN FILE", Unknown_file_test);
    //more
    add_test("TEST RANDOM FILE", random_file_test);
    add_test("TEST ALL ASCII CHARACTERS", all_ascii_file_test);
    add_test("TEST REAPTING SINGLE CHARACTER", repeating_single_character_file_test);
    //error handling
    add_test("TEST NOT EXIST FILE", not_exist_file_test);
    add_test("TEST IN CORRECT PATH", Incorrect_path);

   run_all_tests();

    return 0;
}

