#include <string.h> // I added
#include <stdlib.h> // I added
#include <sys/stat.h> // I added // #include "types.h" I commented out these four include statements
#include <stdio.h>    // I added // #include "stat.h"
#include <dirent.h>   // I added // #include "user.h"
#include <unistd.h>   // I added // #include "fs.h"
#include <fcntl.h> // I added
#define DIRSIZ 40

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;
  
  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;
  
  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}

void
ls(char *path)
{
  char buf[512], *p;
  int fd;
  struct dirent *de; // struct dirent de; // CHANGE #1
  struct stat st;

  if ((fd = open(path, O_RDONLY)) < 0) { // if((fd = open(path, 0)) < 0){ CHANGE #11?
    fprintf(stderr, "ls: cannot open %s\n", path); // printf(2, "ls: cannot open %s\n", path); CHANGE #1
    close(fd);
    return;
  }
  
  if(stat(path, &st) < 0){ // changed
    fprintf(stderr, "ls: cannot stat%s\n", path); // printf(2, "ls: cannot stat %s\n", path); CHANGE #2
    close(fd);
    return;
  }
  
  switch(st.st_mode & S_IFMT) {// switch(st.type){ CHANGE #???
    case S_IFREG: // case T_FILE: CHANGE #???
    fprintf(stdout, "%s %d %d %d\n", fmtname(path), st.st_mode, st.st_ino, st.st_size); // printf(1, "%s %d %d %d\n", fmtname(path), st.type, st.ino, st.size); CHANGE #3
    break;
  
    case S_IFDIR: // case T_DIR: CHANGE # ???
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
     fprintf(stdout, "ls: path too long\n"); // printf(1, "ls: path too long\n"); CHANGE #4
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';

    DIR *d = opendir(path); // changed / I added
    if (!d) {
    	fprintf(stderr, "ls: cannot open directory %s\n", path);
	return;
    }
    while ((de = readdir(d)) != NULL) { // while(read(fd, &de, sizeof(de)) == sizeof(de)){ CHANGE #10
      if (de->d_ino == 0) // if(de.inum == 0) CHANGE #???
        continue;
      memmove(p, de->d_name, DIRSIZ); // memmove(p, de.name, DIRSIZ); CHANGE #???
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){
        fprintf(stdout, "ls: cannot stat %s\n", buf); // printf(1, "ls: cannot stat %s\n", buf); CHANGE #5
        continue;
      }
      fprintf(stdout, "%s %d %d %d\n", fmtname(buf), st.st_mode, st.st_ino, st.st_size); // printf(1, "%s %d %d %d\n", fmtname(buf), st.type, st.ino, st.size); CHANGE #6
    }
    closedir(d); // added
    break;
  }
    close(fd);
}

int
main(int argc, char *argv[])
{
  int i;

  if(argc < 2){
    ls(".");
    exit(0); // exit(); CHANGE #7
  }
  for(i=1; i<argc; i++)
    ls(argv[i]);
  exit(0); // exit(); CHANGE #8
}
