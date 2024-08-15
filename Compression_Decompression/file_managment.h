#ifndef FILE_MANAGMENT_H
#define FILE_MANAGMENT_H

unsigned char* read_file(const char* file_path, int* file_size);
void write_file(const char* file_path, const unsigned char* buffer, int file_size);

#endif //FILE_MANAGMENT_H

