#include "prefixTrees.h"
#include "utils.h"

struct Node* InsertPrefix(char prefix[PREFIXSIZE], int next_hop, struct Node *root) {

	int n = 0; /* index of the bit on the prefix string*/
	struct Node *aux = root;

	/* for the case of the empty prefix <--- might be changed after 
	discovering what the professor wants to use as an empty prefix */
	if (strcmp(prefix, "e") == 0) {
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

		root = InsertPrefix(prefix, next_hop, root);

		memset(prefix, 0, sizeof(prefix));

	}

	if ( fclose(file) == EOF ) {
		printf("Error while closing file: %s\n", strerror(errno));
		exit(1);
	}

	return root;
}

int LookUp(struct Node *root, struct Node *current_node, char prefix[PREFIXSIZE], int *next_hop, int *tree_level) {

	if(current_node != root ){
		if(current_node->next_hop != -1) {
			(*next_hop) = current_node->next_hop;
		}
	}


	// caso em que a root tem uma empty address
	if(current_node == root && root->next_hop != -1) {
		(*next_hop) = root->next_hop;
	}


	if ((*tree_level) < strlen(prefix) ) {

		if (current_node->child_zero != NULL && prefix[*tree_level] == '0') {
			(*tree_level)++;
			LookUp(root, current_node->child_zero, prefix, next_hop, tree_level);
		} 
		else if (current_node->child_one != NULL && prefix[(*tree_level)] == '1') {
			(*tree_level)++;
			LookUp(root, current_node->child_one,prefix, next_hop, tree_level);

		}
	}

	return (*next_hop);
}


void PrintTable(struct Node *root, struct Node *current_node, char *binary_level, char aux[PREFIXSIZE], int *tree_level ) {

	if(current_node != root ){
		aux[(*tree_level)] = *binary_level;
		if(current_node->next_hop != -1) {
			printf("%s %d\n", aux, current_node->next_hop);
		}
	}

	// caso em que a root tem uma empty address
	if(current_node == root ){
		if(current_node->next_hop != -1) {
			printf("e %d\n", current_node->next_hop);
		}
	}

	if (current_node->child_zero != NULL) {
		(*tree_level)++;
		(*binary_level) = '0';
		PrintTable(root, current_node->child_zero, binary_level, aux, tree_level);
	}

	if (current_node->child_one != NULL) {
		(*tree_level)++;
		(*binary_level) = '1';

		PrintTable(root, current_node->child_one, binary_level, aux, tree_level);

	}
	
	aux[(*tree_level)] = '\0';
	(*tree_level)--;

	return;
}

struct Node* DeletePrefix(struct Node *root, char prefix[PREFIXSIZE], char *binary_level, char aux[PREFIXSIZE], int *tree_level) {

	if ((*tree_level) > 0) {
		aux[(*tree_level) - 1] = *binary_level;
	}

	if (strcmp(aux, prefix) == 0) {
		root->next_hop = -1;
		free(root);
		return NULL;
	}

	if (strcmp(prefix, aux) != 0 && root != NULL) {

		if (root->child_zero != NULL && prefix[*tree_level] == '0') {
			(*tree_level)++;
			(*binary_level) = '0';
			root->child_zero = DeletePrefix(root->child_zero, prefix, binary_level, aux, tree_level);
		} 
		else if (root->child_one != NULL && prefix[(*tree_level)] == '1') {
			(*tree_level)++;
			(*binary_level) = '1';
			root->child_one = DeletePrefix(root->child_one, prefix, binary_level, aux, tree_level);

		}
	}

	// if the node correspondent to the prefix that must be deleted is a leaf, then we free it from our tree
	if(root->child_zero == NULL && root->child_one == NULL && root->next_hop == -1){
		free(root);
		return NULL;
	}

	return root;
}


void FreePrefixTree(struct Node *root, struct Node *current_node) {
	
	if (current_node->child_zero != NULL) {
		FreePrefixTree(root, current_node->child_zero);
	}

	if (current_node->child_one != NULL) {
		FreePrefixTree(root, current_node->child_one);
	}

	free(current_node);
	
	return;
};



// struct TwoBitNode* BinaryToTwoBit(struct Node *root, struct Node *current_node, struct TwoBitNode *root_two, int *tree_level, char prefix[PREFIXSIZE], char *binary_level) {

// 	if (current_node == root) {
// 		// função que aloca TwoBitNode, insere na TwoBit PrefixTree e retorna a root dela
// 	}
// 	if ((*tree_level) == odd){
// 		if(current_node->child_zero != NULL) {

// 				if(current_node->next_hop != -1) {

// 					if(current_node->child_zero != NULL){
// 						if (current_node->child_zero->next_hop == -1) {
// 							junta um '0' ao prefixo
// 							ultimo_next_hop = current_node->next_hop
// 							cria um no com next hop=ultimo_next_hop
// 							junta esse nó ao 2bittree
// 						} else {
// 							junta um '0' ao prefixo
// 							cria um no com next hop=current_node->child_zero->next_hop
// 							junta esse nó ao 2bittree
// 						}
// 					} else {
// 						junta um '0' ao prefixo
// 						ultimo_next_hop = current_node->next_hop
// 						cria um no com next hop=ultimo_next_hop
// 						junta esse nó ao 2bittree
// 					}

// 					if(current_node->child_one != NULL){
// 						if (current_node->child_one->next_hop == -1) {
// 							junta um '1' ao prefixo
// 							ultimo_next_hop = current_node->next_hop
// 							cria um no com next hop=ultimo_next_hop
// 							junta esse nó ao 2bittree
// 						} else {
// 							junta um '1' ao prefixo
// 							cria um no com next hop=current_node->child_one->next_hop
// 							junta esse nó ao 2bittree
// 						}
// 					} else {
// 						junta um '1' ao prefixo
// 						ultimo_next_hop = current_node->next_hop
// 						cria um no com next hop=ultimo_next_hop
// 						junta esse nó ao 2bittree
// 					}

// 				} else {
// 					if(current_node->child_zero != NULL){
// 						if (current_node->child_zero->next_hop == -1) {
// 							junta um '0' ao prefixo
// 							cria um no com next hop=ultimo_next_hop
// 							junta esse nó ao 2bittree
// 						} else {
// 							junta um '0' ao prefixo
// 							cria um no com next hop=current_node->child_zero->next_hop
// 							junta esse nó ao 2bittree
// 						}
// 					}

// 					if(current_node->child_one != NULL){ 
// 						if (current_node->child_one->next_hop == -1) {
// 							junta um '1' ao prefixo
// 							cria um no com next hop=ultimo_next_hop
// 							junta esse nó ao 2bittree
// 						} else {
// 							junta um '1' ao prefixo
// 							cria um no com next hop=current_node->child_one->next_hop
// 							junta esse nó ao 2bittree
// 						}
// 					} 
// 				}

// 		} else if(current_node->child_one != NULL) {

// 			// meter o que está acima para o child_one
// 		}
// 	}

// 	BinaryToTwoBit(root, current_node->child_zero, root_two, tree_level, char prefix[PREFIXSIZE], char *binary_level);
// 	BinaryToTwoBit(root, current_node->child_one, root_two, tree_level, char prefix[PREFIXSIZE], char *binary_level);

// }