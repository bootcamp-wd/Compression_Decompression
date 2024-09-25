#include "algorithms_test.h"

void test_algorithms_lz77_and_huffman(void)
{
	const U_08* input_lz77 = "of the";
    size_t input_size_lz77 = strlen(input_lz77);
    U_08* output_lz77 = (U_08*)malloc(input_size_lz77 * get_size_of_encoded_sequence_struct() * 100);
    size_t output_size_lz77 = 0;
    lz77_encode(input_lz77, input_size_lz77, output_lz77, &output_size_lz77, 5);

    U_08* output_huffman = (U_08*)malloc((output_size_lz77 + sizeof(Huffman_metadata)) * 100);
	printf("0x%x\n", output_huffman);
	printf("0x%x -4\n", *(U_32*)(output_huffman - 4));
	printf("0x%x -8\n", *(U_32*)output_huffman - 8);
	printf("0x%x -12\n", *(U_32*)output_huffman - 12);
	printf("0x%x -16\n", *(U_32*)output_huffman - 16);
    size_t output_size_huffman = 0;
    huffman_encode(output_lz77, output_huffman, output_size_lz77, &output_size_huffman);

	printf("0x%x\n", output_huffman);
	printf("0x%x -4\n", *(U_32*)(output_huffman - 4));
	printf("0x%x -8\n", *(U_32*)output_huffman - 8);
	printf("0x%x -12\n", *(U_32*)output_huffman - 12);
	printf("0x%x -16\n", *(U_32*)output_huffman - 16);

    U_08* output_huffman_decode = (U_08*)malloc(output_size_lz77 * 100);
    size_t output_size_huffman_decode;
    huffman_decode(output_huffman, &output_size_huffman, output_huffman_decode, &output_size_huffman_decode);

	printf("0x%x\n", output_huffman);
	printf("0x%x -4\n", *(U_32*)(output_huffman - 4));
	printf("0x%x -8\n", *(U_32*)output_huffman - 8);
	printf("0x%x -12\n", *(U_32*)output_huffman - 12);
	printf("0x%x -16\n", *(U_32*)output_huffman - 16);
    ASSERT_EQUAL(output_size_lz77, output_size_huffman_decode, "output size of huffman decode and lz77 encode");
    U_08* result_end = (U_08*)malloc(input_size_lz77 * 100);
	size_t size1 = 36;
    lz77_decode(output_huffman_decode, &output_size_huffman_decode, result_end);
    ASSERT(strcmp(input_lz77, result_end, input_size_lz77), "The decode result is wrong");
	printf("0x%x\n", output_huffman);
	printf("0x%x -4\n", *(U_32*)(output_huffman - 4));
	printf("0x%x -8\n", *(U_32*)output_huffman - 8);
	printf("0x%x -12\n", *(U_32*)output_huffman - 12);
	printf("0x%x -16\n", *(U_32*)output_huffman - 16);
	free(output_lz77);
    free(output_huffman);
    free(output_huffman_decode);
    free(result_end);
}
