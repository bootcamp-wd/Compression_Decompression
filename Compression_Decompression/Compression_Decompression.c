#include <stdio.h>
#include <string.h>
#include "lz77.h"
#include "file_managment.h"
#include "compressor_decompressor.h"
#include "huffman_code.h"


U_32 main()
{
	const U_08 st[28] = "april";
	const U_08 res[100];
	const U_08 out[28];
	U_32 res_size;
	huffman_encode(st, res, 5, &res_size);
	//prU_32f("%s", res);
	huffman_decode(res, &res_size, out);
}






