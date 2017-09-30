#ifndef PREFIX_TREES
#define PREFIX_TREES

#include <errno.h>
#include <stdio.h>
#include <string.h>

#define PREFIXSIZE 16

struct Node{
	int next_hop;
	struct Node *child_zero;
	struct Node *child_one;
};

struct Node* InsertTree(char prefix[PREFIXSIZE], int next_hop, struct Node* root);

struct Node* PrefixTree(int argc, char const *argv[]);

void PrintTable(struct Node *node);

#endif