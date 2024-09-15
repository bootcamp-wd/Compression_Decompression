#include <stdio.h>
#include <time.h>
#include "test_framework.h"
#include "lz77_test.h"

int main()
{
    srand(time(0));

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


   run_all_tests();

    return 0;


}

