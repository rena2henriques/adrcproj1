#include "prefixTrees.h"
#include "utils.h"


/**
 * Descrição da função:
 * A função recebe como argumentos, um prefixo, o seu correspondente next-hop, e a raiz da PrefixTree
 * Analisa o prefixo, caractere a caractere, avançando na árvore pelo child zero ou one, caso seja 1 ou 0
 * Avança-se até chegar ao ultimo caractere do prefixo, caso não existam os vértices intermédios necessários, eles são criados
 * com next hop igual a -1. **/

struct Node* InsertPrefix(char prefix[PREFIXSIZE], int next_hop, struct Node *root) {

	int n = 0; /* index of the bit on the prefix string*/
	struct Node *aux = root;

	/* for the case of the empty prefix */
	if (strcmp(prefix, "e") == 0) {
		root->next_hop = next_hop;
		return root;
	}

	for (n = 0; n < strlen(prefix); n++ ) {

		if (prefix[n] == '0') {
			if (aux->child_zero != NULL) {
				//there's already a node that allows to advance
				aux = aux->child_zero;
			} else {
				//creates the necessary node to get to the end of the prefix
				aux->child_zero = (struct Node*) mymalloc( sizeof(struct Node));
				aux = aux->child_zero;
				aux->next_hop = -1;
				aux->child_zero = NULL;
				aux->child_one = NULL;
			}

		} else if (prefix[n] == '1') {
			if (aux->child_one != NULL) {
				//there's already a node that allows to advance
				aux = aux->child_one;
			} else {
				//creates the necessary node to get to the end of the prefix
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

/**Descrição da função:
 * Função que lê a informação de um ficheiro linha a linha, e atualiza a árvore.**/

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

/**
 * Descrição da função:
 * Recebe-se o prefixo para o qual se quer encontrar o next hop
 * Compara-se caractere a caractere e avança-se pelo child zero ou one conforme seja 0 ou 1, até total matching ou uma folha
 * Se for uma folha e o prefixo ainda não acabou, significa que essa folha tem o next hop mais especifico**/

int LookUp(struct Node *current_node, char prefix[PREFIXSIZE], int *next_hop, int *tree_level) {
	
	//se for um next-hop relevante guarda-o, até ao próximo filho com next-hop relevante (mais especifico)
	if(current_node->next_hop != -1)
		(*next_hop) = current_node->next_hop;
		

	//a cada chamada compara-se cada caracter do prefixo, para se decidir se se avança pelo child one ou zero
	//a cada chamada avança-se uma posição no prefixo até se chegar ao fim
	if ((*tree_level) < strlen(prefix) ) {

		if (current_node->child_zero != NULL && prefix[*tree_level] == '0') {
			(*tree_level)++;
			LookUp(current_node->child_zero, prefix, next_hop, tree_level);
		} 
		else if (current_node->child_one != NULL && prefix[(*tree_level)] == '1') {
			(*tree_level)++;
			LookUp(current_node->child_one,prefix, next_hop, tree_level);

		}
	}

	return (*next_hop);
}

/**
 * Descrição da função:
 * Percorre todos os vértices da árovre e imprime o prefixo correspondente e o seu next hop (caso seja diferente de -1)
 * A string aux mantém o prefixo total correspondente ao vértice em que a função se encontra**/

void PrintTable(struct Node *root, struct Node *current_node, char *binary_level, char aux[PREFIXSIZE], int *tree_level ) {

	//prints the info if next hop isnt -1
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
		//the info is printed upon entering the function again
		PrintTable(root, current_node->child_zero, binary_level, aux, tree_level);
	}

	if (current_node->child_one != NULL) {
		(*tree_level)++;
		(*binary_level) = '1';
		//the info is printed upon entering the function again
		PrintTable(root, current_node->child_one, binary_level, aux, tree_level);
	}
	
	//when we get here, the node is a leaf, the function will return clears the last bit aux string 
	aux[(*tree_level)] = '\0';
	(*tree_level)--;

	return;
}

/**
 * Descrição da função:
 * 
 * Caso o prefixo que se deseja apagar não seja uma folha, basta mudar o next hop do vertice correspondente para -1, o restante
 * programa interpretará esse vértice como irrelevante, ou seja, faz apenas parte de um caminho para um vértice relevante
 * 
 * Caso seja uma folha, esse vértice é libertado da memória, e caso existam, os seus ascendentes irrelevante (com next_hop igual a -1)
 * são libertado até ao seu primeiro ascendente relevante**/
 
struct Node* DeletePrefix(struct Node *root, char prefix[PREFIXSIZE], char *binary_level, char aux[PREFIXSIZE], int *tree_level) {

	if ((*tree_level) > 0) {
		aux[(*tree_level) - 1] = *binary_level;
	}

	// caso em que a root tem o empty prefix
	if (strcmp(prefix, "e") == 0 && *tree_level == 0) {
		root->next_hop = -1;
	}

	//quando se chega ao vertice a apagar, muda-se o next_hop para -1
	if (strcmp(aux, prefix) == 0) {
		root->next_hop = -1;
	}
	
	//comparar o caractere do prefixo que se quer apagar, para se decidir a proxima chamada da função é pelo child zero ou one
	//compara-se a posição do prefixo corresponde ao nivel da arvore em que o nó se encontra
	//a variavel aux guarda o prefixo total que se percorreu até agora
	if (strcmp(prefix, aux) != 0 && root != NULL && strcmp(prefix, "e") != 0) {

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

/**Descrição da função:
 * Liberta toda a memória de forma recursiva, em profundidade primeiro **/
struct Node * FreePrefixTree(struct Node *root) {
	
	if (root == NULL){
		return root;
	}

	if (root->child_zero != NULL) {
		FreePrefixTree(root->child_zero);
	}

	if (root->child_one != NULL) {
		FreePrefixTree(root->child_one);
	}
	
	free(root);
	root = NULL;

	return root;
};

/**
 * Descrição da função
 * 
 * Percorre-se toda a prefix tree, se o nó estiver num nivel impar processa-se a informação do nó e dos seus filhos(caso existam)
 * se o nó estiver num nivel par, a sua informação ja foi processada, já que obrigatoriamente o seu pai estava num nivel impar que ja foi processado
 * avança-se para o proximo nó sem qualquer processamento
 * 
 * Perante um nó (num nivel impar) existem duas possibilidades
 * (1): o next_hop não é '-1', logo juntar 0 ou 1 para tornar o prefixo par, tratar a informação obrigatoriamente para os dois casos (para 0 e para 1), 
 * 		mesmo que os filhos não existam, já que para TwoBitTree essa informação tem de estar presente, verficar os filhos desse nó, em que podem exisitir as seguintes situações:
 * 			(1-a) Caso tenha filhos e os seus next hops não sejam -1, esse next_hop 'ganha' ao next_hop do pai (nó atual do nivel impar), porque é mais especifico, juntar à TwoBitTree
 * 			(1-b) Caso não tenha filhos, ou se tiver e os seus next_hop são -1, o next_hop do pai 'ganha', já que não há mais informação, juntar à TwoBitTree
 *
 * (2): o next_hop do nó atual é '-1', este nó nunca poderá ser uma folha, juntar 0 ou 1, para tornar o prefixo par, tratar apenas a informação dos filhos que existam,
 * 		pode suceder umas das seguintes situações
 * 			(2-a) O(s) filho(s) têm next_hop diferente de -1, colocar na twoBitTree com o next_hop do filho
 * 			(2-b) O(s) filho(s) têm next_hop igual a -1, não colocar nada na twoBitTree, haverá informação relevante num nivel posterior da arvore
**/

struct TwoBitNode* BinaryToTwoBit(struct Node *root, struct TwoBitNode *root_two, int *tree_level, char prefix[PREFIXSIZE]) {

	char aux[PREFIXSIZE] = "";
	
	strcpy(aux, prefix);
	
 	if ( (*tree_level) == 0 ) {
 		//cria a raiz da TwoBitTree
 		root_two = InsertTwoBit(root_two, aux, root->next_hop);
 	}
 	// caso seja um nivel impar mas que não seja a raiz
 	else if ((*tree_level) % 2){

		if(root->next_hop != -1) {		/**Descrição (1)**/

 			if(root->child_zero != NULL){

				aux[(*tree_level)] = '0'; //junta um '0' ao prefixo

 				if (root->child_zero->next_hop == -1) { 	/**Descrição (1-b)**/
 					InsertTwoBit(root_two, aux, root->next_hop);
 				} else {											/**Descrição (1-a)**/
 					InsertTwoBit(root_two, aux, root->child_zero->next_hop); 
 				}

 			} else {	/**Descrição (1-b)**/
				aux[(*tree_level)] = '0'; 
				InsertTwoBit(root_two, aux, root->next_hop); 
			}
					
			if(root->child_one != NULL) {

				aux[(*tree_level)] = '1'; //junta um '1' ao prefixo

				if (root->child_one->next_hop == -1) {	/**Descrição (1-b)**/
 					InsertTwoBit(root_two, aux, root->next_hop);
 				} else {										/**Descrição (1-a)**/
 					InsertTwoBit(root_two, aux, root->child_one->next_hop);
 				}

 			} else {	/**Descrição (1-b)**/
				aux[(*tree_level)] = '1'; 
 				InsertTwoBit(root_two, aux, root->next_hop);
 			}
 			
 		} else { /**next hop == -1, Descrição (2)**/
				
 			if(root->child_zero != NULL){	//Se for NULL não se faz nada

 				if (root->child_zero->next_hop == -1) {	/**Descrição (2-b)**/
 					aux[(*tree_level)] = '0'; //junta um '0' ao prefixo
 					InsertTwoBit(root_two, aux, -1);
				} else {										/**Descrição (2-a)**/
					aux[(*tree_level)] = '0'; //junta um '1' ao prefixo
					InsertTwoBit(root_two, aux, root->child_zero->next_hop);
				}
			}

			if(root->child_one != NULL){ 	//se for NULL não se faz nada
				if (root->child_one->next_hop == -1) {	/**Descrição (2-b)**/
					aux[(*tree_level)] = '1'; //junta um '1' ao prefixo
					InsertTwoBit(root_two, aux, -1); 
				} else {										/**Descrição (2-a)**/
					aux[(*tree_level)] = '1'; //junta um '1' ao prefixo
					InsertTwoBit(root_two, aux, root->child_one->next_hop); 
				}
			}
 		}
 	}
	
	if (root->child_zero != NULL) {
		prefix[(*tree_level)] = '0';
		(*tree_level)++;
		root_two = BinaryToTwoBit(root->child_zero, root_two, tree_level, prefix);
	}
	
	if (root->child_one != NULL) {
		prefix[(*tree_level)] = '1';
		(*tree_level)++;
		root_two = BinaryToTwoBit(root->child_one, root_two, tree_level, prefix);
	}
	
	prefix[(*tree_level)] = '\0';
	(*tree_level)--;
	
	return root_two;
}

/**
 * Descrição da função:
 * Funciona de forma identica à InsertPrefix
 * Analisa o prefixo, dois em dois caracteres, avançando consoante a comparação
 * Avança-se até chegar ao ultimo caractere do prefixo, caso não existam os vértices intermédios necessários, eles são criados
 * com next hop igual a -1. **/

struct TwoBitNode* InsertTwoBit(struct TwoBitNode *root_two, char prefix[PREFIXSIZE], int next_hop) {

	int n = 0; /* index of the prefix string*/
	struct TwoBitNode *aux = NULL;

	if (root_two == NULL) {
		root_two = (struct TwoBitNode*) mymalloc( sizeof(struct TwoBitNode));
		// initialize the root node
		root_two->next_hop = next_hop;
		root_two->child_00 = NULL;
		root_two->child_01 = NULL;
		root_two->child_10 = NULL;
		root_two->child_11 = NULL;
		return root_two; //1ª vez
	}

	/* for the case of the empty prefix */
	if (strcmp(prefix, "e") == 0) {
		root_two->next_hop = next_hop;
		return root_two;
	}

	// using aux to avoid losing the pointer to root_two
	aux = root_two;

	/* the prefix is evaluated 2 by 2 and n must always be odd, that way we 
	can always do n -1 without returning errors */
	for (n = 1; n < strlen(prefix); n = n + 2) {

		if (prefix[n-1] == '0' && prefix[n] == '0') {
			// if the child exists we move to it, if it doesnt, then we create it
			if (aux->child_00 != NULL) {
				aux=aux->child_00;
			} else {
				aux->child_00 = (struct TwoBitNode*) mymalloc( sizeof(struct TwoBitNode));
				aux = aux->child_00;
				// initializes the new node
				aux->next_hop = next_hop;
				aux->child_00 = NULL;
				aux->child_01 = NULL;
				aux->child_10 = NULL;
				aux->child_11 = NULL;
			} 

		} else if (prefix[n-1] == '0' && prefix[n] == '1') {
			if (aux->child_01 != NULL) {
				aux = aux->child_01;
			} else {
				aux->child_01 = (struct TwoBitNode*) mymalloc( sizeof(struct TwoBitNode));
				aux = aux->child_01;
				// initializes the new node
				aux->next_hop = next_hop;
				aux->child_00 = NULL;
				aux->child_01 = NULL;
				aux->child_10 = NULL;
				aux->child_11 = NULL;
			} 
			
		} else if (prefix[n-1] == '1' && prefix[n] == '0') {
			if (aux->child_10 != NULL) {
				aux = aux->child_10;
			} else {
				aux->child_10 = (struct TwoBitNode*) mymalloc( sizeof(struct TwoBitNode));
				aux = aux->child_10;
				// initializes the new node
				aux->next_hop = next_hop;
				aux->child_00 = NULL;
				aux->child_01 = NULL;
				aux->child_10 = NULL;
				aux->child_11 = NULL;
			} 
			
		} else if (prefix[n-1] == '1' && prefix[n] == '1') {
			if (aux->child_11 != NULL) {
				aux = aux->child_11;
			} else {
				aux->child_11 = (struct TwoBitNode*) mymalloc( sizeof(struct TwoBitNode));
				aux = aux->child_11;
				// initializes the new node
				aux->next_hop = next_hop;
				aux->child_00 = NULL;
				aux->child_01 = NULL;
				aux->child_10 = NULL;
				aux->child_11 = NULL;
			} 
			
		}

	}

	aux->next_hop = next_hop;

	return root_two;
}

/**Descrição da função:
 * Funciona de forma idêntica à função PrintTable, neste caso a string auxiliar que regista o prefixo 
 * percorrido é iterada de dois em dois bits. **/

void PrintTableEven(struct TwoBitNode *root, char aux[PREFIXSIZE], int *n ) {
	// n is used to iterate through aux string
	// always prints if the next_hop of the node is different than -1
	if(root->next_hop != -1) {
		if ( (*n) == -1 ) {
			// case that the root corresponds to the empty address
			printf("e %d\n", root->next_hop);
		} else {
			printf("%s %d\n", aux, root->next_hop);
		}
	}

	if (root->child_00 != NULL) {


		// apends 00 to aux string
		(*n)++;
		aux[(*n)] = '0';
		(*n)++;
		aux[(*n)] = '0';


		PrintTableEven(root->child_00, aux, n);
	}

	if (root->child_01 != NULL) {

		// apends 01 to aux string
		(*n)++;
		aux[(*n)] = '0';
		(*n)++;
		aux[(*n)] = '1';

		PrintTableEven(root->child_01, aux, n);
	}
	
	if (root->child_10 != NULL) {


		// appends 10 to aux string
		(*n)++;
		aux[(*n)] = '1';
		(*n)++;
		aux[(*n)] = '0';

		PrintTableEven(root->child_10, aux, n);
	}
	
	if (root->child_11 != NULL) {


		// apends 11 to aux string
		(*n)++;
		aux[(*n)] = '1';
		(*n)++;
		aux[(*n)] = '1';


		PrintTableEven(root->child_11, aux, n);

	}
	
	// cleans the last two bits to return to the ascendent node
	aux[(*n)] = '\0';
	(*n)--;
	aux[(*n)] = '\0';
	(*n)--;

	return;
}

/**Descrição da função:
 * Liberta toda a memória de forma recursiva, em profundidade primeiro **/

struct TwoBitNode * FreeTwoBitPrefixTree(struct TwoBitNode *root_two) {
	
	if (root_two == NULL) {
		return root_two;
	}

	if (root_two->child_00 != NULL) {
		FreeTwoBitPrefixTree(root_two->child_00);
	}

	if (root_two->child_01 != NULL) {
		FreeTwoBitPrefixTree(root_two->child_01);
	}

	if (root_two->child_10 != NULL) {
		FreeTwoBitPrefixTree(root_two->child_10);
	}

	if (root_two->child_11 != NULL) {
		FreeTwoBitPrefixTree(root_two->child_11);
	}

	free(root_two);
	root_two = NULL;

	return root_two;
}
