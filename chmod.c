#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
	int retval = 0;
	if (argc < 3) {
		retval = 2;
		fprintf(stderr, "%s: not enough parameters\n", argv[0]);
	} else {
		mode_t mode = strtol(argv[1], NULL, 8);
		for (int i = 2; i < argc; i++) {
			if (chmod(argv[i], mode) != 0) {
				retval = 1;
				fprintf(stderr, "%s: couldn't change %s\n", argv[0], argv[i]);
			}
		}
	}
	return retval;
}
