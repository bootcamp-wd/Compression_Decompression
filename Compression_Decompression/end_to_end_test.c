#include "end_to_end_test.h"#include "end_to_end_test.h"

void end_to_end_treatment(U_08* buffer, U_32 buffer_size,U_08* test_name, void (*test_function)(void))
{
	FILE* input_file;
	FILE* output_file;
	fopen("input_file.txt", "wb+");
	
	fwrite(buffer, buffer_size, 1, input_file);
	//compress
	process_file("input_file.txt", "output_file", 2,1);
   //decompress
	process_file("output_file", "outpu_file.txt", 2,0);
  U_08* res_buffer= fread(buffer, 1, buffer_size, output_file);
	//check
  if (!strcmp(buffer, res_buffer)) {
	  printf("the data had changed in the compression-decompression progress");
  }
//add to tests
   add_test(test_name,test_function);
}

void regular_file_size() {
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
	end_to_end_treatment(buffer, sizeof(buffer), "regular size of file test", regular_file_size);

}
void short_file_size() {
	U_08 buffer[] = "data";
	end_to_end_treatment(buffer, sizeof(buffer), "short size of file test", short_file_size);
}
void long_file_size() {
	U_08 buffer;

U_08 txt[] = "LZ77 algorithm\n\
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
U_32 txt_size = sizeof(txt);
U_08 buffer = malloc((U_08)(10000 * txt_size));
//increas the file content
for (int i = 0; i < 100000; i++) {
	strncat(buffer, txt, txt_size);
}
end_to_end_treatment(buffer, sizeof(buffer), "long size of file test", long_file_size);
}
