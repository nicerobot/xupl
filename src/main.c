#include <stdio.h>
#include <err.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include "xupl.h"

int main (int argc, char *argv[]) {

	int atty = isatty(0);
	if (argc <= 1 && atty) return 1;

	const char* FILENAME = argv[1];

	FILE* in = NULL;
	int fd = -1;
	off_t filesize = 0;
	off_t buffsize = 32 * 1024;

	if (!atty) {
		in = stdin;
	} else {
		fd = open(FILENAME, O_RDONLY);
		if (fd == -1) {
			err(2, "open: %s", FILENAME);
			return 2;
		}
		struct stat fs;
		if (fstat(fd, &fs) == -1) {
			err(3, "stat: %s", FILENAME);
			return 3;
		}
		in = fdopen(fd, "r");
		buffsize = filesize = fs.st_size;
		//printf("filesize=%lld\n", filesize);
	}

	xupl(in,buffsize);

	if (filesize > 0) {
		fclose(in);
	}

}
