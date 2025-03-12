#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main() {
	char buffer[1024];

	while (fgets(buffer, sizeof(buffer), stdin)) {
		printf("record.out> %s", buffer);
	}

	return 0;
}

























