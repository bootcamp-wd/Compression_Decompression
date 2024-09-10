#include "lz77_test.h"

void test_lz77_treatment(const U_08* test_name, U_08* input_buffer, U_32 input_size, U_32 compress_level)
{
    U_08* compressed_buffer = (U_08*)malloc(input_size * get_size_of_encoded_sequence_struct());
    U_08* decompressed_buffer = (U_08*)malloc(input_size);
    U_32 compressed_size = 0;

    lz77_encode(input_buffer, input_size, compressed_buffer, &compressed_size, compress_level);
    lz77_decode(compressed_buffer, &compressed_size, decompressed_buffer);

    U_08 flag = 1;

    for (U_32 i = 0; i < input_size; i++)
    {
        if (input_buffer[i] != decompressed_buffer[i])
            flag = 0;
    }
    ASSERT_EQUAL(flag, 1, "Decompressed data does not match original");

    free(compressed_buffer);
    free(decompressed_buffer);
}

void test_lz77_search_in_dictionary_treatment(const U_08* dictionary, const U_08* buffer, const U_08* buffer_expected)
{
    U_08* dict_start = dictionary;
    U_08* dict_end = dictionary + strlen(dictionary) - 1;
    U_08* buffer_start = buffer;
    U_08* buffer_end = buffer + strlen(buffer) - 1;

    U_08* result = search_in_dictionary(dict_start, dict_end, buffer_start, buffer_end);

    U_08 flag = 1;
    U_32 length_loop = strlen(buffer);
    if (!result)
    {
        length_loop = 0;
    }
    for (U_32 i = 0; i < length_loop; i++)
    {
        if (buffer_expected[i] != result[i])
            flag = 0;
    }
    ASSERT_EQUAL(flag, 1, "Search in dictionary doesn't return an expected result");
}

void test_lz77_size_of_window_treatment(const U_32 level, const U_32 dictionary_expected, const U_32 buffer_search_expected)
{
    U_32 dictionary_size, buffer_search_size;
    size_of_window_according_level(level, &dictionary_size, &buffer_search_size);
    ASSERT(dictionary_expected == dictionary_size && buffer_search_expected == buffer_search_size, "SIZE OF WINDOW WORKS WRONG");
}

void generate_random_input(U_08* buffer, U_32 size)
{
    for (U_32 i = 0; i < size; i++)
    {
        buffer[i] = rand() % 256;
    }
    buffer[size - 1] = '\0'; // Null-terminate the string
}

void test_lz77_regular_size(void)
{
    U_32 input_size = rand() % 10000;
    U_08* input_buffer = (U_08*)malloc(input_size * sizeof(U_08));
    generate_random_input(input_buffer, input_size);
    U_32 compress_level = rand() % 7;

    test_lz77_treatment("LZ77 Test Regular Size", input_buffer, input_size, compress_level);
}

void test_lz77_identical_characters(void)
{
    U_08 char_to_input = (rand() % (126 - 32 + 1)) + 32;
    U_32 input_size = rand() % 10000;
    U_08* input_buffer = (U_08*)malloc(input_size * sizeof(U_08));
    for (U_32 i = 0; i < input_size; i++)
    {
        input_buffer[i] = char_to_input;
    }
    U_32 compress_level = rand() % 7;

    test_lz77_treatment("LZ77 Test Identical Characters", input_buffer, input_size, compress_level);
}

void test_lz77_empty(void)
{
    U_08 input_buffer[] = "";
    U_32 input_size = strlen((char*)input_buffer);
    U_32 compress_level = rand() % 7;

    test_lz77_treatment("LZ77 Test Empty", input_buffer, input_size, compress_level);
}

void test_lz77_many_repeat(void)
{
    U_08 input_buffer[] = "aaaaaaaaaaaaaaaabbbbbbbbbbbbbbbbbbbbbbbbbbbaaaaabbbbbbbbcccccccccccccccdddddddddddddddddddddaaaaaaaabbbbcccccccccbbbbddd";
    U_32 input_size = strlen((char*)input_buffer);
    U_32 compress_level = rand() % 7;

    test_lz77_treatment("LZ77 Test Many Repeat", input_buffer, input_size, compress_level);
}

void test_lz77_search_dictionary_exist_in_middle(void)
{
    U_08* dictionary = "abcdefghijklmnop";
    U_08* buffer = "hijklm";
    U_08* expected = "hijklm";

    test_lz77_search_in_dictionary_treatment(dictionary, buffer, expected);
}

void test_lz77_search_dictionary_dont_exist(void)
{
    U_08* dictionary = "abcdefghijklmnop";
    U_08* buffer = "abd";
    U_08* expected = NULL;

    test_lz77_search_in_dictionary_treatment(dictionary, buffer, expected);
}

void test_lz77_search_dictionary_in_begin(void)
{
    U_08* dictionary = "abcdefghijklmnop";
    U_08* buffer = "abcd";
    U_08* expected = "abcd";

    test_lz77_search_in_dictionary_treatment(dictionary, buffer, expected);
}

void test_lz77_search_dictionary_in_end(void)
{
    U_08* dictionary = "abcdefghijklmnop";
    U_08* buffer = "mnop";
    U_08* expected = "mnop";

    test_lz77_search_in_dictionary_treatment(dictionary, buffer, expected);
}

void test_lz77_size_window_regular(void)
{
    U_32 dictionary_size_exp, buffer_search_size_exp, level;

    level = 5;
    dictionary_size_exp = 16384;
    buffer_search_size_exp = 8192;
    
    test_lz77_size_of_window_treatment(level, dictionary_size_exp, buffer_search_size_exp);
}

void test_lz77_size_window_not_in_range(void)
{
    U_32 dictionary_size_exp, buffer_search_size_exp, level;

    level = 18;
    dictionary_size_exp = 4096;
    buffer_search_size_exp = 2048;

    test_lz77_size_of_window_treatment(level, dictionary_size_exp, buffer_search_size_exp);
}


//int main(void) {
//    srand(time(0));
//
//    add_test("LZ77 TEST REGULAR SIZE", test_lz77_regular_size);
//    add_test("LZ77 TEST IDENTICAL CHARACTERS", test_lz77_identical_characters);
//    add_test("LZ77 TEST EMPTY", test_lz77_empty);
//    add_test("LZ77 TEST MANY REPEAT", test_lz77_many_repeat);
//    add_test("TEST LZ77 SEARCH DICTIONARY EXIST IN MIDDLE", test_lz77_search_dictionary_exist_in_middle);
//    add_test("TEST LZ77 SEARCH DICTIONARY DONT EXIST", test_lz77_search_dictionary_dont_exist);
//    add_test("TEST LZ77 SEARCH DICTIONARY IN BEGIN", test_lz77_search_dictionary_in_begin);
//    add_test("TEST LZ77 SEARCH DICTIONARY IN END", test_lz77_search_dictionary_in_end);
//    add_test("TEST LZ77 SIZE WINDOW REGULAR", test_lz77_size_window_regular);
//    add_test("TEST LZ77 SIZE WINDOW NOT IN RANGE", test_lz77_size_window_not_in_range);
//
//    run_all_tests();
//
//    return 0;
//}
