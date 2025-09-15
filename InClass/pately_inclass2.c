// file: pately_inclass2.c
// teamates: yash patel (U11247717, pately@bu.edu), leah jones (U19167303, ldsj@bu.edu)
#include "stdio.h"
#include <stdint.h>

// Converts endianness of a 32-bit integer using 8-bit shifting
uint32_t convertEndian(uint32_t input) {
    uint32_t output = 0;

    for (int i = 0; i < 4; i++) {
        // Extract lowest 8 bits (one byte)
        uint32_t curr_byte = input & 0xFF;

        // Shift output left by 8 bits and append current byte
        output = (output << 8) | curr_byte;

        // Shift input right by 8 bits to process the next byte
        input >>= 8;
    }

    return output;
}


void main() {
  uint32_t usr_input;
	printf("Enter a integer:\n");
	scanf("%d", &usr_input);
  
  uint32_t result = convertEndian(usr_input);

  printf("0x%X\t0x%x\t%u", usr_input, result, result);
}