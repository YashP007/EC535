// file: mylist.h
// Author/Date: Yash Patel (pately@bu.edu), 09/10/25
// EC535, HW1, Question 2

#ifndef mylist
#define mylist
#include <stdint.h>

typedef struct node {
  uint32_t flipped;
  unsigned int count;
  char ascii[32]; // buffer to hold ASCII string representation of 'flipped' value
  struct node *next;
} node;

// Updated function prototypes to match implementations in mylist.c
node* createBaseNode(uint32_t, unsigned int);
void appendNode(node **, uint32_t, unsigned int);

#endif