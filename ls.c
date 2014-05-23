#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <libutil.h>	/* for humanize_number - requires `-lutil` for build */
#include <sys/stat.h>	/* for stat */
#include <string.h>	/* for strmode */
#include <pwd.h>	/* for getpwuid */
#include <grp.h>	/* for getgrgid */

#define SIZESTRLEN 14	/* limit based on length of long long */
#define HMSZSTRLEN 5 	/* up to 3 digits + prefix + null byte */
/*
 * These are for print_struct_stat. LONGPRINTF pades with tabs for large file
 * sizes, and HMSZPRINTF prints without because a humanized string is always
 * equal to HMSZSTRLEN.
 */
#define LONGPRINTF "%s %d %s %s\t%s\t%s %s\n"
#define HMSZPRINTF "%s %d %s %s %s %s %s\n"

int hide_dots	= 1;
int long_list	= 0;
int humanize	= 0;

/* prints a longlist from a struct stat *f, *dir is filename */
int print_struct_stat(struct stat *f, char *dir) {
	int retval = 0;
	char *format;

	/* get mode */
	char mode[12]; /* 12 is how big a mode string can be */
	strmode(f->st_mode, mode);

	/* get formatted date */
	struct tm *ft;
	ft = localtime(&f->st_ctime);
	char tformat[14]; /* 14 should be the max size for the format we use */
	strftime(tformat, 14, "%b %d %H:%M", ft);

	/* get uid/gid names */
	struct passwd *uid = getpwuid(f->st_uid);
	struct group *gid = getgrgid(f->st_gid);

	/* optionally humanize size, and set the proper sizing string */
	char size[SIZESTRLEN];
	if (humanize) {
		format = HMSZPRINTF;
		humanize_number(size, HMSZSTRLEN, f->st_size, "", HN_AUTOSCALE, (HN_DECIMAL | HN_B | HN_NOSPACE));
	} else {
		format = LONGPRINTF;
		snprintf(size, SIZESTRLEN, "%lld", f->st_size);
	}

	/* print & clean */
	printf(format, mode, f->st_nlink, uid->pw_name, gid->gr_name, size, tformat, dir);
	return retval;
}

/* prints directory listing for a directory */
int print_dirents(char *dir) {
	int retval = 0;
	DIR *d;
	if ((d = opendir(dir)) == NULL) {
		extern char *__progname;
		fprintf(stderr, "%s: couldn't open %s\n", __progname, dir);
		retval = 1;
		return retval;
	}
	struct dirent *e;
	while ((e = readdir(d))) {
		if (hide_dots & (e->d_name[0] == '.')) {
			continue;
		}
		if (long_list) {
			/* do fstatat magic */
			struct stat *f = malloc(sizeof(struct stat));
			if (fstatat(dirfd(d), e->d_name, f, 0) == 0) {
				print_struct_stat(f, e->d_name);
			}
			free(f);
		} else {
			printf("%s\n", e->d_name);
		}
	}
	closedir(d);
	return retval;
}

/* prints directory listing for a file */
int print_file(char *dir) {
	int retval = 0;
	if (long_list) {
		struct stat *f = malloc(sizeof(struct stat));
		if (lstat(dir, f) == 0) {
			print_struct_stat(f, dir);
		}
		free(f);
	} else {
		printf("%s\n", dir);
	}
	return retval;
}

/* calls the right print method */
int print_thing(char *dir) {
	int retval = 0;
	struct stat* thing = malloc(sizeof(struct stat));
	if (stat(dir, thing) == 0) {
		if(S_ISDIR(thing->st_mode)) {
			retval = print_dirents(dir);
		} else {
			print_file(dir);
		}
	}
	free(thing);
	return retval;
}

void usage(void) {
	extern char *__progname;
	printf("usage: %s [-alh] [files]\n", __progname);
	exit(1);
}

int main(int argc, char *argv[]) {
	int retval = 0;
	int ch;
	while ((ch = getopt(argc, argv, "ahl")) != -1) {
		switch (ch) {
			case 'a':
				hide_dots = 0;
				break;
			case 'h':
				humanize = 1;
				break;
			case 'l':
				long_list = 1;
				break;
			default:
				usage();
		}
	}
	if (argc == optind) {
		retval = print_thing(".");
	} else {
		for (int i = optind; i < argc; i++) {
			if (argc - optind > 1) {
				printf("%s:\n", argv[i]);
				print_thing(argv[i]);
				puts("");
			} else {
				retval = print_thing(argv[i]);
			}
		}
	}
	return retval;
}
