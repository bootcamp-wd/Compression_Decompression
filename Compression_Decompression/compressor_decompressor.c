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
void process_file(const U_08* input_path, const U_08* output_path, U_32 compress_level, U_32 compress)
{
    U_08* input_buffer_p = NULL;
    U_08* compressed_data_buffer_p = NULL;
    size_t file_size, compressed_buffer_size;
    FILE* metadata_file = NULL;
    U_08 metadata_path[BUFFER_SIZE];
    U_08 extension[BUFFER_SIZE] = { 0 };

    input_buffer_p = read_file(input_path, &file_size);
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
            free(input_buffer_p);
            return;
        }
        fprintf(metadata_file, "%s", extension);
        fclose(metadata_file);

        compress_data(input_buffer_p, file_size, &compressed_data_buffer_p, &compressed_buffer_size, compress_level);
    }

    //if the compress is zero - the file needs to be decompressed
    else
    {
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
    if (input_buffer_p)
    {
        free(input_buffer_p);
    }
    write_file(output_path, compressed_data_buffer_p, compressed_buffer_size);

    if (compressed_data_buffer_p)
    {
        free(compressed_data_buffer_p);
    }
    printf("File processed successfully. Input size: %d bytes, Output size: %d bytes\n", file_size, compressed_buffer_size);
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
void compress_data(const U_08* input_buffer, size_t input_size, U_08** output_buffer,
    size_t* output_size, U_32 compress_level)
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
    size_t lz77_output_size = 0;
    size_t huffman_output_size = 0;

    lz77_encode(input_buffer, input_size, lz77_output, &lz77_output_size, compress_level);

    huffman_encode(lz77_output, huffman_output, lz77_output_size, &huffman_output_size);

    *output_size = sizeof(size_t) + huffman_output_size;
    *output_buffer = (U_08*)malloc((*output_size));
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
    **(size_t**)output_buffer = input_size;

    // Copy the Huffman output after the input size
    memcpy(*output_buffer + sizeof(size_t), huffman_output, huffman_output_size);

    if (lz77_output)
    {
        free(lz77_output);
        lz77_output = NULL;
    }
    /* if (huffman_output)
     {
         free(huffman_output);
         huffman_output = NULL;
     }*/
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
void decompress_data(const U_08* input_buffer, size_t input_size, U_08** output_buffer, size_t* output_size)
{
    output output;
    output.original_size = *(size_t*)input_buffer;

    *output_size = output.original_size;
    const U_08* input_huffman = input_buffer + sizeof(output);

    //U_32 size = sizeof(Huffman_decode_node);
    size_t size_output_huffman = (input_size + INVALID_INDEX * sizeof(Huffman_decode_node) + sizeof(size_t)) * sizeof(U_08);
    U_08* huffman_output = (U_08*)malloc(size_output_huffman);
    if (huffman_output == NULL)
    {
        perror("Memory allocation failed in decompress_data");
        exit(1);
    }

    size_t input_size_huffman = input_size - sizeof(size_t);
    size_t output_size_huffman = 0;
    huffman_decode(input_huffman, &input_size_huffman, huffman_output, &output_size_huffman);

    U_08* lz77_output = (U_08*)malloc((output_size_huffman * 16384) * sizeof(U_08));
    if (lz77_output == NULL)
    {
        perror("Memory allocation failed in decompress_data");
        exit(1);
    }

    lz77_decode(huffman_output, &output_size_huffman, lz77_output);

    *output_buffer = (U_08*)malloc(*output_size);
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
