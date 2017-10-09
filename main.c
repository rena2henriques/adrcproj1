#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "prefixTrees.h"

int main(int argc, char const *argv[])
{	
	char temp[50] = "";
	int user_choice = 0;
	char address[32];
	char prefix_input[PREFIXSIZE] = "";
	char prefix_two[PREFIXSIZE] = "";
	char aux[PREFIXSIZE] = "";


	int tree_level = -1;
	int tree_level_two = 0;
	char binary_level = '9';
	char binary_level_two = '9';
	int next_hop = -1;
	int next_hop_input = -1;
	int relevant_hop = -1;

	/* Prefix Tree */
	struct Node *root = PrefixTree(argc, argv);
	struct TwoBitNode *root_two = NULL;	

	while(1) {
		printf("\n");
		printf("What do you want to do:\n");
		printf(" - write 1 if you want to print the table;\n");
		printf(" - write 2 and an Adress if you want to look up the next-hop of said address;\n");
		printf(" - write 3, a prefix and the associated next-hop to write them in the table;\n");
		printf(" - write 4 and a prefix if you want to remove the prefix from the table;\n");
		printf(" - write 5 if you want to print an even prefix table;\n");
		printf(" - write exit if you want to exit the program\n");
		fgets(temp, sizeof(temp), stdin);
		printf("\n");
		
		if( sscanf(temp, "%d", &user_choice) == 1 && user_choice == 1) {
			printf("Prefix Table:\n");
			tree_level = -1;
			PrintTable(root, root, &binary_level, aux, &tree_level);
		}
		else if( (sscanf(temp, "%d %s", &user_choice, address) == 2) && user_choice == 2) {
			tree_level = 0;
			next_hop = -1; //input em LookUp
			next_hop = LookUp(root, root, address, &next_hop, &tree_level);
			if (next_hop == -1 ) {
				printf("There's no available next_hop value for that prefix!\n");
			} else {
				printf("The correspondent next-hop is %d\n", next_hop);
			}
		}
		else if( sscanf(temp, "%d %s %d", &user_choice, prefix_input, &next_hop_input) == 3 && user_choice == 3) {
			root = InsertPrefix(prefix_input, next_hop_input, root);
			//confirmar a inserção
			tree_level = 0;
			next_hop = -1;
			if (LookUp(root, root, prefix_input, &next_hop, &tree_level) == next_hop_input) { // this line might be useless
				printf("Prefix successfully inserted\n");
			} else {
				printf("Ocorreu um erro na inserção\n");
			}
		}
		else if( sscanf(temp, "%d %s", &user_choice, prefix_input) == 2 && user_choice == 4) {
			tree_level = 0;
			root = DeletePrefix(root, prefix_input, &binary_level, aux, &tree_level);
			printf("Prefix successfully deleted\n");
		}
		
		else if( sscanf(temp, "%d", &user_choice) == 1 && user_choice == 5) {
			printf("Prefix Table Even:\n");
			tree_level_two = 0;
			memset(prefix_two, 0, sizeof(prefix_two));
			root_two = FreeTwoBitPrefixTree(root_two);

			root_two = BinaryToTwoBit(root, root, root_two, &tree_level_two, prefix_two, &binary_level_two, &relevant_hop);
			tree_level_two = -1;
			memset(prefix_two, 0, sizeof(prefix_two));
			PrintTableEven(root_two, root_two, &binary_level_two, prefix_two, &tree_level_two );
		}
		
		else if( sscanf(temp, "%s", address) == 1) {
			if(strcmp(address, "exit") == 0) {
				// free the prefix table
				root = FreePrefixTree(root);
				root_two = FreeTwoBitPrefixTree(root_two);
				break;
			}
		}
				
		//clear buffers
		memset(address, 0, sizeof(address));
		memset(temp, 0, sizeof(temp));
		memset(prefix_input, 0, sizeof(prefix_input));
		memset(aux, 0, sizeof(aux));
	}

	return 0;
}
