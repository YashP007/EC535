// file: main.c
// Author/Date: Yash Patel (pately@bu.edu), 09/09/25
// EC535, HW1, Question 1

#include "stdio.h"
#include "stdlib.h"
#include "bits.h"
#include "mylist.h"

// Testing the L-R bit flip function
//unsigned int input = 1414551882; //[5]10 --> [0101]2

int main(int numArgs, char *varArgs[]) {
	// Debug: print received args
	fprintf(stderr, "DEBUG: program started. argc=%d\n", numArgs);
	for (int i = 0; i < numArgs; ++i) {
		fprintf(stderr, "DEBUG: argv[%d]=%s\n", i, varArgs[i]);
	}

	// Assert that 3 args are provided: program-file, input-file-name, output-file-name
	if (numArgs != 3){
		printf("Not enough args provided: please run this program as such: \n\t\t ./MyBitApp inputfile.txt outputfile.txt\n\n");
		return 1; // returning 1 cause program execution failed
	}

	FILE *inputFile  = fopen(varArgs[1], "r");
	FILE *outputFile = fopen(varArgs[2], "w"); // will re-write any exisisting content in this file

	// Debug: report fopen results
	fprintf(stderr, "DEBUG: fopen inputFile=%p outputFile=%p\n", (void*)inputFile, (void*)outputFile);

	// Check that the input file exists
	if (inputFile== NULL){
		fprintf(stderr, "ERROR: The input file does not exist or cannot be opened: %s\n", varArgs[1]);
		return 1; // exit early to avoid segfaults
	}

	// Check that the output file was opened
	if (outputFile== NULL){
		fprintf(stderr, "ERROR: Could not open output file for writing: %s\n", varArgs[2]);
		fclose(inputFile);
		return 1;
	}

	// Generate base node for linked list
	node* head = NULL; // initialize to avoid undefined behavior

	unsigned int currValue;
	// Read single line (i.e. one value) in from input file
	while (fscanf(inputFile, "%u", & currValue) == 1){
		fprintf(stderr, "DEBUG: read value=%u\n", currValue);

		// use bits.c to get the fliiped input and count values. 
		unsigned int flipped_input = BinaryMirror(currValue);
		unsigned int count_of_pattern = CountSequence(currValue);
		fprintf(stderr, "DEBUG: flipped=%u count=%u\n", flipped_input, count_of_pattern);

		// If first value need to create base node
		if (head == NULL){
			fprintf(stderr, "DEBUG: creating base node\n");
			head = createBaseNode(flipped_input, count_of_pattern);
			if (head == NULL) {
				fprintf(stderr, "ERROR: createBaseNode returned NULL\n");
				fclose(inputFile);
				fclose(outputFile);
				return 1;
			}
			continue;
		} 
		// if not first value -- Append to linked list using the append function in mylist.c
		fprintf(stderr, "DEBUG: appending node\n");
		appendNode(head, flipped_input, count_of_pattern);
	}

	// Loop through linked list and print all values into output file
	node* current = head;
	fprintf(stderr, "DEBUG: writing list to output file\n");
	while (current != NULL){
		// Output file print format:		<flipped_input>		<count of pattern>
		fprintf(outputFile, "%u\t%u\n", current->flipped, current->count);
		fprintf(stderr, "DEBUG: wrote -> flipped=%u count=%u\n", current->flipped, current->count);
		current = current -> next;
  	}	

	// Free the list once done using linked list
	fprintf(stderr, "DEBUG: freeing list\n");
  	current = head;
  	while (current != NULL) {
    	node* temp = current;
    	current = current->next;
    	free(temp);
  	}

	fclose(inputFile);
	fclose(outputFile);
	fprintf(stderr, "DEBUG: program completed successfully\n");
	return 0; // ie return 0 to indicate program completion succeded. 
}

//## need to add in file reading and writing into main.c