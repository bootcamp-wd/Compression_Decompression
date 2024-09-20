#include "file_managment.h"
//Function to read a file into a buffer
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
// Function to write a buffer to a file
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









