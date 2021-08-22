#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int 
main(int argc, char *argv[])
{
  if(argc != 1){
    fprintf(2, "usage: pingpong\n");
    exit(1);
    }
    
  char buf[5];
  int p[2];
  pipe(p); // p[0] = open(temp, O_CREATE | O_RDWR), p[1] = dup(p[0]);
  
  if(fork() == 0){
    read(p[0], buf, sizeof(buf)); // Wait utill ether data written or all fd to write end close.
    fprintf(1, "%d: received ping\n", getpid());
    close(p[0]);
    
    write(p[1], "a", 1);
    close(p[1]);
    exit(0);
  }
  else{
    write(p[1], "a", 1);
    close(p[1]);
    wait(0); //Why core of the problem?
    
    read(p[0], buf, sizeof(buf));
    fprintf(1, "%d: received pong\n", getpid());
    exit(0);
  }
}
