#ifndef PREFIX_TREES
#define PREFIX_TREES

#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define PREFIXSIZE 16

struct Node{
	int next_hop;
	struct Node *child_zero;
	struct Node *child_one;
};

struct TwoBitNode {
	int next_hop;
	struct TwoBitNode *child_00;
	struct TwoBitNode *child_01;
	struct TwoBitNode *child_10;
	struct TwoBitNode *child_11;
};

struct Node* InsertPrefix(char prefix[PREFIXSIZE], int next_hop, struct Node* root);

struct Node* PrefixTree(int argc, char const *argv[]);

void PrintTable(struct Node *root, struct Node *current_node, char *binary_level, char aux[PREFIXSIZE], int *tree_level );

int LookUp(struct Node *root, struct Node *current_node, char prefix[PREFIXSIZE], int *next_hop, int *tree_level);

struct Node* DeletePrefix(struct Node *root, char prefix[PREFIXSIZE], char *binary_level, char aux[PREFIXSIZE], int *tree_level);

void FreePrefixTree(struct Node *root, struct Node *current_node);

struct TwoBitNode* BinaryToTwoBit(struct Node *root, struct TwoBitNode *root_two, int *tree_level, char prefix[PREFIXSIZE], char *binary_level);

#endif
