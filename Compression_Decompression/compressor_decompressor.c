#include "compressor_decompressor.h"



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
    file_metadata sizes_for_decompression;
    sizes_for_decompression.original_size = input_size;

    U_08* lz77_output = (U_08*)malloc(input_size * get_size_of_encoded_sequence_struct());
    if (lz77_output == NULL)
    {
        perror("Memory allocation failed for LZ77 output");
        exit(1);
    }
    U_08* huffman_output = (U_08*)malloc(input_size * get_size_of_encoded_sequence_struct());
    if (huffman_output == NULL)
    {
        perror("Memory allocation failed for Huffman output");
        exit(1);
    }
    size_t lz77_output_size = 0;
    size_t huffman_output_size = 0;

    lz77_encode(input_buffer, input_size, lz77_output, &lz77_output_size, compress_level);

    sizes_for_decompression.size_output_huffman = lz77_output_size;

    huffman_encode(lz77_output, huffman_output, lz77_output_size, &huffman_output_size);

    *output_size = sizeof(file_metadata) + huffman_output_size;
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
    **(file_metadata**)output_buffer = sizes_for_decompression;

    // Copy the Huffman output after the input size
    memcpy(*output_buffer + sizeof(file_metadata), huffman_output, huffman_output_size);

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
void decompress_data(const U_08* input_buffer, size_t input_size, U_08** output_buffer, size_t* output_size)
{
    /*  printf("Decompress function ");
      for (int i = 0; i < input_size; i++) {
          printf("%02x ", input_buffer[i]);
      }
      printf("\n");*/
    file_metadata sizes;
    sizes = *(file_metadata*)input_buffer;

    *output_size = sizes.original_size;

    //U_32 size = sizeof(Huffman_decode_node);
    U_08* huffman_output = (U_08*)malloc(sizes.size_output_huffman);
    if (huffman_output == NULL)
    {
        perror("Memory allocation failed in decompress_data");
        exit(1);
    }

    const U_08* input_huffman = input_buffer + sizeof(sizes);
    size_t input_size_huffman = input_size - sizeof(file_metadata);
    size_t output_size_huffman = 0;
    huffman_decode(input_huffman, &input_size_huffman, huffman_output, &output_size_huffman);
    assert(output_size_huffman == sizes.size_output_huffman);
    
    //U_08* lz77_output = (U_08*)malloc((output_size_huffman * 16384) * sizeof(U_08));
    U_08* lz77_output = (U_08*)malloc(sizes.original_size);
    if (lz77_output == NULL)
    {
        perror("Memory allocation failed in decompress_data");
        exit(1);
    }

    lz77_decode(huffman_output, &output_size_huffman, lz77_output);

    *output_buffer = (U_08*)malloc(sizes.original_size);
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
