#include <stdio.h>
#include <string.h>
#include "lz77.h"


void lz77_test_regular_size()
{
	const U_08* input_buffer = "abababasfadsgd igpoeritiorg figjeriog ijfiog jrhfiqe \
		jreiwoj jigodifo gejgire weiorjog jsld; gndfig er324346 bj546 vjoy";
	U_32 const input_size = strlen(input_buffer);
	U_08* output_buffer = (U_08*)malloc(input_size * 5);
	U_32 output_size;
	U_32 const compress_level = 6;
	U_08* output_data = (U_08*)malloc(strlen(input_buffer));

	lz77_encode(input_buffer, input_size, output_buffer, &output_size, compress_level);
	lz77_decode(output_buffer, &output_size, output_data);
	printf("%s", output_data);
}

int main()
{
	lz77_test_regular_size();
}