#include "compressor_decompressor.h"

/**************************************************************************
*						            PROCESS FUNCTION
* Name			: process - process the file (either compress or decompress)
* Parameters	: input_path, output_path - names of the input and output pathes
*				  compress_level - gets the level of compressing, more compressed is less fast
*                 compress - a variable that gets one if the file needs to be compressed,
*                            and zero if it needs to be decompressed
* Returned		: none
* *************************************************************************/
void process_file(const U_08* input_path, U_08* output_path, U_32 compress_level, U_32 compress)
{
    U_08* buffer = NULL;
    U_08* processed = NULL;
    U_32 file_size, processed_size;
    FILE* metadata_file = NULL;
    U_08 metadata_path[BUFFER_SIZE];
    U_08 extension[BUFFER_SIZE] = { 0 };

    buffer = read_file(input_path, &file_size);
    if (file_size == 0)
    {
        return;
    }

    if (compress)
    {
        const U_08* dot = strrchr(input_path, '.');
        if (dot && dot != input_path)
        {
            strcpy_s(extension, sizeof(extension), dot + 1);
        }

        snprintf(metadata_path, sizeof(metadata_path), "%s.meta", output_path);
        if (fopen_s(&metadata_file, metadata_path, "w") != 0)
        {
            perror("Error opening metadata file");
            free(buffer);
            buffer = NULL;
            return;
        }
        fprintf(metadata_file, "%s", extension);
        fclose(metadata_file);

        compress_data(buffer, file_size, &processed, &processed_size, compress_level);
    }

    //if the compress is zero - the file needs to be decompressed
    else
    {
        // Read extension from metadata file
        snprintf(metadata_path, sizeof(metadata_path), "%s.meta", input_path);
        if (fopen_s(&metadata_file, metadata_path, "r") != 0)
        {
            perror("Error opening metadata file");
            free(buffer);
            buffer = NULL;
            return;
        }
        fscanf_s(metadata_file, "%s", extension, (unsigned)_countof(extension));
        fclose(metadata_file);

        // Append extension to output path
        snprintf(metadata_path, sizeof(metadata_path), "%s.%s", output_path, extension);

        decompress_data(buffer, file_size, &processed, &processed_size);
        // Update the output path with the constructed file name
        output_path = metadata_path;
    }
    if (buffer)
    {
        free(buffer);
        buffer = NULL;
    }
    write_file(output_path, processed, processed_size);

    if (processed)
    {
        free(processed);
        processed = NULL;
    }
    printf("File processed successfully. Input size: %d bytes, Output size: %d bytes\n", file_size, processed_size);
}

/**************************************************************************
*						            COMPRESS DATA FUNCTION
* Name			: compress_data - the compression with calling to the algorithms that do it
* Parameters	: input_buffer, the data that needs to be compressed, input_size - size of the input
*				  output_buffer - buffer for storing the result
*                 output_size - a variable to save the size of output
*                 compress_level - gets the level of compressing, more compressed is less fast
* Returned		: none
* *************************************************************************/
void compress_data(const U_08* input_buffer, U_32 input_size, U_08** output_buffer,
    U_32* output_size, U_32 compress_level)
{
    U_08* lz77_output = (U_08*)malloc(input_size * get_size_of_encoded_sequence_struct() * sizeof(U_08));
    if (lz77_output == NULL)
    {
        perror("Memory allocation failed for LZ77 output");
        exit(1);
    }
    U_08* huffman_output = (U_08*)malloc(input_size * get_size_of_encoded_sequence_struct() * sizeof(U_08));
    if (huffman_output == NULL)
    {
        perror("Memory allocation failed for Huffman output");
        exit(1);
    }
    int lz77_output_size, huffman_output_size;

    lz77_encode(input_buffer, input_size, lz77_output, &lz77_output_size, compress_level);

    huffman_encode(lz77_output, huffman_output, lz77_output_size, &huffman_output_size);

    *output_size = sizeof(U_32) + huffman_output_size;
    *output_buffer = (U_08*)malloc((*output_size) * sizeof(U_08));
    if (*output_buffer == NULL) // Check the dereferenced pointer
    {
        perror("Memory allocation failed in compress_data");
        free(lz77_output);
        lz77_output = NULL;
        free(huffman_output);
        huffman_output = NULL;
        exit(1);
    }
    // Write the input size to the first 4 bytes of the output buffer
    **(U_32**)output_buffer = input_size;    

    // Copy the Huffman output after the input size
    memcpy(*output_buffer + sizeof(U_32), huffman_output, huffman_output_size);

    if (lz77_output)
    {
        free(lz77_output);
        lz77_output = NULL;
    }
    if (huffman_output)
    {
        free(huffman_output);
        huffman_output = NULL;
    }
}

/**************************************************************************
*						            DECOMPRESS DATA FUNCTION
* Name			: decompress_data - the decompression with calling to the algorithms that do it
* Parameters	: input_buffer, the data that needs to be compressed
                  input_size - size of the input
*				  output_buffer - buffer for storing the result
*                 output_size - a variable to save the size of output
* Returned		: none
* *************************************************************************/
void decompress_data(const U_08* input_buffer, U_32 input_size, U_08** output_buffer, U_32* output_size)
{
    *output_size = (U_32)*input_buffer;
    U_08* input_huffman = input_buffer + sizeof(U_32);
    U_32 size = sizeof(Huffman_decode_node);
    U_32 size_output_huffman = (input_size + INVALID_INDEX * sizeof(Huffman_decode_node) +sizeof(U_32)) * sizeof(U_08);
    
    U_08* huffman_output = (U_08*)malloc(size_output_huffman);
    if (huffman_output == NULL)
    {
        perror("Memory allocation failed in decompress_data");
        exit(1);
    }

    U_32 input_size_huffman = input_size - sizeof(U_32);
    U_32 output_size_huffman = 0;
    huffman_decode(input_huffman, &input_size_huffman, huffman_output,&output_size_huffman);

    U_08* lz77_output = (U_08*)malloc((output_size_huffman * 16384) * sizeof(U_08));
    if (lz77_output == NULL)
    {
        perror("Memory allocation failed in decompress_data");
        exit(1);
    }

    //void huffman_decode(U_08* input_buffer_p, U_32* input_size, U_08 * output_buffer_p)
    lz77_decode(huffman_output, &output_size_huffman, lz77_output);
    
    *output_buffer = (U_08*)malloc(*output_size * sizeof(U_08));
    if (*output_buffer == NULL)
    {
        perror("Memory allocation failed for output_buffer");
        free(huffman_output);
        huffman_output = NULL;
        free(lz77_output);
        lz77_output = NULL;
        exit(1);
    }

    // Correct the memcpy call
    memcpy(*output_buffer, lz77_output, *output_size);

    // Clean up
    if (huffman_output)
    {
        free(huffman_output);
        huffman_output = NULL;
    }
    if (lz77_output)
    {
        free(lz77_output);
        lz77_output = NULL;
    }
}

////Function to treat the metadata of the file
//void metadata_treatment(const U_08* file_path, U_08* extension, U_08* metadata_path, FILE* metadata_file,
//    U_08* buffer, const U_08* read_write_mode)
//{
//    assert(read_write_mode != NULL && (strcmp(read_write_mode, "w") == 0 || strcmp(read_write_mode, "r") == 0));
//    snprintf(metadata_path, sizeof(metadata_path), "%s.meta", file_path);
//    if (fopen_s(&metadata_file, metadata_path, read_write_mode) != 0)
//    {
//        perror("Error opening metadata file");
//        if (buffer != NULL)
//        {
//            free(buffer);
//            buffer = NULL;
//        }
//        return;
//    }
//    fscanf_s(metadata_file, "%s", extension, (unsigned)_countof(extension));
//    fclose(metadata_file);
//}
