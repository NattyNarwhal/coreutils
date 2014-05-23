#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

#define ASCII_LIMIT 127 /* where the ASCI table end*/
#define SPACE ' '

int ascii = 1;	/* use all characters on the ASCII table, even control chars
		 * this is useful for feeding as a WiFi passphrase to get hex */
int space = 1;	/* allow a space character*/
int newln = 1;	/* end on a newline*/

int chars = 8;

int getpwch() {
	int i = arc4random_uniform(ASCII_LIMIT);
	/* do not prematurely end on a null byte */
	if (i == '\0') goto fail;
	if (ascii) {
		/* if we fail try again */
		if (!isprint(i)) goto fail;
		if (space) {
			/* don't use \t or other weird whitespaces
			 * as some things don't like them */
			if (isspace(i) & (i != SPACE)) goto fail;
		} else if (!space) {
			if (isspace(i)) goto fail;
		}
	}
	return i;
	fail:
		return getpwch();
}

char* mkpw() {
	char* pw = calloc(chars, sizeof(char));
	for (int i = 0; i < chars; i++) {
		pw[i] = getpwch();
	}
	pw[chars + 1] = '\0';
	return pw;
}

void usage(void) {
	extern char *__progname;
	printf("usage: %s [-asn] [-c characters]\n", __progname);
	exit(1);
}

int main(int argc, char *argv[]) {
	int retval = 0;
	int ch;
	while ((ch = getopt(argc, argv, "ac:sn")) != -1) {
		switch (ch) {
			case 'a':
				ascii = 0;
				break;
			case 'c':
				chars = strtol(optarg, NULL, 0);
				break;
			case 's':
				space = 0;
				break;
			case 'n':
				newln = 0;
				break;
			default:
				usage();
		}
	}
	char* pw = mkpw();
	printf("%s", pw);
	if (newln) printf("\n");
	return retval;
}
