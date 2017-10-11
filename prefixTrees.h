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

int LookUp(struct Node *current_node, char prefix[PREFIXSIZE], int *next_hop, int *tree_level);

struct Node* DeletePrefix(struct Node *root, char prefix[PREFIXSIZE], char *binary_level, char aux[PREFIXSIZE], int *tree_level);

struct Node * FreePrefixTree(struct Node *root);

struct TwoBitNode* BinaryToTwoBit(struct Node *root, struct Node *current_node, struct TwoBitNode *root_two, 
									int *tree_level, char prefix[PREFIXSIZE], char *binary_level, int *relevant_hop);
// Function that inserts a new node at two bit prefix tree
struct TwoBitNode* InsertTwoBit(struct TwoBitNode *root_two, char prefix[PREFIXSIZE], int next_hop);

void PrintTableEven(struct TwoBitNode *root, struct TwoBitNode *current_node, char aux[PREFIXSIZE], int *n );

struct TwoBitNode * FreeTwoBitPrefixTree(struct TwoBitNode *root_two);

#endif
