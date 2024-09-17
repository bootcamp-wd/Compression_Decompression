#include "algorithms_test.h"

void test_algorithms_lz77_and_huffman(void)
{
    const U_08* input_lz77 = "hi everybody hello world !!!! hi everybody hello world !!!! \n\
        hi everybody hello world !!!!hi everybody hello world !!!!hi everybody hello world \
        !!!!hi everybody hello world !!!!hi everybody hello world !!!!hi everybody hello\
        world !!!!hi  hi everybody hello world !!!!hi everybody hello world !!!!hi everybody hello world !!!!hi \
hi everybody hello world !!!! hi everybody hello world !!!! \n\
        hi everybody hello world !!!!hi everybody hello world !!!!hi everybody hello world \
        !!!!hi everybody hello world !!!!hi everybody hello world !!!!hi everybody hello\
        world !!!!hi  hi everybody hello world !!!!hi everybody hello world !!!!hi everybody hello world !!!!hi \
What is a decision support system (DSS)?\n\
A decision support system (DSS) is a computer program application used to improve a company's decision-making capabilities.\n\
It analyzes large amounts of data and presents an organization with the best possible options available.\n\
Decision support systems bring together data and knowledge from different areas and sources to provide users with information beyond the usual reports and summaries.\n\
This is intended to help people make informed decisions.\n\
Typical information a decision support application might gather and present include the following : \n\
comparative sales figures between one week and the next; \n\
projected revenue figures based on new product sales assumptions;and \n\
the consequences of different decisions.\n\
A decision support system is an informational application as opposed to an operational application.\n\
Informational applications provide users with relevant information based on a variety of data sources to support better - informed decision - making.\n\
Operational applications, by contrast, record the details of business transactions, including the data required for the decision - support needs of a business.\n\
Decision support system components\n\
A typical DSS consists of three different parts : knowledge database, softwareand user interface.\n\
Knowledge base.A knowledge base is an integral part of a decision support system database, containing information from both internaland external sources.\n\
It is a library of information related to particular subjectsand is the part of a DSS that stores information used by the system's reasoning engine to determine a course of action.\n\
Learn about knowledge base uses in customer service and call centers.\n\
Software system.The software system is composed of model management systems.\n\
A model is a simulation of a real - world system with the goal of understanding how the system works and how it can be improved.\n\
Organizations use models to predict how outcomes will change with different adjustments to the system.\n\
For example, models can be helpful for understanding systems that are too complicated, too expensive or too dangerous to fully explore in real life.\n\
That's the idea behind computer simulations used for scientific research, engineering tests, weather forecasting and many other applications.\n\
Models can also be used to represent and explore systems that don't yet exist, like a proposed new technology, a planned factory or a business's supply chain.\n\
Businesses also use models to predict the outcomes of different changes to a system -- such as policies, risksand regulations -- to help make business decisions.\n\
User interface.The user interface enables easy system navigation.\n\
The primary goal of the decision support system's user interface is to make it easy for the user to manipulate the data that is stored on it.\n\
Businesses can use the interface to evaluate the effectiveness of DSS transactions for the end users.\n\
DSS interfaces include simple windows, complex menu - driven interfaces and command - line interfaces.\n";
    U_32 input_size_lz77 = strlen(input_lz77);
    U_08* output_lz77 = (U_08*)malloc(input_size_lz77 * get_size_of_encoded_sequence_struct() * 100);
    U_32 output_size_lz77 = 0;
    lz77_encode(input_lz77, input_size_lz77, output_lz77, &output_size_lz77, 5);

    U_08* output_huffman = (U_08*)malloc((output_size_lz77 + sizeof(Huffman_metadata)) * 100);
	/*printf("0x%x\n", output_huffman);
	printf("0x%x -4\n", *(U_32*)(output_huffman - 4));
	printf("0x%x -8\n", *(U_32*)output_huffman - 8);
	printf("0x%x -12\n", *(U_32*)output_huffman - 12);
	printf("0x%x -16\n", *(U_32*)output_huffman - 16);*/
    U_32 output_size_huffman = 0;
    huffman_encode(output_lz77, output_huffman, output_size_lz77, &output_size_huffman);

	/*printf("0x%x\n", output_huffman);
	printf("0x%x -4\n", *(U_32*)(output_huffman - 4));
	printf("0x%x -8\n", *(U_32*)output_huffman - 8);
	printf("0x%x -12\n", *(U_32*)output_huffman - 12);
	printf("0x%x -16\n", *(U_32*)output_huffman - 16);*/

    U_08* output_huffman_decode = (U_08*)malloc(output_size_lz77 * 100);
    U_32 output_size_huffman_decode;
    huffman_decode(output_huffman, &output_size_huffman, output_huffman_decode, &output_size_huffman_decode);

	///*printf("0x%x\n", output_huffman);
	//printf("0x%x -4\n", *(U_32*)(output_huffman - 4));
	//printf("0x%x -8\n", *(U_32*)output_huffman - 8);
	//printf("0x%x -12\n", *(U_32*)output_huffman - 12);
	//printf("0x%x -16\n", *(U_32*)output_huffman - 16);*/
   ASSERT_EQUAL(output_size_lz77, output_size_huffman_decode, "output size of huffman decode and lz77 encode");
    U_08* result_end = (U_08*)malloc(input_size_lz77 * 100);
	U_32 size1 = 36;
    lz77_decode(output_huffman_decode, &output_size_huffman_decode, result_end);
    ASSERT(strcmp(input_lz77, result_end, input_size_lz77), "The decode result is wrong");
	//printf("0x%x\n", output_huffman);
	//printf("0x%x -4\n", *(U_32*)(output_huffman - 4));
	//printf("0x%x -8\n", *(U_32*)output_huffman - 8);
	//printf("0x%x -12\n", *(U_32*)output_huffman - 12);
	//printf("0x%x -16\n", *(U_32*)output_huffman - 16);
	free(output_lz77);

    free(output_huffman);
    free(output_huffman_decode);
    free(result_end);
}

//void lz77_encode(const U_08* input_buffer, U_32 input_size, U_08* output_buffer, U_32* output_size,
//S_32 compress_level);

//void lz77_decode(const U_08* input_data,const U_32* input_size, U_08* output_data);


	//const S_08* initial_string = "abcd efgh&ijklmn";
	//U_32 length = strlen(initial_string) + 1;  // +1 for the null terminator
	//U_08* st = (U_08*)malloc(length * sizeof(U_08));
	//memcpy(st, initial_string, length);

	//U_08* res = (U_08*)malloc(1000 * sizeof(U_08));
	//U_08* out = (U_08*)malloc(1000 * sizeof(U_08));
	////const U_08 st[30] = "abcd efgh&ijklmn";
	////const U_08 res[100];
	////const U_08 out[100];
	//int res_size;
	//int out_size;
	//lz77_encode(st, length, res, &res_size, DEFAULT_COMPRESSION_LEVEL);
	//for (int i = 0; i < res_size; i++)
	//{
	//	printf("%hi", res[i]);
	//	i += 2;
	//	printf("%hi", res[i]);
	//	i += 2;
	//	printf("%c", res[i]);
	//}
	//printf("\n");

	//printf("\n");
	//huffman_encode(res, out, res_size, &out_size);
	//printf("%s", res);
	//huffman_decode(out, &out_size, res);
	//printf("%s", res);
	//int size1 = 66;
	//lz77_decode(res, &size1, out);
	//for (int i = 0; i < length; i++)
	//{
	//	printf("%c", out[i]);
	//}
	//printf("\n");
	//free(st);
	//free(res);
	//free(out);
	//const U_08* original_file = "input.png";
	//const U_08* compressed_file = "compressed.bin";
	//const U_08* decompressed_file = "decompressed";
	//printf("compression...\n");
	//process_file(original_file, compressed_file, DEFAULT_COMPRESSION_LEVEL, COMPRESS);
	//printf("\ndecompression...\n");
	//process_file(compressed_file, decompressed_file, DEFAULT_COMPRESSION_LEVEL, UN_COMPRESS);
	////int size = 5, output_size = 0;
	////unsigned char* compressed_data = (unsigned char*)malloc(output_size * sizeof(unsigned char));;

