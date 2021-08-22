#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void
find(char *path, char *filename)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0){
    fprintf(2, "find: cannot open %s\n", path);
    exit(1);
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    exit(1);
  }
  
  switch(st.type){
  case T_FILE:
    fprintf(2, "find: %s is not a directory\n", path);
    close(fd);
    exit(1);
    
  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("find: path too long\n");
      close(fd);
      exit(1);
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0 || !strcmp(".", de.name) || !strcmp("..", de.name)){
        continue;
      }
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){
        printf("find: cannot stat %s\n", buf);
        continue;
      }
      if(st.type == T_FILE && !strcmp(filename, de.name)){
        fprintf(1, "%s\n", buf);
      }
      else if(st.type == T_DIR){
        find(buf, filename);
      }
    }
  }
  close(fd);
}

int 
main(int argc, char *argv[])
{
  if(argc < 2 || argc > 3){
    fprintf(2, "Usge: find\n");
    exit(1);
  }
  else if(argc == 2){
    find(".", argv[1]);
  }
  else{
    find(argv[1], argv[2]);
  }
  exit(0);
}
