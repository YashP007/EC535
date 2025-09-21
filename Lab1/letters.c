// file: letters.c
// Yash Patel (pately@bu.edu)

#include <stdio.h>

// define starting and ending letters
#define START_LETTER 65 // 'A'
#define END_LETTER 90 // 'Z'

int main() {
    // Loop through ASCII values from 'A' to 'Z'
    for (char letter = START_LETTER; letter <= END_LETTER; letter++) {
        // Print the letter followed by a space
        putchar(letter);
    }
    // Print a newline at the end
    putchar('\n');
    return 0;
}