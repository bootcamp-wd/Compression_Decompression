
#include "file_test.h"

void files_test()
{
	FILE* test_file;
	U_08 buffer[] = "data to tests";
	U_32 buffer_size = sizeof(buffer);
	write_test("test_file", buffer, buffer_size);
	read_test("test_file", buffer, buffer_size);
}

void read_test(const U_08* file_path, const U_08* exepted_buffer, U_32 buffer_size)
{
	U_32 res_buffer_size;
	U_08* res_buffer = read_file(file_path, &res_buffer_size);
	if (res_buffer_size != buffer_size)
	{
		printf("the size result are worng\n");
	}
	if (strcmp(res_buffer, exepted_buffer)) {
		printf("The data is not written and read well from the file\n");
	}
}

void write_test(const U_08* file_path, const U_08* buffer, U_32 buffer_size)
{
	write_file(file_path, buffer, buffer_size);
}

void add()
{
	
}

