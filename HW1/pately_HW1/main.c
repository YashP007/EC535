// file: main.c
// Author/Date: Yash Patel (pately@bu.edu), 09/09/25
// EC535, HW1, Question 1

#include "stdio.h"
#include "stdlib.h"
#include "bits.h"
#include "mylist.h"
#include <inttypes.h> // For PRIu32

// Function Definitions for Functions in this File 
void _printBin(uint32_t); // FOR DEBUG PURPOSES ONLY

// Testing the L-R bit flip function
//unsigned int input = 1414551882; //[5]10 --> [0101]2

unsigned int debug_mode = 0; 

int main(int numArgs, char *varArgs[]) {
	if (debug_mode == 1){for (int i = 0; i < numArgs; ++i) {fprintf(stderr, "DEBUG: argv[%d]=%s\n", i, varArgs[i]);}}

	// Assert that 3 args are provided: program-file, input-file-name, output-file-name
	if (numArgs != 3){
		printf("Not enough args provided: please run this program as such: \n\t\t ./MyBitApp inputfile.txt outputfile.txt\n\n");
		return 1; // returning 1 cause program execution failed
	}

	FILE *inputFile  = fopen(varArgs[1], "r");
	FILE *outputFile = fopen(varArgs[2], "w"); // will re-write any exisisting content in this file

	// Check that the input file exists
	if (inputFile== NULL){
		fprintf(stderr, "ERROR: The input file does not exist or cannot be opened: %s\n", varArgs[1]);
		return 1; // exit early to avoid segfaults
	}

	// Check that the output file was opened
	if (outputFile== NULL){
		if (debug_mode == 1)
			fprintf(stderr, "ERROR: Could not open output file for writing: %s\n", varArgs[2]);
		fclose(inputFile);
		return 1;
	}

	// Generate base node for linked list
	node* head = NULL; // initialize to avoid undefined behavior

	uint32_t  currValue;
	// Read single line (i.e. one value) in from input file
	while (fscanf(inputFile, "%u", & currValue) == 1){
		// use bits.c to get the fliiped input and count values. 
		uint32_t flipped_input = BinaryMirror(currValue);
		unsigned int count_of_pattern = CountSequence(currValue);
		if (debug_mode==1){
			fprintf(stderr, "\nDEBUG\n");
			fprintf(stderr,"INPUT: \t"); _printBin(currValue); fprintf(stderr, "\t %"PRIu32"\n", currValue);
			fprintf(stderr,"FLIPD: \t"); _printBin(flipped_input); fprintf(stderr, "\t %"PRIu32"\n", flipped_input);
			fprintf(stderr,"COUNT: \t"); fprintf(stderr, "%d\n", count_of_pattern);
		}
		// If first value need to create base node
		if (head == NULL){
			head = createBaseNode(flipped_input, count_of_pattern);
			if (head == NULL) {
				fclose(inputFile);
				fclose(outputFile);
				return 1;
			}
			continue;
		} 
		// if not first value -- Append to linked list using the append function in mylist.c
		appendNode(&head, flipped_input, count_of_pattern);
	}

	if (debug_mode==1){
		fprintf(stderr, "\nReading in Values Complete, now printing values into output file:\n\n");
	}

	// Loop through linked list and print all values into output file
	node* current = head;
	
	while (current != NULL){
		// Output file print format:		<flipped_input>		<count of pattern>
		fprintf(outputFile, "%u\t%u\n", current->flipped, current->count);
		if (debug_mode==1)
			fprintf(stderr, "DEBUG: wrote -> flipped=%u \t count=%u\n", current->flipped, current->count);
		current = current -> next;
  	}	

	// Free the list once done using linked list
  	current = head;
  	while (current != NULL) {
    	node* temp = current;
    	current = current->next;
    	free(temp);
  	}

	fclose(inputFile);
	fclose(outputFile);
	return 0; // ie return 0 to indicate program completion succeded. 
}

void _printBin(uint32_t input) {
    const int nbits = 32;  // explicitly 32 bits for uint32_t

    for (int i = nbits - 1; i >= 0; --i) {
        putchar('0' + ((input >> i) & 1U));

        // Add underscore after every 4 bits, except at the very end
        if (i % 4 == 0 && i != 0) {
            putchar('_');
        }
    }
    putchar('\n');
}
