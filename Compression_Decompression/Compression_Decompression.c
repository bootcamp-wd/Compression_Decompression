#include <stdio.h>
#include <string.h>
#include "lz77.h"
#include "file_managment.h"
#include "compressor_decompressor.h"
#include "huffman_code.h"


int main()
{
	const char st[28] = "april";
	const char res[100];
	const char out[28];
	int res_size;
	huffman_encode(st, res, 27, &res_size);
	//printf("%s", res);
	huffman_decode(res, &res_size, out);
}






