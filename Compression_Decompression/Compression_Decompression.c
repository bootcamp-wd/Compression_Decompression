//#include "compressor_decompressor.h"
//#include "huffman_code.h"
//#include "general_define.h"
//
//#define COMPRESS 1
//#define UN_COMPRESS 1
//#define DEFAULT_COMPRESSION_LEVEL 1
//
//int main()
//{
//	const U_08 st[28] = "april";
//	const U_08 res[100];
//	const U_08 out[28];
//	int res_size;
//	huffman_encode(st, res, 27, &res_size);
//	printf("%s", res);
//	huffman_decode(res, &res_size, out);
//	printf("%s", out);
//
//	const U_08* original_file = "input.txt";
//	const U_08* compressed_file = "compressed.bin";
//	const U_08* decompressed_file = "decompressed";
//
//	// Simulate compression
//	printf("Simulating compression...\n");
//	process_file(original_file, compressed_file, 1, 1);
//	//void process_file(const U_08* input_path, U_08* output_path, S_32 compress_level, S_32 compress)
//	printf("compression...\n");
//	process_file(original_file, compressed_file, DEFAULT_COMPRESSION_LEVEL, COMPRESS);
//
//	// Simulate decompression
//	printf("\nSimulating decompression...\n");
//	process_file(compressed_file, decompressed_file, 1, 0);
//	printf("\ndecompression...\n");
//	process_file(compressed_file, decompressed_file, DEFAULT_COMPRESSION_LEVEL, UN_COMPRESS);
//
//	//int size = 5, output_size = 0;
//	//unsigned char* compressed_data = (unsigned char*)malloc(output_size * sizeof(unsigned char));;
//
//}

//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include "compressor_decompressor.h"
//#include "huffman_code.h"
//#include "general_define.h"
//#define COMPRESS 1
//#define UN_COMPRESS 1
//#define DEFAULT_COMPRESSION_LEVEL 1
//int main()
//{
//	// "april a" -lz77 decode נופל ב
//	// יותר מ 5 אותיות במילה אחת הוא נופל
//	// "hello world !!!! - מדפיס רק את המילה הראשונה
//
//	const S_08* initial_string = "abcd efgh&ijklmn";
//	U_32 length = strlen(initial_string) + 1;  // +1 for the null terminator
//	U_08* st = (U_08*)malloc(length * sizeof(U_08));
//	memcpy(st, initial_string, length);
//
//	U_08* res = (U_08*)malloc(1000 * sizeof(U_08));
//	U_08* out = (U_08*)malloc(1000 * sizeof(U_08));
//	//const U_08 st[30] = "abcd efgh&ijklmn";
//	//const U_08 res[100];
//	//const U_08 out[100];
//	int res_size;
//	int out_size;
//	lz77_encode(st, length, res, &res_size, DEFAULT_COMPRESSION_LEVEL);
//	for (int i = 0; i < res_size; i++)
//	{
//		printf("%hi", res[i]);
//		i += 2;
//		printf("%hi", res[i]);
//		i += 2;
//		printf("%c", res[i]);
//	}
//	printf("\n");
//
//	printf("\n");
//	huffman_encode(res, out, res_size, &out_size);
//	printf("%s", res);
//	huffman_decode(out, &out_size, res);
//	printf("%s", res);
//	int size1 = 66;
//	lz77_decode(res, &size1, out);
//	for (int i = 0; i < length; i++)
//	{
//		printf("%c", out[i]);
//	}
//	printf("\n");
//	free(st);
//	free(res);
//	free(out);
//	const U_08* original_file = "input.png";
//	const U_08* compressed_file = "compressed.bin";
//	const U_08* decompressed_file = "decompressed";
//	printf("compression...\n");
//	process_file(original_file, compressed_file, DEFAULT_COMPRESSION_LEVEL, COMPRESS);
//	printf("\ndecompression...\n");
//	process_file(compressed_file, decompressed_file, DEFAULT_COMPRESSION_LEVEL, UN_COMPRESS);
//	//int size = 5, output_size = 0;
//	//unsigned char* compressed_data = (unsigned char*)malloc(output_size * sizeof(unsigned char));;
//}





#include "compressor_decompressor.h"
#include "huffman_code.h"
#include "general_define.h"

#define COMPRESS 1
#define UN_COMPRESS 0
#define DEFAULT_COMPRESSION_LEVEL 1

int main()
{
    const U_08 st[30] = "a";
    U_08* res = (U_08*)malloc(strlen(st) * get_size_of_encoded_sequence_struct() * sizeof(U_08));
    U_08* out = (U_08*)malloc(1000 * sizeof(U_08));
    //U_08 res[1000];  // Output buffer for lz77_encode
    //U_08 out[1000];  // Output buffer for huffman_encode
    int res_size;
    int out_size;
    // LZ77 Encoding
    lz77_encode(st, 4, res, &res_size, DEFAULT_COMPRESSION_LEVEL);
    /*prints of lz77_encode*/
    printf("res_size after lz77_encode: %d\n", res_size);
    printf("res of LZ_77 encode:\n");
    for (int i = 0; i < res_size; i+=2)
    {
        printf("%hi", res[i]);
        i += 2;
        printf("%hi", res[i]);
        i += 2;
        printf("%c", res[i]);
    }
    printf("\n");

    // Print LZ77 encoded result in hex format
    printf("LZ77 Encoded Result: ");
    for (int i = 0; i < res_size; i++) {
        printf("%02x ", res[i]);
    }
    printf("\n");

    // Huffman Encoding
    huffman_encode(res, out, res_size, &out_size);
    printf("res_size after huffman_encode: %d\n", res_size);
    printf("out_size after huffman_encode: %d\n", out_size);

    // Print Huffman encoded result in hex format
    printf("Huffman Encoded Result: ");
    for (int i = 0; i < out_size; i++) {
        printf("%02x ", out[i]);
    }
    printf("\n");



    // Huffman Decoding
    huffman_decode(out, &out_size, res);
    printf("out_size after huffman_decode: %d\n", out_size);

    // Print Huffman decoded result
    printf("Huffman Decoded Result: ");
    for (int i = 0; i < out_size; i++) {
        printf("%02x ", res[i]);
    }
    printf("\n");

    // LZ77 Decoding
    lz77_decode(res, &res_size, out);
    printf("res_size after lz77_decode: %d\n", res_size);
    for (int i = 0; i < 4; i++)
    {
        printf("%c", out[i]);
    }
    printf("\n");
    // Print LZ77 decoded result
    printf("LZ77 Decoded Result: \n");
    for (int i = 0; i < res_size; i++) {
        printf("%02x ", out[i]);
    }
    printf("\n");


    const U_08* original_file = "input.txt";
    const U_08* compressed_file = "compressed.bin";
    const U_08* decompressed_file = "decompressed";

    printf("compression...\n");
    process_file(original_file, compressed_file, DEFAULT_COMPRESSION_LEVEL, COMPRESS);

    printf("\ndecompression...\n");
    process_file(compressed_file, decompressed_file, DEFAULT_COMPRESSION_LEVEL, UN_COMPRESS);

    //int size = 5, output_size = 0;
    //unsigned char* compressed_data = (unsigned char*)malloc(output_size * sizeof(unsigned char));;

}
