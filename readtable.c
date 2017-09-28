#include "readtable.h"

void readTable(int argc, char const *argv[]) {
	
	FILE *file = NULL;

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

	/* Do Magic stuff */

}

