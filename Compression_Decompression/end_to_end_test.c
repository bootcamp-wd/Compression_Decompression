#include "end_to_end_test.h"
//const U_08* original_file = "input.txt";
//const U_08* compressed_file = "compressed.bin";
//const U_08* decompressed_file = "decompressed";
//
//printf("compression...\n");
//process_file(original_file, compressed_file, DEFAULT_COMPRESSION_LEVEL, COMPRESS);
//
//printf("\ndecompression...\n");
//process_file(compressed_file, decompressed_file, DEFAULT_COMPRESSION_LEVEL, UN_COMPRESS);


void end_to_end_treatment(const U_08* orginal_test_file, U_08* input_buffer)
{
	 U_08* compressed_test_file = "compressed_test.bin";
	 U_08* decompressed_test_file = "decompressed_test";

	U_08* res_buffer=NULL;
	size_t res_size;

	//compress
	process_file(orginal_test_file, compressed_test_file, DEFAULT_COMPRESSION_LEVEL, COMPRESS);
    //decompress
	process_file(compressed_test_file,decompressed_test_file ,DEFAULT_COMPRESSION_LEVEL, UN_COMPRESS);
	 
	//compare the result 
	res_buffer=read_file(decompressed_test_file,&res_size);
    if (!strcmp(input_buffer, res_buffer))
	{
	  printf("the data had changed in the compression-decompression process");
    }
    //close the files- ereas
    /*fclose(orginal_test_file);
	fclose(compressed_test_file);
	fclose(decompressed_test_file);*/
}

void regular_size_file_test() {
	const U_08* file_path = "orginal_test_file.txt";
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
	end_to_end_treatment(file_path,buffer);
}
void short_file_test() {
	const U_08* file_path = "orginal_test_file.txt";
	U_08 buffer[] = "data";
	write_file(file_path, buffer, sizeof(buffer));
	end_to_end_treatment(file_path,buffer);
}
void long_file_test() {
	const char* txt = "Hello";
	size_t txt_size = strlen(txt);

	size_t buffer_size = (txt_size * 100001); // Adjust buffer size calculation
	char* buffer = (char*)malloc(buffer_size);

	if (buffer != NULL) {
		if (strcpy_s(buffer, buffer_size, txt) == 0) { // Copy txt to buffer securely
			for (int i = 0; i < 3; i++) {
				strncat_s(buffer, buffer_size, txt, txt_size); // Concatenate txt to buffer securely
			}

			// Output the final concatenated string
			printf("Final concatenated string: %s\n", buffer);

			// Clean up allocated memory
			free(buffer);
		}
		else {
			// Handle copying failure
			printf("Error copying string\n");
		}
	}
	else {
		// Handle memory allocation failure
		printf("Memory allocation failed\n");
	}

}

void Text_file_test()
{
	const U_08* file_path = "orginal_test_file.txt";
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
	size_t size;
	const U_08* file_path = "input.png";
	U_08* buffer =read_file(file_path,&size);
	end_to_end_treatment(file_path, buffer);
}

void Audio_file_test()
{

}

void PDF_file_test()
{
}

void Unknown_file_test()
{
}

void random_file_test()
{
	/*void generate_random_input(U_08 * buffer, U_32 size)*/
	//{
	//	for (U_32 i = 0; i < size; i++)
	//	{
	//		// Generate random printable ASCII characters (from space (32) to tilde (126))
	//		buffer[i] = rand() % 256;
	//	}
	//	buffer[size - 1] = '\0'; // Null-terminate the string
	//}
}
