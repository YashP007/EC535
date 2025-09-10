// file: mylist.c
// Author/Date: Yash Patel (pately@bu.edu), 09/10/25
// EC535, HW1, Question 2

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mylist.h"

unsigned int decimal2binary(unsigned int input) {
  unsigned int result = 0;
  unsigned int scale = 1;
  if (input == 0) return 0;
  while (input > 0) {
    if (input & 1)
      result += scale;
    scale *= 10;
    input >>= 1;
  }
  return result;
}

void decimal2ascii(unsigned int input, char* output){
  sprintf(output, "%u", input);
}

node* createBaseNode(unsigned int input){
    node* newNode = (node*)malloc(sizeof(node));
    if (newNode == NULL) return NULL;
    newNode->decimal = input;
    newNode->binary = decimal2binary(input);
    decimal2ascii(input, newNode->ascii);
    newNode->next = NULL;
    return newNode;
}

void appendNode(node* head, unsigned int input){
  // to make more effient you can call this function with the most recent node you have access to instead of head to 
  // reduce the loop time for this function to get to the end of the linked list
    if (head == NULL) return;
    node* current = head;

    // loop through linked list to the end of it, if given head and long linked list this is ineffient
    while (current->next != NULL) {
        current = current->next;
    }

    // allocate new node object
    node* newNode = (node*)malloc(sizeof(node));
    if (newNode == NULL) return;

    // add in values into this node object
    newNode->decimal = input;
    newNode->binary = decimal2binary(input);
    decimal2ascii(input, newNode->ascii);
    newNode->next = NULL; // set the pointer to the next value to NULL (ie this node is at the end of the linked list)
    current->next = newNode; // set the pointer of the previous node to the location of this new one this function all created
}

int main(int numArgs){
  // create base node
  unsigned int values[] = {5, 10, 255, 0, 42};
  int n = sizeof(values)/sizeof(values[0]);

  // Create the head node
  node* head = createBaseNode(values[0]);
  if (head == NULL) {
    printf("Failed to create head node.\n");
    return 1;
  }

  // Append the rest of the nodes
  for (int i = 1; i < n; i++) {
    appendNode(head, values[i]);
  }

  // Print the list
  node* current = head;
  printf("Decimal\tBinary\t\t\t\t\tASCII\n");
  printf("-------------------------------------------------------------\n");
  while (current != NULL) {
    printf("%d\t%u\t\t%s\n", current->decimal, current->binary, current->ascii);
    current = current->next;
  }

  // Free the list
  current = head;
  while (current != NULL) {
    node* temp = current;
    current = current->next;
    free(temp);
  }

  return 0;
}