#include "file_managment.h"

unsigned S_08* read_file(const U_08* file_path, size_t* file_size)
{
    FILE* file = NULL;
    size_t bytes_read = 0;
    if (fopen_s(&file, file_path, "rb") != 0)
    {
        perror("Error opening file");
        return 0;
    }
    fseek(file, 0, SEEK_END);
    *file_size = ftell(file);
    rewind(file);
    U_08* buffer = (U_08*)malloc(*file_size * sizeof(U_08));
    if (buffer == NULL)
    {
        perror("Memory allocation failed");
        fclose(file);
        return 0;
    }
    bytes_read = fread(buffer, 1, *file_size, file);
    fclose(file);
    if (bytes_read != *file_size)
    {
        printf("Warning: Read %zu bytes, expected %d bytes\n", bytes_read, *file_size);
    }

    printf("\n");
    return buffer;
}

void write_file(const U_08* file_path, const U_08* buffer, size_t file_size)
{
    FILE* file = NULL;
    size_t bytes_written = 0;
    if (fopen_s(&file, file_path, "wb") != 0)
    {
        perror("Error opening file");
        return;
    }
    bytes_written = fwrite(buffer, 1, file_size, file);
    if (bytes_written != file_size)
    {
        printf("Warning: Wrote %d bytes, expected to write %d bytes\n", bytes_written, file_size);
    }
    fclose(file);
}

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







