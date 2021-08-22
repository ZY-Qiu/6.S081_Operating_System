#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

void
assemble(char **args, int indx, char *buf, int n)
{
  int prev = 0;
  for(int i = 0; i <= n; i++){
    if(buf[i] == ' ' || buf[i] == '\n'){
      args[indx] = malloc(i-prev);
      memmove(args[indx++], &buf[prev], i-prev);
      prev = i + 1;
    }
  }
}

int
main(int argc, char *argv[])
{
  char buf[512];
  int n;
  if ((n = read(0, buf, sizeof buf)) < 0){
    fprintf(2, "xargs: failed to read buf\n");
    exit(1);
  }
   
  char *args[MAXARG+1];
  int indx = 0;
  
  if(!strcmp(argv[1], "-n")){
    for(int i = 1; i < argc; i++){
      args[indx] = malloc(sizeof(argv[i]));
      memmove(args[indx++], argv[i], sizeof(argv[i]));
    }
    //for -n func
    // Wasted!!!
  }
  
  else{
    for(int i = 1; i < argc; i++){
      args[indx] = malloc(sizeof(argv[i]));
      memmove(args[indx++], argv[i], sizeof(argv[i]));
    }
    assemble(args, indx, buf, n);
    exec(args[0], args);
  }
  
  exit(0);
}
