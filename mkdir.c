#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
	int retval = 0;
	for (int i = 1; i < argc; i++) {
		if (mkdir(argv[i], (S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)) != 0) {
			retval = 1;
			fprintf(stderr, "%s: couldn't make %s\n", argv[0], argv[i]);
		}
	}
	return retval;
}
