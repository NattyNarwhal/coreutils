#include <stdio.h>
#include <unistd.h>
#include <wctype.h>

#define NEWLINE '\n'

/* returns an array of numbers
 * [0] = lines
 * [1] = words
 * [2] = chars
 */
void word_count(FILE* f, int counts[]) {
	/* if we don't init these, we get garbage */
	counts[0] = counts[1] = counts[2] = 0;
	int chr;
	int in_word = 0;
	while((chr = getc(f)) != EOF) {
		counts[2]++;
		if (chr == NEWLINE) counts[0]++;
		if (iswspace(chr)) in_word = 1;
		else if (in_word) {
			in_word = 0;
			counts[1]++;
		}
	}
}

int main(int argc, char *argv[]) {
	int retval = 0;

	/* if no params, use stdin */
	if (argc == 1) {
		int counts[3];
		word_count(stdin, counts);
		printf("\t%d\t%d\t%d\n", counts[0], counts[1], counts[2]);
	} else {
		int total_lines, total_words, total_chars, total_files;
		total_lines = total_words = total_chars = total_files = 0;
		for (int i = 1; i < argc; i++) {
			int counts[3];
			FILE* f = fopen(argv[i], "r");
			word_count(f, counts);
			total_lines += counts[0];
			total_words += counts[1];
			total_chars += counts[2];
			total_files++;
			fclose(f);
			printf("\t%d\t%d\t%d\t%s\n", counts[0], counts[1], counts[2], argv[i]);
		}
		if (total_files > 1) {
			printf("\t%d\t%d\t%d\ttotal (%d)\n", total_lines, total_words, total_chars, total_files);
		}
	}

	return retval;
}
