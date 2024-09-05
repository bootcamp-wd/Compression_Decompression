#include "compressor_decompressor.h"
#include "file_managment.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lz77.h"
#include "huffman_code.h"

#define BUFFER_SIZE 4096

// Function to process the file (either compress or decompress)
void process_file(const U_08* input_path, const U_08* output_path, U_32 compress_level , U_32 compress)
{
    U_08* input_buffer_p = NULL, * compressed_data_buffer_p = NULL;
    U_32 file_size = 0, compressed_buffer_size = 0;
    FILE* metadata_file = NULL;
    char metadata_path[BUFFER_SIZE];
    char extension[BUFFER_SIZE] = { 0 };

    // Read the input file
    input_buffer_p = read_file(input_path, &file_size);
    if (file_size == 0)
    {
        return;
    }

    if (compress)
    {
        // Extract file extension
        const char* dot = strrchr(input_path, '.');
        if (dot && dot != input_path)
        {
            strcpy_s(extension, sizeof(extension), dot + 1);
        }

        // Store extension in metadata file
        snprintf(metadata_path, sizeof(metadata_path), "%s.meta", output_path);
        if (fopen_s(&metadata_file, metadata_path, "w") != 0)
        {
            perror("Error opening metadata file");
            free(input_buffer_p);
            return;
        }
        fprintf(metadata_file, "%s", extension);
        fclose(metadata_file);

        compress_data(input_buffer_p, file_size, &compressed_data_buffer_p, &compressed_buffer_size,compress_level);
    }
    else {
        // Read extension from metadata file
        snprintf(metadata_path, sizeof(metadata_path), "%s.meta", input_path);
        if (fopen_s(&metadata_file, metadata_path, "r") != 0)
        {
            perror("Error opening metadata file");
            free(input_buffer_p);
            return;
        }
        fscanf_s(metadata_file, "%s", extension, (unsigned)_countof(extension));
        fclose(metadata_file);

        // Append extension to output path
        snprintf(metadata_path, sizeof(metadata_path), "%s.%s", output_path, extension);

        decompress_data(input_buffer_p, file_size, &compressed_data_buffer_p, &compressed_buffer_size);

        // Update the output path with the constructed file name
        output_path = metadata_path;
    }

    // Free the input buffer
    free(input_buffer_p);

    // Write the compressed_data_buffer_p data to the output file
    write_file(output_path, compressed_data_buffer_p, compressed_buffer_size);

    // Free the compressed_data_buffer_p buffer
    free(compressed_data_buffer_p);

    printf("File compressed_data_buffer_p successfully. Input size: %d bytes, Output size: %d bytes\n", file_size, compressed_buffer_size);
}

void compress_data(const U_08* input_buffer_p, U_32 input_size,U_08* output_buffer_p, U_32* output_size, U_32 compress_level)
{
    output_buffer_p = (unsigned char*)malloc((input_size* get_size_of_encoded_sequence_struct() +sizeof(int)));//alocat the memory 
    *output_buffer_p = input_size;//save the size of the data
    if (output_buffer_p == NULL)
    {
        perror("Memory allocation failed in simulate_compress_data");
        exit(1);
    }
   lz77_encode(input_buffer_p, input_size, output_buffer_p +sizeof(int), output_size, compress_level,0);
   huffman_encode(output_buffer_p +sizeof(int), output_buffer_p + sizeof(int), *output_size, output_size);

   realloc(output_buffer_p, (output_size + sizeof(int)));//decreas the memory size after finish the comression progress 
}

void decompress_data(const U_08* input_buffer_p, U_32 input_size, U_08* output_buffer_p, U_32* output_size)
{
    *output_size = input_buffer_p;
    input_buffer_p += sizeof(int);
    *output_buffer_p = (unsigned char*)malloc(output_size);
    if (*output_buffer_p == NULL)
    {
        perror("Memory allocation failed in simulate_decompress_data");
        exit(1);
    }
    huffman_decode(input_buffer_p,input_size,output_buffer_p,output_size);
    lz77_decode(output_buffer_p,output_size,output_buffer_p);
}