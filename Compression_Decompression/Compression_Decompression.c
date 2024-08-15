#include <stdio.h>
#include <string.h>
#include "lz77.h"
#include "file_managment.h"
#include "compressor_decompressor.h"


int main()
{
    const char* original_file = "input.png";
    const char* compressed_file = "compressed.bin";
    const char* decompressed_file = "decompressed";

    // Simulate compression
    printf("Simulating compression...\n");
    process_file(original_file, compressed_file, 1);

<<<<<<< HEAD
	lz77_encode(input_data, input_size, output_data, &output_size, 12, 12);

	for (int i = 0; i < output_size; i+=2)
	{
		printf("%hi", output_data[i]);
		i += 2;
		printf("%hi", output_data[i]);
		i += 2;
		printf("%c", output_data[i]);
	}
	printf("\n");
	unsigned char* output_buffer_lz_decode = (unsigned char*)malloc(6 * sizeof(char));

	lz77_decode(output_data, &output_size, output_buffer_lz_decode);
	for (int i = 0; i < strlen(input_data); i++)
	{
		printf("%c", output_buffer_lz_decode[i]);
	}

	free(output_data);
=======
    // Simulate decompression
    printf("\nSimulating decompression...\n");
    process_file(compressed_file, decompressed_file, 0);
>>>>>>> 5e74596b3d23be3d236b91c9c67957fecbee3416
}
