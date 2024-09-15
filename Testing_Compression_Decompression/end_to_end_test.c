#include "end_to_end_test.h"


void end_to_end_treatment(const U_08* orginal_test_file, U_08* input_buffer)
{
	U_08* compressed_test_file = "compressed_test.bin";
	U_08* decompressed_test_file = "decompressed_test";

	U_08* res_buffer = NULL;
	size_t res_size;

	//compress
	process_file(orginal_test_file, compressed_test_file, DEFAULT_COMPRESSION_LEVEL, COMPRESS);
	//decompress
	process_file(compressed_test_file, decompressed_test_file, DEFAULT_COMPRESSION_LEVEL, UN_COMPRESS);

	res_buffer = read_file(decompressed_test_file, &res_size);

	//compare the result 
	ASSERT(strcmp(input_buffer, res_buffer), "The data changed in the compression-decompression process ");

}

void regular_size_file_test() {
	const U_08* file_path = "input_test.txt";
	U_08 buffer[] = "LZ77 algorithm\n\
		Little background about the algorithm…\n\
		LZ77 algorithm compresses memory cells according to repetitions of sequences.\n\
		The algorithm represents each sequence that is identical to a previous sequence using the distance from the identical sequence, and its length.\n\
		The algorithm uses a window, the window is placed at any current moment on a part of the input.\n\
		The window is divided into 2 parts - the first part is the dictionary, in this part the algorithm looks for the repeated templates, and the second part is the buffer_search, this is the sequence on which it is checked if it appears in the first part - in the dictionary.\n\
		dictionary - it uses 2 pointers to point to the beginning and the end of the part that is being checked if the sequence in buffer_search appears.\n\
		buffer_search - it uses 2 pointers to point the start and the end of the current sequence that is looking for now.\n\
		Therefore, the smaller the dictionary - the speed will be greater and the compression ratio smaller, and the larger the dictionary will be, the speed will decrease and the compression ratio will increase.\n\
		so, the size of the dictionary will be according to input from the user, he will enter if he wants faster or more compressed and at what level and the compression will be according to the input.\n\
		The default option is a balance between compression ratio and speed.\n\
		The algorithm saves for each sequence a struct that includes :\n\
	    position: distance from the first byte of the identical sequence found in dictionary.\n\
		length : the length of the identical sequence.\n\
		actual explanation\n\
		The algorithm goes through the input, and appends each byte to the sequence that exists in the buffer_search and then checks if the current buffer_search is in the dictionary. if so, increases the end pointer of the buffer_search and adds to the current struct the position of the beginning of the sequence and the length of the sequence. if it's not found - the current byte goes into the struct at the location of the last_byte. then the struct is added to the result and the entire buffer_search points to the next byte.\n\
		The position and length are initialized to zero, in case a return is found they are updated if there is a byte that doesn't exist in dictionary it will be written to last_byte and they remain reset.\n\
		In any case, the dictionary moves to include the current byte as well.\n\
		finally, the size is updated according to the size of the result.\n\
		An Example of the LZ77 algorithm :\n\
	";
	write_file(file_path, buffer, sizeof(buffer));
	end_to_end_treatment(file_path, buffer);
}

void short_file_test() {
	const U_08* file_path = "input_test.txt";
	U_08 buffer[] = "data";
	write_file(file_path, buffer, sizeof(buffer));
	end_to_end_treatment(file_path, buffer);
}

void long_file_test() {
	const U_08* file_path = "input_test.txt";
	const char* txt = "LZ77 algorithm\n\
		Little background about the algorithm…\n\
		LZ77 algorithm compresses memory cells according to repetitions of sequences.\n\
		The algorithm represents each sequence that is identical to a previous sequence using the distance from the identical sequence, and its length.\n\
		The algorithm uses a window, the window is placed at any current moment on a part of the input.\n\
		The window is divided into 2 parts - the first part is the dictionary, in this part the algorithm looks for the repeated templates, and the second part is the buffer_search, this is the sequence on which it is checked if it appears in the first part - in the dictionary.\n\
		dictionary - it uses 2 pointers to point to the beginning and the end of the part that is being checked if the sequence in buffer_search appears.\n\
		buffer_search - it uses 2 pointers to point the start and the end of the current sequence that is looking for now.\n\
		Therefore, the smaller the dictionary - the speed will be greater and the compression ratio smaller, and the larger the dictionary will be, the speed will decrease and the compression ratio will increase.\n\
		so, the size of the dictionary will be according to input from the user, he will enter if he wants faster or more compressed and at what level and the compression will be according to the input.\n\
		The default option is a balance between compression ratio and speed.\n\
		The algorithm saves for each sequence a struct that includes :\n\
	    position: distance from the first byte of the identical sequence found in dictionary.\n\
		length : the length of the identical sequence.\n\
		actual explanation\n\
		The algorithm goes through the input, and appends each byte to the sequence that exists in the buffer_search and then checks if the current buffer_search is in the dictionary. if so, increases the end pointer of the buffer_search and adds to the current struct the position of the beginning of the sequence and the length of the sequence. if it's not found - the current byte goes into the struct at the location of the last_byte. then the struct is added to the result and the entire buffer_search points to the next byte.\n\
		The position and length are initialized to zero, in case a return is found they are updated if there is a byte that doesn't exist in dictionary it will be written to last_byte and they remain reset.\n\
		In any case, the dictionary moves to include the current byte as well.\n\
		finally, the size is updated according to the size of the result.\n\
		An Example of the LZ77 algorithm :\n\
	";
	size_t txt_size = strlen(txt);

	size_t buffer_size = (txt_size * 100001); // Adjust buffer size calculation
	U_08* buffer = (U_08*)malloc(buffer_size);

	if (buffer != NULL)
	{
		if (strcpy_s(buffer, buffer_size, txt) == 0) // Copy txt to buffer securely 
		{
			for (int i = 0; i < 3; i++)
			{
				strncat_s(buffer, buffer_size, txt, txt_size); // Concatenate txt to buffer securely
			}
			end_to_end_treatment(file_path, buffer, buffer_size);
			// Clean up allocated memory
			free(buffer);
		}
	}
}

void empty_file_test()
{
	const U_08* file_path = "input_test.txt";
	write_file(file_path, "", 0);
	end_to_end_treatment(file_path, "");
}

void single_character()
{
	const U_08* file_path = "input_test.txt";
	write_file(file_path, "a", 1);
	end_to_end_treatment(file_path, "a");
}

void Text_file_test()
{
	const U_08* file_path = "input_test.txt";
	U_08 buffer[] = "The algorithm goes through the input, and appends each byte to the sequence that exists in the buffer_search and then checks if the current buffer_search is in the dictionary. if so, increases the end pointer of the buffer_search and adds to the current struct the position of the beginning of the sequence and the length of the sequence. if it's not found - the current byte goes into the struct at the location of the last_byte. then the struct is added to the result and the entire buffer_search points to the next byte.\n\
		The position and length are initialized to zero, in case a return is found they are updated if there is a byte that doesn't exist in dictionary it will be written to last_byte and they remain reset.\n\
		In any case, the dictionary moves to include the current byte as well.\n\
		finally, the size is updated according to the size of the result.\n\
		An Example of the LZ77 algorithm";
	write_file(file_path, buffer, sizeof(buffer));
	end_to_end_treatment(file_path, buffer);
}

void Image_file_test()
{
	size_t buffer_size;
	const U_08* file_path = "input_test.png";
	U_08* buffer = read_file(file_path, &buffer_size);
	end_to_end_treatment(file_path, buffer);
}

void Audio_file_test()
{
	size_t buffer_size;
	const U_08* file_path = "input_test.m4a";
	U_08* buffer = read_file(file_path, &buffer_size);
	end_to_end_treatment(file_path, buffer);
}

void PDF_file_test()
{
	size_t buffer_size;
	const U_08* file_path = "input_test.pdf";
	U_08* buffer = read_file(file_path, &buffer_size);
	end_to_end_treatment(file_path, buffer);
}

void Unknown_file_test()
{
	size_t buffer_size;
	const U_08* file_path = "input_test.asd";
	U_08* buffer = read_file(file_path, &buffer_size);
	end_to_end_treatment(file_path, buffer);
}

void random_file_test()
{
	size_t buffer_size = 100000;
	const U_08* file_path = "input_test.txt";
	U_08* buffer = (U_08*)malloc(buffer_size);

	for (U_32 i = 0; i < buffer_size; i++)
	{
		// Generate random printable ASCII characters (from space (32) to tilde (126))
		buffer[i] = (U_08)rand() % 256;
	}
	buffer[buffer_size - 1] = '\0'; // Null-terminate the string
	write_file(file_path, buffer, buffer_size);
	end_to_end_treatment(file_path, buffer);

	free(buffer);
}

void all_ascii_file_test()
{
	U_32 buffer_size = 255;
	const U_08* file_path = "input_test.txt";
	U_08* buffer = (U_08*)malloc(buffer_size);

	for (U_32 i = 0; i < buffer_size; i++)
	{
		buffer[i] = (U_08)i;
	}
	buffer[buffer_size - 1] = '\0'; // Null-terminate the string
	write_file(file_path, buffer, buffer_size);
	end_to_end_treatment(file_path, buffer);

	free(buffer);
}

void repeating_single_character_file_test()
{
	U_32 buffer_size = rand() % 10000;
	const U_08* file_path = "input_test.txt";
	U_08* buffer = (U_08*)malloc(buffer_size);
	U_08 character = (U_08)rand() % 256;
	for (U_32 i = 0; i < buffer_size; i++)
	{

		buffer[i] = character;
	}
	buffer[buffer_size - 1] = '\0'; // Null-terminate the string
	write_file(file_path, buffer, buffer_size);
	end_to_end_treatment(file_path, buffer);

	free(buffer);
}

void not_exist_file_test()
{
	const U_08* file_path = "not_exist_file.txt";
	process_file(file_path, file_path, DEFAULT_COMPRESSION_LEVEL, COMPRESS);
}

void Incorrect_path()
{
	const U_08* file_path = "../../data/input_test.txt";
	process_file(file_path, file_path, DEFAULT_COMPRESSION_LEVEL, UN_COMPRESS);
}



