#include <string.h>
#include "test_framework.h"
#include "huffman_code.h"
#include "huffman_test.h"

//encode
void test_huffman_frequency_compare(void) {
    Huffman_node_t node1 = { .frequency = 5, .by_ascii = 'A' };
    Huffman_node_t node2 = { .frequency = 10, .by_ascii = 'B' };
    Huffman_node_t node3 = { .frequency = 5, .by_ascii = 'C' };
    ASSERT(huffman_frequency_compare(&node1, &node2) < 0, "node1 should be less than node2");
    ASSERT(huffman_frequency_compare(&node2, &node1) > 0, "node2 should be greater than node1");
    ASSERT(huffman_frequency_compare(&node1, &node3) < 0, "node1 should be less than node3 (same frequency, lower ASCII)");
}
void test_initialize_nodes(void) {
    Huffman_node_t nodes[ASCII_SIZE];
    initialize_nodes(nodes);
    for (U_32 i = 0; i < ASCII_SIZE; i++) {
        ASSERT_EQUAL(i, nodes[i].by_ascii, "Node ASCII value should match index");
        ASSERT_EQUAL(0, nodes[i].frequency, "Node frequency should be initialized to 0");
    }
}
void test_count_frequencies(void) {
    Huffman_node_t nodes[ASCII_SIZE] = { 0 };
    initialize_nodes(nodes);
    const U_08 data[] = "AABBBCCCC";
    U_32 input_size = strlen((char*)data);
    count_frequencies(data, input_size, nodes);
    ASSERT_EQUAL(2, nodes['A'].frequency, "Frequency of 'A' should be 2");
    ASSERT_EQUAL(3, nodes['B'].frequency, "Frequency of 'B' should be 3");
    ASSERT_EQUAL(4, nodes['C'].frequency, "Frequency of 'C' should be 4");
}
void test_huffman_encode(void) {
    const U_08 input[] = "AABBBCCCC";
    U_32 input_size = strlen((char*)input);
    U_08 output_buffer[1024];
    size_t output_size;
    huffman_encode(input, output_buffer, input_size, &output_size);
    ASSERT(output_size > 0, "Output size should be greater than 0");
    //ASSERT(output_size < input_size, "Compressed size should be less than input size");
}
//decode
void test_get_tree()
{
    U_08 output_buffer[100];
    size_t output_size;
    U_32 tree_length;
    Huffman_decode_node* root, * tmp_root, * nodes;

    huffman_encode("hello", output_buffer, 5, &output_size);

    root = get_tree(output_buffer, &tree_length);
    nodes = root;
    tmp_root = root + (tree_length - 1);//move the pointer to the root of the tree
    root = tmp_root;

    //check size
    ASSERT_EQUAL(tree_length, 7, "The tree length wrong");

    //check tree
    ASSERT_EQUAL(root->left, 4, "The tree is not accurate");
    ASSERT_EQUAL(root->right, 5, "The tree is not accurate");

    root = &(nodes[root->left]);
    ASSERT_EQUAL(root->left, 0, "The tree is not accurate");
    ASSERT_EQUAL(root->right, 1, "The tree is not accurate");

    root = &(nodes[root->left]);
    ASSERT_EQUAL(root->left, 512, "The tree is not accurate");
    ASSERT_EQUAL(root->right, 512, "The tree is not accurate");
    ASSERT_EQUAL(root->by_ascii, 'e', "The tree is not accurate");

    root = tmp_root;

    root = &(nodes[root->left]);
    root = &(nodes[root->right]);
    ASSERT_EQUAL(root->left, 512, "The tree is not accurate");
    ASSERT_EQUAL(root->right, 512, "The tree is not accurate");
    ASSERT_EQUAL(root->by_ascii, 'h', "The tree is not accurate");

    root = tmp_root;

    root = &(nodes[root->right]);
    ASSERT_EQUAL(root->left, 2, "The tree is not accurate");
    ASSERT_EQUAL(root->right, 3, "The tree is not accurate");

    root = &(nodes[root->right]);
    ASSERT_EQUAL(root->left, 512, "The tree is not accurate");
    ASSERT_EQUAL(root->right, 512, "The tree is not accurate");
    ASSERT_EQUAL(root->by_ascii, 'l', "The tree is not accurate");

    root = tmp_root;

    root = &(nodes[root->right]);
    root = &(nodes[root->left]);
    ASSERT_EQUAL(root->left, 512, "The tree is not accurate");
    ASSERT_EQUAL(root->right, 512, "The tree is not accurate");
    ASSERT_EQUAL(root->by_ascii, 'o', "The tree is not accurate");
}
void test_find_ascii_in_tree()
{
    U_08* output_buffer = (U_08*)malloc(100);
    size_t output_size;
    U_32 tree_length;
    Huffman_metadata* metadata;
    Huffman_decode_node* root;
    U_08 ascii;
    size_t num_bits = 0;
    huffman_encode("green", output_buffer, 5, &output_size);
    root = get_tree(output_buffer, &tree_length);
    output_buffer += (sizeof(metadata->tree_length) + sizeof(Huffman_decode_node) * tree_length);
    ascii = find_ascii_in_tree(&output_buffer, root, &num_bits, tree_length);
    ASSERT_EQUAL(ascii, 'g', "The found ascii is wrong");
    //ASSERT_EQUAL(num_bits, 1, "The num of bits is wrong");
    //free output buffer
    output_buffer -= (sizeof(metadata->tree_length) + sizeof(Huffman_decode_node) * tree_length);
    free(output_buffer);
}
void test_find_ascii_last_byte()
{
    U_08* output_buffer = (U_08*)malloc(100);
    size_t output_size;
    U_32 tree_length;
    Huffman_metadata* metadata;
    Huffman_decode_node* root;
    U_08 ascii;
    size_t num_bits = 0;
    U_08* result = (U_08*)malloc(5);
    huffman_encode("april", output_buffer, 5, &output_size);
    root = get_tree(output_buffer, &tree_length);
    output_buffer += (sizeof(metadata->tree_length) + sizeof(Huffman_decode_node) * tree_length);
    output_buffer++;
    find_ascii_last_byte(output_buffer, root, 10, result, tree_length, &output_size);
    ASSERT_EQUAL(*result, 'l', "The ascii is wrong");
    free(result);
}
//encode-decode
void test_exactly_bits_in_bytes()
{
    U_08* input = "abcd";// "ELOOLE";
    size_t input_size = 4;
    U_08* output = (U_08*)malloc(input_size + sizeof(Huffman_metadata));
    size_t output_size;
    size_t size = 0;
    U_08* result = (U_08*)malloc(input_size);
    huffman_encode(input, output, input_size, &output_size);
    huffman_decode(output, &output_size, result, &size);
    ASSERT(strcmp(input, result), "The decode result is wrong");
    ASSERT_EQUAL(input_size, size, "The output size is wrong");
    free(output);
    free(result);
}

void test_huffman_long_input()
{
    U_08* input = "hi everybody hello world !!!! hi everybody hello world !!!! \n\
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
    size_t input_size = strlen(input);
    U_08* output = (U_08*)malloc(input_size + sizeof(Huffman_metadata));
    size_t output_size;
    size_t size = 0;
    U_08* result = (U_08*)malloc(input_size);
    huffman_encode(input, output, input_size, &output_size);
    huffman_decode(output, &output_size, result, &size);
    ASSERT(strcmp(input, result), "The decode result is wrong");
    ASSERT_EQUAL(input_size, size, "The output size is wrong");
    free(output);
    free(result);

}

void test_not_exactly_bits_in_bytes()
{
    U_08* input = "april";
    size_t input_size = strlen(input);
    U_08* output = (U_08*)malloc(input_size + sizeof(Huffman_metadata));
    size_t output_size;
    size_t size = 0;
    U_08* result = (U_08*)malloc(input_size);
    huffman_encode(input, output, input_size, &output_size);
    huffman_decode(output, &output_size, result, &size);
    ASSERT(strcmp(input, result), "The decode result is wrong");
    ASSERT_EQUAL(input_size, size, "The output size is wrong");
    free(output);
    free(result);
}
