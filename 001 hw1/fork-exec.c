#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// You must fill in wherever it says 'HOMEWORK'.

void help();
int read_command(char *program);

// In C, a string is of type 'char []' or equivalently, 'char *'
int main(int argc, char *argv[]) {
  help();

  while (1) { // This is a "read-exec-print" loop.
    printf("%% "); // print prompt
    fflush(stdout); // Don't wait for 'newline'.  Flush stdout to screen now.

    int cmd;
    while (1) {
      cmd = read_command(argv[0]);
      if (cmd == '\n') { continue; } // Ignore newlines
      if (cmd == '#') {
        printf("HOMEWORK: This is a comment char.  Read through newline.\n");
	
	char buf[1];
	read(0, buf, 1);
	while (buf[0] != '\n') {
		read(0, buf, 1);
	}
        continue;
      }
      break;
    }

    // The syntax "1" is an int and "'1'" is a char.
    switch (cmd) {
      case 'h':
        help();
        break; // Don't forget 'break'.  Otherwise control passes to next case.
      case 'x':
      case 'q':
        printf("Exiting\n");
        exit(0); // The argument 0 of exit says return code is 0:  success.

      case '1':
        printf("HOMEWORK:  Execute 'ls' to list files.\n");	

	int pid_ls = fork();
	if (pid_ls == 0) {
		char *args_ls[] = {"ls", NULL};
        	execvp("ls", args_ls);
	} else {
		sleep(2);
		printf("The child should now have executed 'ls' \n");
	}

        break;
      case '2':
        printf("HOMEWORK:  Execute 'ls -l' to list files.\n");

	int pid_ls_l = fork();
	if (pid_ls_l == 0) {
		char *args_ls_l[] = {"ls", "-l", NULL};
        	execvp("ls", args_ls_l);
	} else {
		sleep(2);
		printf("'ls -l' should have executed \n");
	}

        break;
      case '3':
        // You'll need to continue to read 'dir' and stop at newline.
        printf("HOMEWORK:  See 'man 2 chdir'; implement 'cd'.\n");
	

	//char discard;
	//while (read(0, &discard, 1) == 1 && discard != '\n') {}

	//printf("Enter directory to change to: ");
	//fflush(stdout);

	char dir_cd[100];
	int i_cd = 0;
	char buf_cd[1];
	read(0, buf_cd, 1);
	while (buf_cd[0] != '\n') {
		dir_cd[i_cd] = buf_cd[0];
		i_cd++;
		read(0, buf_cd, 1);
	}
	dir_cd[i_cd] = '\0';

	if (chdir(dir_cd) == 0) {
		printf("Changed directory to: %s\n", dir_cd);
	} else {
		printf("failed\n");
	}

        break;
	case '4':
        // You'll need to continue to read 'env var' and stop at newline.
        printf("HOMEWORK:  See 'man 3 getenv'; print env var (e.g., PWD).\n");
	
	//char discard_getenv;
        //while (read(0, &discard_getenv, 1) == 1 && discard_getenv != '\n') {}

	//printf("Enter an environment variable: ");
        //fflush(stdout);

	char env_var_getenv[100];
	int i_getenv = 0;
	char buf_getenv[1];
	read(0, buf_getenv, 1);
	while (buf_getenv[0] != '\n') {
		env_var_getenv[i_getenv] = buf_getenv[0];
		i_getenv++;
		read(0, buf_getenv, 1);
	}
	env_var_getenv[i_getenv] = '\0';

	char *env_value = getenv(env_var_getenv);
	if (env_value != NULL) {
		printf("Value of %s: %s\n", env_var_getenv, env_value);
	} else {
		printf("Environment variable '%s', not found\n", env_var_getenv);
	}

        break;
      case '5':
        // You'll need to to read 'env var' and string; stop at newline.
        printf("HOMEWORK:  See 'man 3 setenv'.\n");

	//char discard_setenv;
        //while (read(0, &discard_setenv, 1) == 1 && discard_setenv != '\n') {}

	//printf("Enter the environment variable name: ");
    	//fflush(stdout);

	char env_var[100];
	int i_setenv = 0;
	char buf_setenv[1];
	read(0, buf_setenv, 1);
	while (buf_setenv[0] != '\n') {
		env_var[i_setenv] = buf_setenv[0];
		i_setenv++;
		read(0, buf_setenv, 1);
	}
	env_var[i_setenv] = '\0';

	//printf("Enter the value for %s: ", env_var);
    	//fflush(stdout);

	char value[100];
	int i_value = 0;
	char buf_value[1];
	read(0, buf_value, 1);
	while (buf_value[0] != '\n') {
		value[i_value] = buf_value[0];
		i_value++;
		read(0, buf_value, 1);
	}
	value[i_value] = '\0';

	setenv(env_var, value, 1);

	printf("Environment variable %s set to %s\n", env_var, value);

        break;
      case '6':
        // Continue to read 'src' and 'dest' files and stop at newline for each.
        printf("HOMEWORK:  Execute 'cp src dest'.\n");
        
	//char discard_cp;
        //while (read(0, &discard_cp, 1) == 1 && discard_cp != '\n') {}

        //printf("Enter src name: ");
        //fflush(stdout);

	char src[100];
	int i_cp = 0;
	char buf_cp[1];
	read(0, buf_cp, 1);
	while (buf_cp[0] != '\n') {
		src[i_cp] = buf_cp[0];
		i_cp++;
		read(0, buf_cp, 1);
	}
	src[i_cp] = '\0';

	//printf("Enter dest name: ");
        //fflush(stdout);

	char dest[100];
	int i_dest = 0;
	char buf_dest[1];
	read(0, buf_dest, 1);
	while (buf_dest[0] != '\n') {
		dest[i_dest] = buf_dest[0];
		i_dest++;
		read(0, buf_dest, 1);
	}
	dest[i_dest] = '\0';


	int pid_cp = fork();
        if (pid_cp == 0) {
                char *args_cp[] = {"cp", src, dest, NULL};
                execvp("cp", args_cp);
        } else {
                sleep(2);
                printf("'cp src dest' should have executed \n");
        }
	
	break;
      case '7':
        // You'll need to continue to read 'dir' and stop at newline.
        printf("HOMEWORK:  Execute 'mkdir dir'.\n");
       

	//char discard_mkdir;
        //while (read(0, &discard_mkdir, 1) == 1 && discard_mkdir != '\n') {}

        //printf("Enter a directory name: ");
        //fflush(stdout);

	char dir_mkdir[100];
	int i_mkdir = 0;
	char buf_mkdir[1];

	read(0, buf_mkdir, 1);
	while (buf_mkdir[0] != '\n') {
		dir_mkdir[i_mkdir] = buf_mkdir[0];
		i_mkdir++;
		read(0, buf_mkdir, 1);
	}
	dir_mkdir[i_mkdir] = '\0';

	int pid_mkdir = fork();
        if (pid_mkdir == 0) {
		char *args_mkdir[] = {"mkdir", dir_mkdir, NULL};
        	execvp("mkdir", args_mkdir);
        } else {
                sleep(2);
                printf("The child should now have executed 'mkdir' \n");
        }
	
	break;
      case '8':
        // You'll need to continue to read 'file' and stop at newline.
        printf("HOMEWORK:  Execute 'rm file'.\n");
       
	//char discard_rm;
        //while (read(0, &discard_rm, 1) == 1 && discard_rm != '\n') {}

        //printf("Enter a file name: ");
        //fflush(stdout);

	char file[100];
	int i_rm = 0;
	char buf_rm[1];

	read(0, buf_rm, 1);
	while (buf_rm[0] != '\n') {
		file[i_rm] = buf_rm[0];
		i_rm++;
		read(0, buf_rm, 1);
	}
	file[i_rm] = '\0';

	int pid_rm = fork();
        if (pid_rm == 0) {
                char *args_rm[] = {"rm", file, NULL};
                execvp("rm", args_rm);
        } else {
                sleep(2);
                printf("The child should now have executed 'rm file' \n");
        }
	
	break;
      case '9':
        // You'll need to continue to read 'dir' and stop at newline.
        printf("HOMEWORK:  Execute 'rmdir dir'.\n");
        
	//char discard_rmdir;
        //while (read(0, &discard_rmdir, 1) == 1 && discard_rmdir != '\n') {}

        //printf("Enter a directory name: ");
        //fflush(stdout);


	char dir_rmdir[100];
	int i_rmdir = 0;
	char buf_rmdir[1];

	read(0, buf_rmdir, 1);
	while (buf_rmdir[0] != '\n') {
		dir_rmdir[i_rmdir] = buf_rmdir[0];
		i_rmdir++;
		read(0, buf_rmdir, 1);
	}
	dir_rmdir[i_rmdir] = '\0';

	int pid_rmdir = fork();
        if (pid_rmdir == 0) {
                char *args_rmdir[] = {"rmdir", dir_rmdir, NULL};
                execvp("rmdir", args_rmdir);
        } else {
                sleep(2);
                printf("The child should now have executed 'rmdir dir' \n");
        }
	
	break;

      default:
        printf("Unrecognized command: %c\n", (char)cmd);
        printf("HOMEWORK: Read through newline.\n");
        
	char buf[1];
	while (read(0, buf, 1) == 1 && buf[0] != '\n') {}
	
	break;
    }
  }

  return 0;  // When the process exits, this is its return code. 0 is success.
}

void help() {
  printf("HOMEWORK:  Print help statement showing ALL cases.\n");
  printf("EXAMPLE:\n 1: ...\n 2: ...\n h: ...\n x: ...\n q: ...\n #: ...\n");
  printf("1: Executes 'ls' \n");
  printf("2: Executes 'ls -1' \n");
  printf("3: Implementation of 'cd' to change directory \n");
  printf("4: Prints 'getenv' \n");
  printf("5: Sets 'setenv' \n");
  printf("6: Executes 'cp src dest' \n");
  printf("7: Executes 'mkdir dir' \n");
  printf("8: Executes 'rm file' \n");
  printf("9: Executes 'rmdir dir' \n");
  printf("h: Prints help statement showing all cases\n");
  printf("x: quits the program \n");
  printf("q: quits the program \n");
  printf("#: a comment \n");
  printf("Otherwise: Unrecognized command \n");
}

int read_command(char *program) {
  char buf[1];
  while (1) {
    int rc;
    rc = read(0, buf, 1);  // fd is 0 (stdin); read just 1 character into buf.
    if (rc == 1) { // If 1 char was read
      break;
    } else if (rc == -1) { // If error
      if (rc == EAGAIN || rc == EINTR) {
        continue;  // The O/S kernel wasn't ready.  Ask it again.
      } else {
        fprintf(stderr, "%s: Error in reading command\n", program);
        exit(1);  // Error:  Set return code to error number 1
      }
    } else if (rc == 0) {
      printf("%s: End-of-input\n", program);
      exit(0);
    }
  }

  int cmd = buf[0];  // Convert 'char' to an 'int'.
  return cmd;
}
