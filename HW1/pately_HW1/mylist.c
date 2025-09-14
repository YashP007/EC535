// file: mylist.c
// Author/Date: Yash Patel (pately@bu.edu), 09/10/25
// EC535, HW1, Question 2

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "mylist.h"

// INTERNAL -- Helper to convert uint32_t to decimal ASCII string
static void _getascii(uint32_t, char*, size_t);


// Write decimal ASCII for 'input' into buf, NUL-terminated.
static void _getascii(uint32_t input, char *buf, size_t bufsize) {
    // Max for uint32_t in decimal is "4294967295" (10 chars) + NUL
    if (bufsize == 0) return;
    snprintf(buf, bufsize, "%u", input);
}

node* createBaseNode(uint32_t flip_val, unsigned int count_val) {
    node *newNode = (node*)malloc(sizeof(node));
    if (!newNode) return NULL;

    newNode->flipped = flip_val;
    newNode->count   = count_val;
    _getascii(flip_val, newNode->ascii, sizeof newNode->ascii);
    newNode->next    = NULL;
    return newNode;
}

// Insert by lexicographic order of ascii (ascending).
void appendNode(node **head, uint32_t flip_val, unsigned int count_val) {
    if (!head) return;

    node *n = (node*)malloc(sizeof(node));
    if (!n) return;

    n->flipped = flip_val;
    n->count   = count_val;
    _getascii(flip_val, n->ascii, sizeof n->ascii);

    n->next    = NULL;

    // Insert at head if list empty or new key sorts before current head
    if (*head == NULL || strcmp(n->ascii, (*head)->ascii) < 0) {
        n->next = *head;
        *head = n;
        return;
    }

    // Walk to insertion point
    node *cur = *head;
    while (cur->next && strcmp(cur->next->ascii, n->ascii) <= 0) {
        cur = cur->next;
    }

    // Insert after 'cur'
    n->next = cur->next;
    cur->next = n;
}
