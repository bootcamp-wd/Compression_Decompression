#ifndef FILE_MANAGMENT_H
#define FILE_MANAGMENT_H

unsigned char* read_file(const char* file_path, int* size);
void write_file(const char* file_path, const unsigned char* buffer, int size);

#endif FILE_MANAGMENT_H