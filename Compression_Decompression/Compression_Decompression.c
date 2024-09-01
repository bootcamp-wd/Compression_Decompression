#include <stdio.h>
#include <string.h>
#include "lz77.h"
#include "file_managment.h"
#include "compressor_decompressor.h"


int main()
{
    const char* original_file = "input.png";
    const char* compressed_file = "compressed.bin";
    const char* decompressed_file = "decompressed";

 const char st[28] = "april";
 const char res[100];
 const char out[28];
 int res_size;
 huffman_encode(st, res, 27, &res_size);
 //printf("%s", res);
 huffman_decode(res, &res_size,out);






    //// Simulate compression
    //printf("Simulating compression...\n");
    //process_file(original_file, compressed_file, 1);

    //// Simulate decompression
    //printf("\nSimulating decompression...\n");
    //process_file(compressed_file, decompressed_file, 0);
}
