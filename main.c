#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "prefixTrees.h"

int main(int argc, char const *argv[])
{	
	char temp[50];
	int user_choice = 0;
	char address[32];
	char prefix[PREFIXSIZE];

	int tree_level = -1;
	char binary_level = '9';

	/* Prefix Tree */
	struct Node *root = PrefixTree(argc, argv);

	PrintTable(root, root, &binary_level, prefix, &tree_level);


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