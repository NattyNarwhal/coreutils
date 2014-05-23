#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	int retval = 0;
	for (int i = 1; i < argc; i++) {
		if (rmdir(argv[i]) != 0) {
			retval = 1;
			fprintf(stderr, "%s: couldn't remove %s\n", argv[0], argv[i]);
		}
	}
	return retval;
}
