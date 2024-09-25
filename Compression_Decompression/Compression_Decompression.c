#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>

#include "file_managment.h"
#include "general_define.h"
#include "getopt.h"



#define MIN_COMPRESS_LEVEL 1
#define MAX_COMPRESS_LEVEL 6
#define DEFAULT_COMPRESS_LEVEL 2

typedef enum {
    ENCODE,
    DECODE
} MODES;

/***************************************************************************
 *                            USER INTERFACE
 * Syntax: ./compressor <options>
 * Options: -c: compression mode
 *          -d: decompression mode
 *          -i <filename>: input file
 *          -o <filename>: output file
 *          -l <value> : compression level (default 6)
 *          -h: help
 ***************************************************************************/
int main(int argc, char* argv[])
{
    int opt;
    MODES mode = -1;
    U_08* filenameIn = NULL, * filenameOut = NULL;
    U_32 compress_level = DEFAULT_COMPRESS_LEVEL;

    while ((opt = getopt(argc, argv, "cdi:o:l:h")) != -1)
    {
        switch (opt)
        {
        case 'c':       /* compression mode */
            mode = ENCODE;
            break;

        case 'd':       /* decompression mode */
            mode = DECODE;
            break;

        case 'i':       /* input file name */
            if (filenameIn != NULL) 
            {
                fprintf(stderr, "Multiple input files not allowed.\n");
                goto error;
            }
            filenameIn = (U_08*)malloc(strlen(optarg) + 1);
            strcpy_s((char*)filenameIn, strlen(optarg) + 1, optarg);

            break;

        case 'o':       /* output file name */
            if (filenameOut != NULL) 
            {
                fprintf(stderr, "Multiple output files not allowed.\n");
                goto error;
            }
            filenameOut = (U_08*)malloc(strlen(optarg) + 1);
            strcpy_s((char*)filenameOut, strlen(optarg) + 1, optarg);

            break;

        case 'l':       /* compression level */
            compress_level = atoi(optarg);
            if (compress_level < MIN_COMPRESS_LEVEL || compress_level > MAX_COMPRESS_LEVEL) 
            {
                fprintf(stderr, "Bad compression level value.\n");
                goto error;
            }
            break;

        case 'h':       /* help */
            printf("Usage: compressor <options>\n");
            printf("  -c : Encode input file to output file.\n");
            printf("  -d : Decode input file to output file.\n");
            printf("  -i <filename> : Name of input file.\n");
            printf("  -o <filename> : Name of output file.\n");
            printf("  -l <value> : Compression level (default %d)\n", DEFAULT_COMPRESS_LEVEL);
            printf("  -h : Command line options.\n\n");
            return 0;

        default:
            fprintf(stderr, "Unknown option: %c\n", opt);
            goto error;
        }
    }

    /* validate command line */
    if (filenameIn == NULL) 
    {
        fprintf(stderr, "Input file must be provided\n");
        goto error;
    }
    if (filenameOut == NULL) 
    {
        fprintf(stderr, "Output file must be provided\n");
        goto error;
    }

    if (mode == ENCODE) 
    {
        printf("Compressing...\n");
        process_file(filenameIn, filenameOut, compress_level, 1);
    }
    else if (mode == DECODE)
    {
        printf("Decompressing...\n");
        process_file(filenameIn, filenameOut, compress_level, 0);
    }
    else 
    {
        fprintf(stderr, "Select ENCODE or DECODE mode\n");
        goto error;
    }

    printf("Operation completed successfully.\n");
    free(filenameIn);
    free(filenameOut);
    return 0;

    /* handle error */
error:
    if (filenameIn != NULL) free(filenameIn);
    if (filenameOut != NULL) free(filenameOut);
    exit(EXIT_FAILURE);
}