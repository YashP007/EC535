// file: mylist.h
// Author/Date: Yash Patel (pately@bu.edu), 09/10/25
// EC535, HW1, Question 2

#ifndef mylist
#define mylist

typedef struct node {
  unsigned int flipped;
  unsigned int count;
  char leadingascii;  
  struct node *next;
} node;

// Updated function prototypes to match implementations in mylist.c
char getleadingascii(unsigned int);
node* createBaseNode(unsigned int, unsigned int);
void appendNode(node*, unsigned int, unsigned int);

#endif