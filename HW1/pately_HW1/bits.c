// file: bits.c
// Author/Date: Yash Patel (pately@bu.edu), 09/09/25
// EC535, HW1, Question 1

#include "stdio.h"
#include "stdint.h"

// Flips binary mirror of input numbers
uint32_t BinaryMirror(uint32_t input){
  uint32_t output = 0;

  for (int i = 0; i < 32; i++) {
    // 1. Extract the least significant bit value
    // 2. Invert this LSB and then add it to the output value
    // 3. Shift the output to the right (this is the thing that actually flips it L-R)
    unsigned int curr_LSB = input & 0b1;

    output = (output << 1) | (input & 1U);  // Append current LSB to output

    // Print out values for debugging and development (commented out for final submission)
    //printf("input %u\tcurr_LSB %u\toutput %u\n", input, curr_LSB, output);
    input = input >> 1; // shift input over (ie iterate through input number bit-by-bit)
  }
  return output;
}

// Counts the number of times the pattern '010' appears in the binary representation of the input number
unsigned int CountSequence(uint32_t input){
  unsigned int code = 0b010; // Looking to count number of 0b010 == [2]10 sequences
  unsigned int sum = 0;
  
  // 1. Get Mask of least significant 3 bits of input
  // 2. Check if it matches the code of interest, add to sum running int
  // 3. Shift input rightward by 1 
  while (input > 0){
    unsigned int masked = input & 0b111;
    sum = sum + (masked == code);
    
    // Print out values for debugging and development (commented out for final submission)
    //printf("input: %u\t masked: %u\t is_pattern: %u \tsum: %u\n", input, masked, (masked == code), sum);

    input = input >> 1;
  }
  return sum;
}