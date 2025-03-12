#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char *argv[]) {
	// verbose = 0 means no -v. 1 means user wants -v
	int verbose = 0;
	if (strcmp(argv[1], "-v") == 0) {
		verbose = 1;
		argv++;
		argc--;
	}

	if (verbose == 1) {
		int pipefd[2];
		pipe(pipefd);

		int childpid1 = fork();
		if (childpid1 == 0) {
			close(pipefd[0]);
			dup2(pipefd[1], 1);
			close(pipefd[1]);
			execvp(argv[1], &argv[1]);
		}

		int childpid2 = fork();

		if (childpid2 == 0) {
			close(pipefd[1]);
			dup2(pipefd[0], 0);
			close(pipefd[0]);
			int fd = open("record.out", O_WRONLY | O_CREAT | O_TRUNC, 0644);
			dup2(fd, 1);
			close(fd);
			execv("./filter", argv);
		}
		close(pipefd[0]);
		close(pipefd[1]);

		int status;
		waitpid(childpid1, &status, 0);
		waitpid(childpid2, &status, 0);
	} else {
		int fd = open("record.out", O_WRONLY | O_CREAT | O_TRUNC, 0644);
        	//close(1);
        	int pid = fork();
        	if (pid == 0) {
                	dup2(fd, 1);
                	close(fd);
                	execvp(argv[1], &argv[1]);
        	} else {
                	close(fd);
                	int status;
                	waitpid(pid, &status, 0);
                	if (WIFEXITED(status)) {
                        	printf("exited, status=%d\n", WEXITSTATUS(status));
                	} // this never prints since I closed fd 1.
        	}	
	}
	return 0;
}





























