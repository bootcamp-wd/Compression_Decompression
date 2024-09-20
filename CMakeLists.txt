cmake_minimum_required(VERSION 3.29)
project(Compression_Decompression)

set(CMAKE_CXX_STANDARD 14)

include_directories(Compression_Decompression)
include_directories(Testsing_Compression_Decompression)

add_executable(Compression_Decompression
    Compression_Decompression/Compression_Decompression.c
    Compression_Decompression/compressor_decompressor.c
    Compression_Decompression/compressor_decompressor.h
    Compression_Decompression/file_managment.c
    Compression_Decompression/file_managment.h
    Compression_Decompression/file_test.c
    Compression_Decompression/file_test.h
    Compression_Decompression/general_define.c
    Compression_Decompression/general_define.h
    Compression_Decompression/general_defines.c
    Compression_Decompression/huffman_code.c
    Compression_Decompression/huffman_code.h
    Compression_Decompression/huffman_struct.h
    Compression_Decompression/huffman_test.c
    Compression_Decompression/log.c
    Compression_Decompression/log.h
    Compression_Decompression/logger.c
    Compression_Decompression/logger.h
    Compression_Decompression/lz77.c
    Compression_Decompression/lz77.h
    Compression_Decompression/lz77_test.c
    Compression_Decompression/lz77_test.h
    Compression_Decompression/priority_queue.c
    Compression_Decompression/priority_queue.h
    Compression_Decompression/resource.h
    Compression_Decompression/Source.cpp
        Testsing_Compression_Decompression/lz77_test.c
        Testsing_Compression_Decompression/lz77_test.h
        Testsing_Compression_Decompression/main.c
        Testsing_Compression_Decompression/test_framework.c
        Testsing_Compression_Decompression/test_framework.h)
