#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[]) {
	struct stat sb;
	
	if (argc != 2) {
               fprintf(stderr, "Usage: %s <pathname>\n", argv[0]);
	       exit(EXIT_FAILURE);
	}

	if (stat(argv[1], &sb) == -1) {
		perror("stat");
		exit(EXIT_FAILURE);
	}

	printf("I-node number: %ju\n", (uintmax_t) sb.st_ino);
	exit(EXIT_SUCCESS);
}
