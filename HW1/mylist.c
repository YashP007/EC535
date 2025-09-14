// file: mylist.c
// Author/Date: Yash Patel (pately@bu.edu), 09/10/25
// EC535, HW1, Question 2

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mylist.h"

/* This function returns the first charectar of input number (ASCII)*/
char getleadingascii(unsigned int input){
    char buf[32];
    // convert number to decimal ASCII and return the first character
    // sprintf always NUL-terminates for these small buffers and unsigned ints
    sprintf(buf, "%u", input);
    return buf[0]; // for input==0 this returns '0'
}

node* createBaseNode(unsigned int flip_val, unsigned int count_val){
    node* newNode = (node*)malloc(sizeof(node));
    if (newNode == NULL) return NULL;
    newNode->flipped = flip_val;
    newNode->count = count_val;
    newNode->leadingascii = getleadingascii(flip_val);
    newNode->next = NULL;
    return newNode;
}

/* This function appends the inputs to the given linked list in lexigrapghic order using O(n) sorting*/
void appendNode(node* head, unsigned int flip_val, unsigned int count_val){
  // to make more effient you can call this function with the most recent node you have access to instead of head to 
  // reduce the loop time for this function to get to the end of the linked list
    if (head == NULL) return;
    node* current = head;
    node* prev = NULL; // initialize to detect head-insert

    // Get leading char value to know where to append this node to
    char leadingascii_val = getleadingascii(flip_val);

    // advance while current leadingascii is less than the new value
    while (current != NULL && current->leadingascii < leadingascii_val) {
        prev = current;
        current = current->next;
    }

    // allocate new node object
    node* newNode = (node*)malloc(sizeof(node));
    if (newNode == NULL) return;

    // add in values into this node object
    newNode->flipped = flip_val;
    newNode->count = count_val;
    newNode->leadingascii = leadingascii_val;
    
    // If inserting before the head, preserve caller's head pointer by moving old head into newNode
    if (prev == NULL) {
        // copy old head into newNode
        newNode->flipped = head->flipped;
        newNode->count = head->count;
        newNode->leadingascii = head->leadingascii;
        newNode->next = head->next;
        // put new data into head
        head->flipped = flip_val;
        head->count = count_val;
        head->leadingascii = leadingascii_val;
        head->next = newNode;
        return;
    }

    // General insert between prev and current
    prev->next = newNode;
    newNode->next = current; // works whether current is NULL (append at end) or not
    return;
  }