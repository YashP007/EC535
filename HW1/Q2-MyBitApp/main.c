// file: Q2-MyBitApp/main.c
// Author/Date: Yash Patel, 09/09/25

#include "stdio.h"
#include "bits.h"

// Testing the L-R bit flip function
unsigned int input = 5; //[5]10 --> [0101]2

int main() {
	unsigned int flipped_input = BinaryMirror(input);
  unsigned int count_of_pattern = CountSequence(input);
	return 0;
}

## need to add in file reading and writing into main.c