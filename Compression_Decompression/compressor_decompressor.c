
//#include "compressor_decompressor.h"
//#include "file_managment.h"
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include "lz77.h"
//#include "huffman_code.h"
//
//#define BUFFER_SIZE 4096
//
//// Function to process the file (either compress or decompress)
//void process_file(const U_08* input_path, const U_08* output_path,U_32 compress_level ,U_32 compress)
//{
//    unsigned char* buffer = NULL, * processed = NULL;
//    int file_size = 0, processed_size = 0;
//    FILE* metadata_file = NULL;
//    char metadata_path[BUFFER_SIZE];
//    char extension[BUFFER_SIZE] = { 0 };
//
//    // Read the input file
//    buffer = read_file(input_path, &file_size);
//    if (file_size == 0)
//    {
//        return;
//    }
//
//    if (compress)
//    {
//        // Extract file extension
//        const char* dot = strrchr(input_path, '.');
//        if (dot && dot != input_path)
//        {
//            strcpy_s(extension, sizeof(extension), dot + 1);
//        }
//
//        // Store extension in metadata file
//        snprintf(metadata_path, sizeof(metadata_path), "%s.meta", output_path);
//        if (fopen_s(&metadata_file, metadata_path, "w") != 0)
//        {
//            perror("Error opening metadata file");
//            free(buffer);
//            return;
//        }
//        fprintf(metadata_file, "%s", extension);
//        fclose(metadata_file);
//
//        // Simulate compression
//        simulate_compress_data(buffer, file_size, &processed, &processed_size,compress_level);
//    }
//    else {
//        // Read extension from metadata file
//        snprintf(metadata_path, sizeof(metadata_path), "%s.meta", input_path);
//        if (fopen_s(&metadata_file, metadata_path, "r") != 0)
//        {
//            perror("Error opening metadata file");
//            free(buffer);
//            return;
//        }
//        fscanf_s(metadata_file, "%s", extension, (unsigned)_countof(extension));
//        fclose(metadata_file);
//
//        // Append extension to output path
//        snprintf(metadata_path, sizeof(metadata_path), "%s.%s", output_path, extension);
//
//        // Simulate decompression
//        simulate_decompress_data(buffer, file_size, &processed, &processed_size);
//
//        // Update the output path with the constructed file name
//        output_path = metadata_path;
//    }
//
//    // Free the input buffer
//    free(buffer);
//
//    // Write the processed data to the output file
//    write_file(output_path, processed, processed_size);
//
//    // Free the processed buffer
//    free(processed);
//
//    printf("File processed successfully. Input size: %d bytes, Output size: %d bytes\n", file_size, processed_size);
//}
//
//// Function to simulate compression by simply copying data
//void simulate_compress_data(const unsigned char* input_buffer, int input_size,
//    unsigned char* output_buffer, int* output_size,int compress_level)
//{
//    output_buffer = (unsigned char*)malloc((input_size* get_size_of_encoded_sequence_struct() +sizeof(int)));//alocat the memory 
//    *output_buffer = input_size;//save the size of the data
//    if (output_buffer == NULL)
//    {
//        perror("Memory allocation failed in simulate_compress_data");
//        exit(1);
//    }
//   lz77_encode(input_buffer, input_size, output_buffer+sizeof(int), output_size, compress_level,0);
//   huffman_encode(output_buffer+sizeof(int), output_buffer + sizeof(int), output_size, output_size);
//
//   realloc(output_buffer, (output_size + sizeof(int)));//decreas the memory size after finish the comression progress 
//}
//
//// Function to simulate decompression by simply copying data
//void decompress_data(const U_08* input_buffer, U_32 input_size, unsigned char* output_buffer, U_32* output_size)
//{
//    *output_size = input_buffer;
//    input_buffer += sizeof(int);
//    *output_buffer = (unsigned char*)malloc(output_size);
//    if (*output_buffer == NULL)
//    {
//        perror("Memory allocation failed in simulate_decompress_data");
//        exit(1);
//    }
//    huffman_decode(input_buffer,input_size,output_buffer,output_size);
//    lz77_decode(output_buffer,output_size,output_buffer);
//}
