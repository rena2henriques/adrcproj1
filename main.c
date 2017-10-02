#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "prefixTrees.h"

int main(int argc, char const *argv[])
{	
	char temp[50] = "";
	int user_choice = 0;
	char address[32];
	char prefix[PREFIXSIZE] = "";
	char aux[PREFIXSIZE] = "";

	int tree_level = -1;
	char binary_level = '9';
	int next_hop = -1;

	/* Prefix Tree */
	struct Node *root = PrefixTree(argc, argv);

	/**  TEMPORARY  **/
	printf("Antes da eliminação:\n");
	PrintTable(root, root, &binary_level, aux, &tree_level);

	tree_level = 0;
	binary_level = '9';
	
	memset(aux, 0, sizeof(aux));

	strcpy(prefix, "0");

	printf("A eliminar o prefixo %s...\n", prefix);
	root = DeletePrefix(root, root, prefix, &binary_level, aux, &tree_level);

	memset(prefix, 0, sizeof(prefix));

	// momentary!
	tree_level = 0;
	strcpy(prefix, "0");

	printf("A procurar o prefixo %s...\n", prefix);
	next_hop = LookUp(root, root, prefix, &next_hop, &tree_level);

	if (next_hop == -1 ) {
		printf("There's no available next_hop value for that prefix!\n");
	} else {
		printf("The correspondent next-hop is %d\n", next_hop);
	}

	printf("Após eliminação:\n");
	memset(aux, 0, sizeof(aux));
	binary_level = '9';
	tree_level = -1;
	PrintTable(root, root, &binary_level, aux, &tree_level);

	/**            **/

	// while(1) {

	// 	printf("What do you want to do:\n");
	// 	printf(" - write 1 if you want to print the table;\n");
	// 	printf(" - write 2 and an Adress if you want to look up the next-hop of said address;\n");
	// 	printf(" - write 3, a prefix and the associated next-hop to write them in the table;\n");
	// 	printf(" - write 4 and a prefix if you want to remove the prefix from the table;\n");
	// 	printf(" - write exit if you want to exit the program\n");

	// 	fgets(temp, sizeof(temp), stdin);
	// 	if( sscanf(temp, "%d %s", &user_choice, address) == 1) {
			
	// 	}


	// }

	return 0;
}