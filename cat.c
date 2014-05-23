/* cat: concats files/stdin and outputs to stdout */
#include <stdio.h>
#include <unistd.h>
#include "lib/filecopy.h"

int main(int argc, char* argv[]) {
	int retval = 0;
	/* if no parameters, accept stdin */
	if (argc == 1)
		filecopy(stdin, stdout);
	for (int i = 1; i < argc; i++) {
		FILE* f = fopen(argv[i], "r");
		filecopy(f, stdout);
		fclose(f);
	}
	return retval;
}
