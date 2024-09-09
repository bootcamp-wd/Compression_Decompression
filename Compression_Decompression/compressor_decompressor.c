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
void process_file(const U_08* input_path, U_08* output_path, S_32 compress_level, S_32 compress)
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

        //calling to function that treats the metadata of the file
        metadata_treatment(output_path, extension, metadata_path, metadata_file, buffer, "w");
        compress_data(buffer, file_size, &processed, &processed_size, compress_level);
    }

    //if the compress is zero - the file needs to be decompressed
    else
    {
        metadata_treatment(input_path, extension, metadata_path, metadata_file, buffer, "r");
        snprintf(metadata_path, sizeof(metadata_path), "%s.%s", output_path, extension);
        output_path = metadata_path;
        decompress_data(buffer, file_size, &processed, &processed_size);
    }
    if (buffer)
    {
        free(buffer);
    }
    write_file(output_path, processed, processed_size);

    if (processed)
    {
        free(processed);
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
    U_32* output_size, S_32 compress_level)
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

    //huffman_output = *output_buffer + sizeof(U_32);
    lz77_encode(input_buffer, input_size, lz77_output, &lz77_output_size, compress_level);

    printf("res_size after lz77_encode: %d\n", input_size);
    printf("res of LZ_77 encode:\n");
    for (int i = 0; i < lz77_output_size; i += 2)
    {
        printf("%hi", lz77_output[i]);
        i += 2;
        printf("%hi", lz77_output[i]);
        i += 2;
        printf("%c", lz77_output[i]);
    }
    printf("\n");

    /*prints of lz77_encode*/
    printf("res_size after lz77_encode: %d\n", lz77_output_size);
    printf("res of LZ_77 encode:\n");
    for (int i = 0; i < lz77_output_size; i += 2)
    {
        printf("%hi", lz77_output[i]);
        i += 2;
        printf("%hi", lz77_output[i]);
        i += 2;
        printf("%c", lz77_output[i]);
    }
    printf("\n");

    //huffman_output += (*output_size);
    huffman_encode(lz77_output, huffman_output, lz77_output_size, &huffman_output_size);

    printf("res_size after huffman_encode: %d\n", lz77_output_size);
    printf("out_size after huffman_encode: %d\n", huffman_output_size);

    // Print Huffman encoded result in hex format
    printf("Huffman Encoded Result: ");
    for (int i = 0; i < huffman_output_size; i++) {
        printf("%02x ", huffman_output[i]);
    }
    printf("\n");

    *output_size = sizeof(S_32) + huffman_output_size;
    *output_buffer = (U_08*)malloc(*output_size);
    if (output_buffer == NULL)
    {
        perror("Memory reallocation failed in compress_data");
        free(lz77_output);
        free(huffman_output);
        exit(1);
    }
    // Write the input size to the first 4 bytes of the output buffer
    memcpy(*output_buffer, &input_size, sizeof(U_32)); 

    // Copy the Huffman output after the input size
    memcpy(*output_buffer + sizeof(U_32), huffman_output, huffman_output_size);

    printf("%d ", *output_buffer[0]);
    for (int i = 0; i < *output_size; i++) {
        printf("%02x ", (*output_buffer)[i]);
    }
    printf("\n");

    if (lz77_output)
    {
        free(lz77_output);

    }
   /* if (huffman_output)
    {
        free(huffman_output);
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
void decompress_data(const U_08* input_buffer, U_32 input_size, U_08** output_buffer, U_32* output_size)
{
  /*  printf("Decompress function ");
    for (int i = 0; i < input_size; i++) {
        printf("%02x ", input_buffer[i]);
    }
    printf("\n");*/
    *output_size = (U_32)*input_buffer;
    U_08* input_huffman = input_buffer + sizeof(U_32);
    
    U_08* output_huffman = (U_08*)malloc(*output_size * sizeof(U_08) * 100);
    if (output_huffman == NULL)
    {
        perror("Memory allocation failed in decompress_data");
        exit(1);
    }
    
    /*printf("Huffman Encoded Result: ");
    for (int i = 0; i < input_size - *output_size; i++) {
        printf("%02x ", input_huffman[i]);
    }
    printf("\n");*/
    U_32 input_size_huffman = input_size - *output_size;
    huffman_decode(input_huffman, &input_size_huffman, output_huffman);
    //void huffman_decode(U_08* input_buffer_p, U_32* input_size, U_08 * output_buffer_p)
    lz77_decode(*output_buffer, output_size, *output_buffer);

    *output_buffer = (U_08*)malloc(*output_size * sizeof(U_08) * 10);
    if (*output_buffer == NULL)
    {
        perror("Memory allocation failed in decompress_data");
        exit(1);
    }
}

//Function to treat the metadata of the file
void metadata_treatment(const U_08* file_path, U_08* extension, U_08* metadata_path, FILE* metadata_file,
    U_08* buffer, const U_08* read_write_mode)
{
    assert(read_write_mode != NULL && (strcmp(read_write_mode, "w") == 0 || strcmp(read_write_mode, "r") == 0));
    snprintf(metadata_path, sizeof(metadata_path), "%s.meta", file_path);
    if (fopen_s(&metadata_file, metadata_path, read_write_mode) != 0)
    {
        perror("Error opening metadata file");
        if (buffer != NULL)
            free(buffer);
        return;
    }
    fscanf_s(metadata_file, "%s", extension, (unsigned)_countof(extension));
    fclose(metadata_file);
}
