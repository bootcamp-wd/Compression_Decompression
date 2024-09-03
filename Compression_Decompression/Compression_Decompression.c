//#include <stdio.h>
//#include <string.h>
#include "lz77_test.h"
#include "file_managment.h"
#include "compressor_decompressor.h"
#include "huffman_code.h"


U_32 main()
{
<<<<<<< HEAD
	lz77_testing();
=======
	const U_08 st[28] = "april";
	const U_08 res[100];
	const U_08 out[28];
	U_32 res_size;
	huffman_encode(st, res, 5, &res_size);
	//prU_32f("%s", res);
	huffman_decode(res, &res_size, out);
>>>>>>> ada1d3ac05c1222ba5b2bd9f232e29c6c82c20ed
}
