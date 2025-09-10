// file: main.c
// Author/Date: Yash Patel (pately@bu.edu), 09/09/25
// EC535, HW1, Question 1

#include "stdio.h"
#include "stdlib.h"
#include "bits.h"

// Testing the L-R bit flip function
unsigned int input = 1414551882; //[5]10 --> [0101]2

int main(int numArgs, char *varArgs[]) {
	// Assert that 3 args are provided: program-file, input-file-name, output-file-name
	if (numArgs != 3){
		printf("Not enough args provided: please run this program as such: \n\t\t ./MyBitApp inputfile.txt outputfile.txt\n\n");
		return 1; // returning 1 cause program execution failed
	}

	File *inputFile  = fopen(varArgs[1], "r");
	File *outputFile = fopen(varArgs[2], "w"); // will re-write any exisisting content in this file

	// Check that the input file exists
	if (inputFile== NULL){
		printf("The input files does not exist, please ensure this file exists before re-running\n");
	}

	u_int32_t currValue;
	while (fscanf(inputFile, "%u", & currValue) == 1){
		unsigned int flipped_input = BinaryMirror(currValue);
		unsigned int count_of_pattern = CountSequence(currValue);
		fprintf(outputFile, "%d\t%d", flipped_input, count_of_pattern);
	}
	return 0; // ie return 0 to indicate program completion succeded. 
}

//## need to add in file reading and writing into main.c