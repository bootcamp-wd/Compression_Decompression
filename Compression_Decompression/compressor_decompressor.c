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
void process_file(const S_08* input_path, const S_08* output_path, S_32 compress_level, S_32 compress)
{
    U_08* buffer = NULL, * processed = NULL;
    S_32 file_size = 0, processed_size = 0;
    FILE* metadata_file = NULL;
    S_08 metadata_path[BUFFER_SIZE];
    S_08 extension[BUFFER_SIZE] = { 0 };

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

    free(buffer);
    write_file(output_path, processed, processed_size);
    free(processed);
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
    *output_buffer = (U_08*)malloc((input_size * get_size_of_encoded_sequence_struct() + sizeof(S_32))); 
     if (*output_buffer == NULL)
     {
        perror("Memory allocation failed in compress_data");
        exit(1);
     }
    //saving the size of the data in the output to use it in the decompress
    **output_buffer = input_size;

   lz77_encode(input_buffer, input_size, *output_buffer + sizeof(U_32), output_size, compress_level);
   huffman_encode(output_buffer + sizeof(U_32), *output_buffer + sizeof(U_32), *output_size, output_size);

   //decreas the memory size after finish the comression progress
   U_08* temp_buffer = (U_08*)realloc(*output_buffer, (*output_size + sizeof(S_32)));
   if (temp_buffer == NULL)
   {
       perror("Memory reallocation failed in compress_data");
       free(*output_buffer);
       exit(1);
   }
   *output_buffer = temp_buffer;
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
void decompress_data(const U_08* input_buffer, S_32 input_size, U_08** output_buffer, S_32* output_size)
{
    *output_size = input_buffer;
    input_buffer += sizeof(int);
    *output_buffer = (U_08*)malloc(output_size);
    if (*output_buffer == NULL)
    {
        perror("Memory allocation failed in decompress_data");
        exit(1);
    }

    huffman_decode(input_buffer,input_size,output_buffer,output_size);
    lz77_decode(output_buffer,output_size,output_buffer);
}

//Function to treat the metadata of the file
void metadata_treatment(const S_08* file_path, S_08* extension, const S_08* metadata_path, FILE* metadata_file,
    U_08* buffer, const S_08* read_write_mode)
{
    assert(read_write_mode != NULL && (strcmp(read_write_mode, "w") == 0 || strcmp(read_write_mode, "r") == 0));
    snprintf(metadata_path, sizeof(metadata_path), "%s.meta", file_path);
    if (fopen_s(&metadata_file, metadata_path, read_write_mode) != 0)
    {
        perror("Error opening metadata file");
        if(buffer != NULL)
            free(buffer);
        return;
    }
    fscanf_s(metadata_file, "%s", extension, (unsigned)_countof(extension));
    fclose(metadata_file);
}
