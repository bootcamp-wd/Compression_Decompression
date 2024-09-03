#include "lz77_test.h"

#include "test_framework.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>

// פונקציה בסיסית לבדיקת קידוד ופריסת LZ77
void lz77_test_treatment(const char* test_name, U_08* input_buffer, U_32 input_size, U_32 compress_level) {
    U_08* compressed_buffer = (U_08*)malloc(input_size * get_size_of_encoded_sequence_struct());
    U_08* decompressed_buffer = (U_08*)malloc(input_size);
    U_32 compressed_size = 0;

    lz77_encode(input_buffer, input_size, compressed_buffer, &compressed_size, compress_level);
    ASSERT(compressed_size > 0, "Compression size should be greater than 0");

    lz77_decode(compressed_buffer, &compressed_size, decompressed_buffer);
    ASSERT_EQUAL(input_size, compressed_size, "Decompressed size should match the original input size");

    for (U_32 i = 0; i < input_size; i++) {
        ASSERT(input_buffer[i] == decompressed_buffer[i], "Decompressed data does not match original");
    }

    printf("Test passed: %s\n", test_name);

    free(compressed_buffer);
    free(decompressed_buffer);
}

void test_lz77_regular_size(void) {
    U_08 input_buffer[] = "Lesson I / Recap\n\
		Setting up a React Environment\n\
		Run this command to create a React application named my - react - app:\n\
	npx create - react - app my - react - app\n\
		The create - react - app will set up everything you need to run a React application.\n\
		Run the React Application\n\
		Run this command to move to the my - react - app directory :\n\
	cd my - react - app\n\
		Run this command to run the React application my - react - app :\n\
		npm start\n\
		A new browser window will pop up with your newly created React App!If not, open your browserand type \
        localhost : 3000 in the address bar.\n\
		React components\n\
		●	React lets you create components, reusable UI elements for your app.\n\
		●	In a React app, every piece of UI is a component.\n\
		●	React components are regular JavaScript functions except :\n\
	1.	Their names always begin with a capital letter.";
    U_32 input_size = strlen((char*)input_buffer);
    U_32 compress_level = rand() % 7;

    lz77_test_treatment("LZ77 Test Regular Size", input_buffer, input_size, compress_level);
}

void test_lz77_identical_characters(void) {
    U_08 input_buffer[] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    U_32 input_size = strlen((char*)input_buffer);
    U_32 compress_level = rand() % 7;

    lz77_test_treatment("LZ77 Test Identical Characters", input_buffer, input_size, compress_level);
}

void test_lz77_empty(void) {
    U_08 input_buffer[] = "";
    U_32 input_size = strlen((char*)input_buffer);
    U_32 compress_level = rand() % 7;

    lz77_test_treatment("LZ77 Test Empty", input_buffer, input_size, compress_level);
}

void test_lz77_many_repeat(void) {
    U_08 input_buffer[] = "aaaaaaaaaaaaaaaabbbbbbbbbbbbbbbbbbbbbbbbbbbaaaaabbbbbbbbcccccccccccccccdddddddddddddddddddddaaaaaaaabbbbcccccccccbbbbddd";
    U_32 input_size = strlen((char*)input_buffer);
    U_32 compress_level = rand() % 7;

    lz77_test_treatment("LZ77 Test Many Repeat", input_buffer, input_size, compress_level);
}

int main(void) {
    srand(time(0));

    add_test("LZ77 Test Regular Size", test_lz77_regular_size);
    add_test("LZ77 Test Identical Characters", test_lz77_identical_characters);
    add_test("LZ77 Test Empty", test_lz77_empty);
    add_test("LZ77 Test Many Repeat", test_lz77_many_repeat);

    run_all_tests();

    return 0;
}
