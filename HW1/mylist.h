// file: mylist.h
// Author/Date: Yash Patel (pately@bu.edu), 09/10/25
// EC535, HW1, Question 2

#ifndef mylist
#define mylist

typedef struct node {
  int decimal;
  unsigned int binary;
  char ascii[10];
  struct node *next;
} node;

// Updated function prototypes to match implementations in mylist.c
unsigned int decimal2binary(unsigned int input);
void decimal2ascii(unsigned int input, char* output);
node* createBaseNode(unsigned int input);
void appendNode(node* head, unsigned int input);

// main entry point
int main(int);

#endif