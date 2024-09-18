#include "file_managment.h"

//Function to read a file into a buffer
unsigned U_08* read_file(const U_08* file_path, U_32* file_size)
{
    log_debug("Starting Read_file function");

    FILE* file = NULL;
    U_32 bytes_read = 0;

    if (fopen_s(&file, file_path, "rb") != 0)
    {
        log_error("Error opening file: %s", file_path);
        return 0;
    }
    log_info("File opened successfully: %s", file_path);

    fseek(file, 0, SEEK_END);
    *file_size = ftell(file);
    rewind(file);
    log_debug("File size determined: %d bytes", *file_size);

    U_08* buffer = (U_08*)malloc(*file_size * sizeof(U_08));
    if (buffer == NULL)
    {
        log_fatal("Memory allocation failed while reading file.");
        fclose(file);
        return 0;
    }
    log_debug("Memory allocated for buffer of size: %d bytes", *file_size);

    bytes_read = fread(buffer, 1, *file_size, file);
    fclose(file);
    log_info("File read completed. Bytes read: %zu", bytes_read);

    if (bytes_read != *file_size)
    {
        log_warn("Warning: Read %zu bytes, expected %d bytes", bytes_read, *file_size);
    }
    return buffer;
}

// Function to write a buffer to a file
void write_file(const U_08* file_path, const U_08* buffer, U_32 file_size)
{
    log_debug("Starting Write_file function");

    FILE* file = NULL;
    U_32 bytes_written = 0;

    if (fopen_s(&file, file_path, "wb") != 0)
    {
        log_error("Error opening file for writing: %s", file_path);
        return;
    }
    log_info("File opened successfully for writing: %s", file_path);

    bytes_written = fwrite(buffer, 1, file_size, file);
    if (bytes_written != file_size)
    {
        log_warn("Warning: Wrote %zu bytes, expected to write %d bytes", bytes_written, file_size);
    }

    fclose(file);
    log_info("File writing completed. Total bytes written: %zu", bytes_written);
}