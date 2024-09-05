//#include "lz77_test.h"
//
//
//void lz77_testing()
//{
//	U_08* input_buffer = NULL;
//	U_32 input_size = 0;
//	U_08* compressed_buffer = NULL;
//	U_32 compressed_size = 0;
//	srand(time(0));
//	U_32 const compress_level = rand() % 7;
//	U_08* decompressed_buffer = NULL;
//
//	lz77_test_regular_size(input_buffer, &input_size, compressed_buffer, &compressed_size, compress_level, decompressed_buffer);
//	lz77_test_identical_characters(input_buffer, &input_size, compressed_buffer, &compressed_size, compress_level, decompressed_buffer);
//	lz77_test_empty(input_buffer, &input_size, compressed_buffer, &compressed_size, compress_level, decompressed_buffer);
//	lz77_test_many_repeat(input_buffer, &input_size, compressed_buffer, &compressed_size, compress_level, decompressed_buffer);
//
//	if (compressed_buffer)
//		free(compressed_buffer);
//	if (decompressed_buffer)
//		free(decompressed_buffer);
//}
//
//void lz77_test_treatment(U_08* input_buffer, U_32 input_size, U_08* compressed_buffer, U_32 compressed_size, 
//	const U_32 compress_level ,U_08* decompressed_buffer)
//{
//	input_size = strlen(input_buffer);
//	compressed_buffer = (U_08*)malloc(input_size * get_size_of_encoded_sequence_struct());
//	decompressed_buffer = (U_08*)malloc(strlen(input_buffer));
//
//	lz77_encode(input_buffer, input_size, compressed_buffer, &compressed_size, compress_level);
//	lz77_decode(compressed_buffer, &compressed_size, decompressed_buffer);
//	for (U_32 i = 0; i < compressed_size / get_size_of_encoded_sequence_struct(); i++)
//	{
//		if (input_buffer[i] != decompressed_buffer[i])
//		{
//			printf("FAILED TEST \"LZ77 TEST EMPTY\n");
//			return;
//		}
//	}
//	printf("TEST \"LZ77 TEST MANY REPEAT\" PASSED!\n");
//	puts(decompressed_buffer);
//}
//
//void lz77_test_regular_size(U_08* input_buffer, U_32* input_size, U_08* compressed_buffer, U_32* compressed_size,
//	const U_32 compress_level, U_08* decompressed_buffer)
//{
//	input_buffer = " Lesson I / Recap\
//		Setting up a React Environment\
//		Run this command to create a React application named my - react - app:\
//	npx create - react - app my - react - app\
//		The create - react - app will set up everything you need to run a React application.\
//		Run the React Application\
//		Run this command to move to the my - react - app directory :\
//	cd my - react - app\
//		Run this command to run the React application my - react - app :\
//		npm start\
//		A new browser window will pop up with your newly created React App!If not, open your browserand type localhost : 3000 in the address bar.\
//		React components\
//		●	React lets you create components, reusable UI elements for your app.\
//		●	In a React app, every piece of UI is a component.\
//		●	React components are regular JavaScript functions except :\
//	1.	Their names always begin with a capital letter.";
//
//	lz77_test_treatment(input_buffer, *input_size, compressed_buffer, *compressed_size, compress_level, decompressed_buffer);
//
//}
//
//void lz77_test_identical_characters(U_08* input_buffer, U_32* input_size, U_08* compressed_buffer, U_32* compressed_size,
//	const U_32 compress_level, U_08* decompressed_buffer)
//{
//	input_buffer = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
//	lz77_test_treatment(input_buffer, *input_size, compressed_buffer, *compressed_size, compress_level, decompressed_buffer);
//	
//}
//
//void lz77_test_empty(U_08* input_buffer, U_32* input_size, U_08* compressed_buffer, U_32* compressed_size,
//	const U_32 compress_level, U_08* decompressed_buffer)
//{
//	input_buffer = "";
//
//	lz77_test_treatment(input_buffer, *input_size, compressed_buffer, *compressed_size, compress_level, decompressed_buffer);
//}
//
//void lz77_test_many_repeat(U_08* input_buffer, U_32* input_size, U_08* compressed_buffer, U_32* compressed_size,
//	const U_32 compress_level, U_08* decompressed_buffer)
//{
//	input_buffer = "aaaaaaaaaaaaaaaabbbbbbbbbbbbbbbbbbbbbbbbbbbaaaaabbbbbbbbcccccccccccccccdddddddddddddddddddddaaaaaaaabbbbcccccccccbbbbddd";
//
//	lz77_test_treatment(input_buffer, *input_size, compressed_buffer, *compressed_size, compress_level, decompressed_buffer);
//}
