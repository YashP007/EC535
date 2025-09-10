// file: main.c
// Author/Date: Yash Patel (pately@bu.edu), 09/09/25
// EC535, HW1, Question 1

#include "stdio.h"
#include "bits.h"

// Testing the L-R bit flip function
unsigned int input = 1414551882; //[5]10 --> [0101]2

int main() {
	unsigned int flipped_input = BinaryMirror(input);
  unsigned int count_of_pattern = CountSequence(input);
	printf("Input:   %u\n", input);
	printf("Flipped: %u\n", flipped_input);
	printf("Counts:  %u\n", count_of_pattern);
	return 0;
}

//## need to add in file reading and writing into main.c