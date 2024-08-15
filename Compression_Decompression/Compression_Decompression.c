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

    // Simulate compression
    printf("Simulating compression...\n");
    process_file(original_file, compressed_file, 1);

    // Simulate decompression
    printf("\nSimulating decompression...\n");
    process_file(compressed_file, decompressed_file, 0);
}
