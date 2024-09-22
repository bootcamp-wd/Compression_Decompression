#ifndef FILE_MANAGMENT_H
#define FILE_MANAGMENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "general_define.h"

#define BUFFER_SIZE 4096

U_08* read_file(const U_08* file_path, size_t* file_size);
void write_file(const U_08* file_path, const U_08* buffer, size_t file_size);
void process_file(const U_08* input_path, const U_08* output_path, U_32 compress_level, U_32 compress);



#endif //FILE_MANAGMENT_H

