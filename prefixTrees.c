#include "prefixTrees.h"
#include "utils.h"

struct Node* InsertTree(char prefix[PREFIXSIZE], int next_hop, struct Node *root) {

	int n = 0; /* index of the bit on the prefix string*/
	struct Node *aux = root;

	/* for the case of the empty prefix <--- might be changed after 
	discovering what the professor wants to use as an empty prefix */
	if (strcmp(prefix, "-1") == 0) {
		root->next_hop = next_hop;
		return root;
	}

	for (n = 0; n < strlen(prefix); n++ ) {

		if (prefix[n] == '0') {
			if (aux->child_zero != NULL) {
				aux = aux->child_zero;
			} else {
				aux->child_zero = (struct Node*) mymalloc( sizeof(struct Node));
				aux = aux->child_zero;
				aux->next_hop = -1;
				aux->child_zero = NULL;
				aux->child_one = NULL;
			}

		} else if (prefix[n] == '1') {
			if (aux->child_one != NULL) {
				aux = aux->child_one;
			} else {
				aux->child_one = (struct Node*) mymalloc( sizeof(struct Node));
				aux = aux->child_one;
				aux->next_hop = -1;
				aux->child_zero = NULL;
				aux->child_one = NULL;
			}

		}
	}

	aux->next_hop = next_hop;

	return root;
}


struct Node *PrefixTree(int argc, char const *argv[]) {
	
	FILE *file = NULL;
	char temp[40];

	/* temporary variables */
	char prefix[PREFIXSIZE];
	int next_hop = 0;

	struct Node *root = (struct Node*) mymalloc( sizeof(struct Node));
	root->next_hop = -1;
	root->child_one = NULL;
	root->child_zero = NULL;

	/* opening the file, if the user gives a file on command line, it's used, 
	otherwise it uses the PrefixTable.txt file*/
	if (argc == 1) {
		if ( (file = fopen("PrefixTable.txt", "r")) == NULL ) {
			printf("Error while reading file: %s\n", strerror(errno));
			exit(1);
		}
	} else {
		if ( (file = fopen(argv[1], "r")) == NULL ) {
			printf("Error while reading file: %s\n", strerror(errno));
			exit(1);
		}
	}

	while (!feof(file)) {
		fgets(temp, sizeof(temp), file);
		sscanf(temp, "%s %d", prefix, &next_hop);

		root = InsertTree(prefix, next_hop, root);

		memset(prefix, 0, sizeof(prefix));

	}

	if ( fclose(file) == EOF ) {
		printf("Error while closing file: %s\n", strerror(errno));
		exit(1);
	}

	return root;
}


void PrintTable(struct Node *node) {

	if( node != NULL) {
		if(node->next_hop != -1){
			printf("%d", node->next_hop);
		} else {
			printf("x");
		}
		PrintTable(node->child_zero);
		PrintTable(node->child_one);
	}

}