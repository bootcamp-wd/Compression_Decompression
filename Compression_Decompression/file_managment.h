#ifndef FILE_MANAGMENT_H
#define FILE_MANAGMENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "general_define.h"

U_08* read_file(const S_08* file_path, S_32* file_size);
void write_file(const S_08* file_path, const U_08* buffer, S_32 file_size);

#endif //FILE_MANAGMENT_H

