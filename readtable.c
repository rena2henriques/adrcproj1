#include "readtable.h"

void readTable(int argc, char const *argv[]) {
	
	FILE *file = NULL;
	char temp[40];

	/* temporary variables */
	char prefix[32];
	int nexthop = 0;

	/* opening the file, if the user gives a file on command line, it's used, 
	otherwise it uses the PrefixTable.txt file*/
	if (argc == 1) {
		if ( (file = fopen("PrefixTable.txt", "r")) == NULL ) {
			printf("Error while reading file: %s\n", strerror(errno));
			return;
		}
	} else {
		if ( (file = fopen(argv[1], "r")) == NULL ) {
			printf("Error while reading file: %s\n", strerror(errno));
			return;
		}
	}

	/* Temporary reading of table, in the future it must create a tree */

	while (!feof(file)) {
		fgets(temp, sizeof(temp), file);
		sscanf(temp, "%s %d", prefix, &nexthop);
		printf("%s %d\n", prefix, nexthop);

		memset(prefix, 0, sizeof(prefix));

	}

	/* */


	if ( fclose(file) == EOF ) {
			printf("Error while closing file: %s\n", strerror(errno));
			return;
		}
}

