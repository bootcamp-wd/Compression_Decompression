//
//
//
//#define DEFAULT_COMPRESSION_LEVEL 6
//
//
//#include <stdlib.h>
//#include <stdio.h>
//#include <string.h>
//#include "getopt.h"
//#include "lz77.h"
#include "compressor_decompressor.h"
#include "huffman_code.h"
#include "general_define.h"
//
///***************************************************************************
// *                                CONSTANTS
// ***************************************************************************/
//#define MIN_LA_SIZE 2       /* min lookahead size */
//#define MAX_LA_SIZE 255     /* max lookahead size */
//#define MIN_SB_SIZE 0       /* min search buffer size */
//#define MAX_SB_SIZE 65535   /* max search buffer size */
//
// /***************************************************************************
//  *                            TYPE DEFINITIONS
//  ***************************************************************************/
//typedef enum {
//    ENCODE,
//    DECODE
//} MODES;
//
///***************************************************************************
// *                            USER INTERFACE
// * Syntax: ./lz77 <options>
// * Options: -c: compression mode
// *          -d: decompression mode
// *          -i <filename>: input file
// *          -o <filename>: output file
// *          -l <value> : lookahead size (default 15)
// *          -s <value> : search-buffer size (default 4095)
// *          -h: help
// ***************************************************************************/
//int main(int argc, char* argv[])
//{
//    /* variables */
//    int opt;
//    FILE* file = NULL;
//    struct bitFILE* bitF = NULL;
//    MODES mode = -1;
//    char* filenameIn = NULL, * filenameOut = NULL;
//    int la_size = -1, sb_size = -1; /* default size */
//
//    while ((opt = getopt(argc, argv, "cdi:o:l:s:h")) != -1)
//    {
//        switch (opt)
//        {
//        case 'c':       /* compression mode */
//            mode = ENCODE;
//            break;
//
//        case 'd':       /* decompression mode */
//            mode = DECODE;
//            break;
//
//        case 'i':       /* input file name */
//            if (filenameIn != NULL) {
//                fprintf(stderr, "Multiple input files not allowed.\n");
//                goto error;
//            }
//            filenameIn = malloc(strlen(optarg) + 1);
//            strcpy(filenameIn, optarg);
//
//            break;
//
//        case 'o':       /* output file name */
//            if (filenameOut != NULL) {
//                fprintf(stderr, "Multiple output files not allowed.\n");
//                goto error;
//            }
//            filenameOut = malloc(strlen(optarg) + 1);
//            strcpy(filenameOut, optarg);
//
//            break;
//
//        case 'l':       /* lookahead size */
//            la_size = atoi(optarg);
//            if (la_size < MIN_LA_SIZE || la_size > MAX_LA_SIZE) {
//                fprintf(stderr, "Bad lookahead size value.\n");
//                goto error;
//            }
//            break;
//
//        case 's':       /* search-buffer size */
//            sb_size = atoi(optarg);
//            if (sb_size < MIN_SB_SIZE || sb_size > MAX_SB_SIZE) {
//                fprintf(stderr, "Bad search-buffer size value.\n");
//                goto error;
//            }
//            break;
//
//        case 'h':       /* help */
//            printf("Usage: lz77 <options>\n");
//            printf("  -c : Encode input file to output file.\n");
//            printf("  -d : Decode input file to output file.\n");
//            printf("  -i <filename> : Name of input file.\n");
//            printf("  -o <filename> : Name of output file.\n");
//            printf("  -l <value> : Lookahead size (default 15)\n");
//            printf("  -s <value> : Search-buffer size (default 4095)\n");
//            printf("  -h : Command line options.\n\n");
//            break;
//
//        }
//    }
//
//    /* validate command line */
//    if (filenameIn == NULL) {
//        fprintf(stderr, "Input file must be provided\n");
//        goto error;
//    }
//    else if (filenameOut == NULL)
//    {
//        fprintf(stderr, "Output file must be provided\n");
//        goto error;
//    }
//
//    if (mode == ENCODE) {
//        if ((file = fopen(filenameIn, "rb")) == NULL) {
//            perror("Opening input file");
//            goto error;
//        }
//        if ((bitF = bitIO_open(filenameOut, BIT_IO_W)) == NULL) {
//            perror("Opening output file");
//            goto error;
//        }
//        encode(file, bitF, la_size, sb_size);
//
//    }
//    else if (mode == DECODE) {
//        if ((bitF = bitIO_open(filenameIn, BIT_IO_R)) == NULL) {
//            perror("Opening input file");
//            goto error;
//        }
//        if ((file = fopen(filenameOut, "w")) == NULL) {
//            perror("Opening output file");
//            goto error;
//        }
//        decode(bitF, file);
//
//    }
//    else {
//        fprintf(stderr, "Select ENCODE or DECODE mode\n");
//        goto error;
//    }
//
//    fclose(file);
//    bitIO_close(bitF);
//    return 0;
//
//    /* handle error */
//error:
//    if (file != NULL) {
//        fclose(file);
//    }
//    if (bitF != NULL) {
//        bitIO_close(bitF);
//    }
//    exit(EXIT_FAILURE);
//}
int main()
{
    

const U_08* original_file = "tatauranga-umanga-maori-statistics-on-maori-businesses-2020-CSV-te-reo.csv";
    const U_08* compressed_file = "decompressed_compressed_.bin";
    const U_08* decompressed_file = "decompressed_decompressed";

    printf("compression...\n");
    process_file(original_file, compressed_file, 6, 1);

    printf("\ndecompression...\n");
    process_file(compressed_file, decompressed_file, 6, 0);

    //int size = 5, output_size = 0;
    //unsigned char* compressed_data = (unsigned char*)malloc(output_size * sizeof(unsigned char));;

}
