#include <stdio.h>
/* filecopy: copyfiles the input file to the output file
 * originates from K&R C */
void filecopy(FILE* input, FILE* output) {
	int chr;
	while ((chr = getc(input)) != EOF) {
		putc(chr, output);
	}
}
