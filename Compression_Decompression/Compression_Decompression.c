// Compression_Decompression.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <string.h>
#include "lz77.h"

int main()
{
	unsigned char* input_data = "abcabd";
	unsigned char* output_data;
	int window_size = 5;
	int input_size = strlen(input_data) * sizeof(unsigned char), output_size;

	output_data = (unsigned char*)malloc(input_size * 5);
	if (output_data == NULL)
	{
		output_size = 0;
		printf("Error! Memory allocation failed in lz77_encode\n");
		exit(1);
	}

	lz77_encode(input_data, input_size, output_data, &output_size, 12, 12);

	for (int i = 0; i < output_size; i+=2)
	{
		printf("%hi", output_data[i]);
		i += 2;
		printf("%hi", output_data[i]);
		i += 2;
		printf("%c", output_data[i]);
	}
	printf("\n");
	unsigned char* output_buffer_lz_decode = (unsigned char*)malloc(6 * sizeof(char));

	lz77_decode(output_data, &output_size, output_buffer_lz_decode);
	for (int i = 0; i < strlen(input_data); i++)
	{
		printf("%c", output_buffer_lz_decode[i]);
	}

	free(output_data);
}

