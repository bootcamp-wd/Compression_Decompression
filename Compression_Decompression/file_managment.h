#ifndef FILE_MANAGMENT_H
#define FILE_MANAGMENT_H

#include <stdio.h>
#include "general_define.h"

unsigned char* read_file(const U_08* file_path, U_32* file_size);
void write_file(const U_08* file_path, const U_08* buffer, U_32 file_size);

#endif //FILE_MANAGMENT_H

