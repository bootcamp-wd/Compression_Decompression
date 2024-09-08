#include "end_to_end_test.h"

void end_to_end_treatment()
{

	void process_file(const U_08 * input_path, U_08 * output_path, S_32 compress_level, S_32 compress)
}

void regular_file_size() {
   U_08* buffer = "LZ77 algorithm
	   Little background about the algorithm…
	   LZ77 algorithm compresses memory cells according to repetitions of sequences.
	   The algorithm represents each sequence that is identical to a previous sequence using the distance from the identical sequence, and its length.
	   The algorithm uses a window, the window is placed at any current moment on a part of the input.
	   The window is divided into 2 parts - the first part is the dictionary, in this part the algorithm looks for the repeated templates, and the second part is the buffer_search, this is the sequence on which it is checked if it appears in the first part - in the dictionary.
	   dictionary - it uses 2 pointers to point to the beginning and the end of the part that is being checked if the sequence in buffer_search appears.
	   buffer_search - it uses 2 pointers to point the start and the end of the current sequence that is looking for now.
	   Therefore, the smaller the dictionary - the speed will be greater and the compression ratio smaller, and the larger the dictionary will be, the speed will decrease and the compression ratio will increase.
	   so, the size of the dictionary will be according to input from the user, he will enter if he wants faster or more compressed and at what level and the compression will be according to the input.
	   The default option is a balance between compression ratio and speed.
	   The algorithm saves for each sequence a struct that includes :
   position: distance from the first byte of the identical sequence found in dictionary.
	   length : the length of the identical sequence.
	   last_byte : the byte that stops the sequence.
	   actual explanation
	   The algorithm goes through the input, and appends each byte to the sequence that exists in the buffer_search and then checks if the current buffer_search is in the dictionary. if so, increases the end pointer of the buffer_search and adds to the current struct the position of the beginning of the sequence and the length of the sequence. if it's not found - the current byte goes into the struct at the location of the last_byte. then the struct is added to the result and the entire buffer_search points to the next byte.
	   The position and length are initialized to zero, in case a return is found they are updated if there is a byte that doesn't exist in dictionary it will be written to last_byte and they remain reset.
	   In any case, the dictionary moves to include the current byte as well.
	   finally, the size is updated according to the size of the result.
	   An Example of the LZ77 algorithm :
   "
}
void short_file_size() {

}
void long_file_size() {

}