/* foreach: accepts a program as first parameter and files to
   send to stdin into that program as the other parameters */

#include <stdio.h>
#include <unistd.h>
#include "lib/filecopy.h"

int main(int argc, char *argv[]) {
	int retval = 0;

	for (int i = 2; i < argc; i++) {
		FILE* p = popen(argv[1], "w");
		FILE* f = fopen(argv[i], "r");
		filecopy(f, p);
		pclose(p);
	}

	return retval;
}
