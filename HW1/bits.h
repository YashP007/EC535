// file: bits.h
// Author/Date: Yash Patel (pately@bu.edu), 09/09/25
// EC535, HW1, Question 1

#ifndef BITS_H
#define BITS_H
#include <stdint.h>

// Flips binary mirror of input numbers
uint32_t BinaryMirror(uint32_t);

// Counts the number of times the pattern '010' appears in the binary representation of the input number
unsigned int CountSequence(uint32_t);

#endif