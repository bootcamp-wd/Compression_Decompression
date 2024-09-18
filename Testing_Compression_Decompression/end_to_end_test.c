#include "end_to_end_test.h"


void end_to_end_treatment(const U_08* orginal_test_file, U_08* input_buffer,U_08* suffix)
{
	U_08* compressed_test_file = "compressed_test.bin";
	U_08* decompressed_test_file = "decompressed_test";
	U_08 path_size = strlen(decompressed_test_file);
	U_08 suffix_size = strlen(suffix);
	U_08* final_path = (U_08*)malloc(path_size + suffix_size);
	strcpy_s(final_path, path_size + suffix_size + 1, decompressed_test_file);
	strcat_s(final_path, path_size + suffix_size + 1, suffix);

	U_08* res_buffer = NULL;
	size_t res_size;

	//compress
	process_file(orginal_test_file, compressed_test_file, DEFAULT_COMPRESSION_LEVEL, COMPRESS);
	//decompress
	process_file(compressed_test_file, decompressed_test_file, DEFAULT_COMPRESSION_LEVEL, UN_COMPRESS);

	
	res_buffer = read_file(final_path, &res_size);

	//compare the result 
	ASSERT(strcmp(input_buffer, res_buffer), "The data changed in the compression-decompression process ");

}

void regular_size_file_test() {
	const U_08* file_path = "input_test.txt";
	U_08 buffer[] = "LZ77 algorithm\n\
		Little background about the algorithm...\n\
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
	end_to_end_treatment(file_path, buffer, ".txt");
}

void short_file_test() {
	const U_08* file_path = "input_test.txt";
	U_08 buffer[] = "data";
	write_file(file_path, buffer, sizeof(buffer));
	end_to_end_treatment(file_path, buffer,".txt");
}

void medium_file_test() {
	const U_08* file_path = "input_test.txt";
	U_08 buffer[] = "projected revenue figures based on new product sales assumptions;and \n\
the consequences of different decisions.\n\
A decision support system is an informational application as opposed to an operational application.\n\
Informational applications provide users with relevant information based on a variety of data sources to support better - informed decision - making.\n\
Operational applications, by contrast, record the details of business transactions, including the data required for the decision - support needs of a business.\n\
Decision support system components\n\
A typical DSS consists of three different parts : knowledge database, softwareand user interface.\n\
Knowledge base.A knowledge base is an integral part of a decision support system database, containing information from both internaland external sources.\n\
Long Text In.accdb files, the Long Text field works the same as the Memo field of old.\n\
That is, it can store up to about a gigabyte of text, even though controls on formsand reports can only display the first 64, 000 characters.\n\
You can set Long Text fields to display Rich Text, which includes formatting like boldand underline.\n\
Short Text In.accdb files, the Short Text field works the same as the Text field in earlier versions.\n\
It stores up to 255 characters.\n\
Learn more about data types for Access desktop databases.\n\
Text fields in Access web apps\n\
Short Text In Access web apps the Short Text field is set to store 255 characters by default, but you can adjust the Character Limit property all the way up to 4000 characters.\n\
Its SQL Server equivalent is nvarchar, with length from 1 to 4000.\n\
Learn more about data types for Access web apps.\n\
Default controls for Long Textand Short Text fields\n\
In most cases, Access uses Text Box controls to display Short Text or Long Text fields.\n\
However, when you add a Long Text field to a view in an Access web app, Access creates a Multiline Textbox.\n\
When using a Multiline Textbox in the browser, you can press Enter to move to a new line in the textbox.\n\
If you're in a datasheet, you'll need to use the scrollbars to see anything below the first line.\n\
In Desktop databases, if a Long Text field is configured to show Rich Text, and you add that field to a form or report, Access automatically applies the Rich Text setting to the text box.\n\
Textedly lets you send texts with over 300 characters in an SMS text.\n\
That's almost twice as much as in a standard text.\n\
However, you can add a picture to send an MMS message with up to 1, 600 characters.\n\
That's a serious amount of messaging power\n\
It is a long established fact that a reader will be distracted by the readable content of a page when looking at its layout.\n\
The point of using Lorem Ipsum is that it has a more - or -less normal distribution of letters, as opposed to using Content here, content here, making it look like readable English.\n\
Many desktop publishing packagesand web page editors now use Lorem Ipsum as their default model text, and a search for lorem ipsum will uncover many web sites still in their infancy.\n\
Contrary to popular belief, Lorem Ipsum is not simply random text.\n\
It has roots in a piece of classical Latin literature from 45 BC, making it over 2000 years old.\n\
Richard McClintock, a Latin professor at Hampden - Sydney College in Virginia, looked up one of the more obscure Latin words, consectetur, from a Lorem Ipsum passage, and going through the cites of the word in classical literature, discovered the undoubtable source.\n\
Lorem Ipsum comes from sections 1.10.32 and 1.10.33 of de Finibus Bonorum et Malorum(The Extremes of Good and Evil) by Cicero, written in 45 BC.\n\
This book is a treatise on the theory of ethics, very popular during the Renaissance.\n\
The first line of Lorem Ipsum, Lorem ipsum dolor sit amet.., comes from a line in section 1.10.32.\n\
The standard Lorem Ipsum passage, used since the 1500s\n\
Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.\n\
Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.\n\
Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur.\n\
Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.\n\
Section 1.10.32 of de Finibus Bonorum et Malorum, written by Cicero in 45 BC\n\
Sed ut perspiciatis unde omnis iste natus error sit voluptatem accusantium doloremque laudantium, totam rem aperiam, eaque ipsa quae ab illo inventore veritatis et quasi architecto beatae vitae dicta sunt explicabo.\n\
Nemo enim ipsam voluptatem quia voluptas sit aspernatur aut odit aut fugit, sed quia consequuntur magni dolores eos qui ratione voluptatem sequi nesciunt.\n\
Neque porro quisquam est, qui dolorem ipsum quia dolor sit amet, consectetur, adipisci velit, sed quia non numquam eius modi tempora incidunt ut labore et dolore magnam aliquam quaerat voluptatem.\n\
Ut enim ad minima veniam, quis nostrum exercitationem ullam corporis suscipit laboriosam, nisi ut aliquid ex ea commodi consequatur ? \n\
Quis autem vel eum iure reprehenderit qui in ea voluptate velit esse quam nihil molestiae consequatur, vel illum qui dolorem eum fugiat quo voluptas nulla pariatur ? \n\
1914 translation by H.\n\
Rackham But I must explain to you how all this mistaken idea of denouncing pleasure and praising pain was born and I will give you a complete account of the system, and expound the actual teachings of the great explorer of the truth, the master - builder of human happiness.\n\
";
	write_file(file_path, buffer, sizeof(buffer));
	end_to_end_treatment(file_path, buffer, ".txt");
}

void long_file_test() {
	const U_08* file_path = "input_test.txt";
	const char* txt = "LZ77 algorithm\n\
		Little background about the algorithm...\n\
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
			end_to_end_treatment(file_path, buffer, ".txt");
			// Clean up allocated memory
			free(buffer);
		}
	}
}

void empty_file_test()
{
	const U_08* file_path = "input_test.txt";
	write_file(file_path, "", 0);
	end_to_end_treatment(file_path, "", ".txt");
}

void single_character()
{
	const U_08* file_path = "input_test.txt";
	write_file(file_path, "a", 1);
	end_to_end_treatment(file_path, "a", ".txt");
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
	end_to_end_treatment(file_path, buffer, ".txt");
}

void Image_file_test()
{
	U_32 buffer_size;
	const U_08* file_path = "input_test.png";
	U_08* buffer = read_file(file_path, &buffer_size);
	end_to_end_treatment(file_path, buffer, ".png");
}

void Audio_file_test()
{
	size_t buffer_size;
	const U_08* file_path = "input_test.m4a";
	U_08* buffer = read_file(file_path, &buffer_size);
	end_to_end_treatment(file_path, buffer, ".m4a");
}

void PDF_file_test()
{
	size_t buffer_size;
	const U_08* file_path = "input_test.pdf";
	U_08* buffer = read_file(file_path, &buffer_size);
	end_to_end_treatment(file_path, buffer,".pdf");
}

void Unknown_file_test()
{
	size_t buffer_size;
	const U_08* file_path = "input_test.asd";
	U_08* buffer = read_file(file_path, &buffer_size);
	end_to_end_treatment(file_path, buffer,".asd");
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
	end_to_end_treatment(file_path, buffer, ".txt");

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
	end_to_end_treatment(file_path, buffer, ".txt");

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
	end_to_end_treatment(file_path, buffer, ".txt");

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



