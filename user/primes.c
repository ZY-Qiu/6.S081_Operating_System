#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void
filter(int *p)
{
  int p1[2];
  pipe(p1);

  char len, buf[64], outbuf[64];
  close(p[1]);
  len = read(p[0], buf, sizeof(buf));
  //printf("Read %d bytes\n", len);
  if(len){
    close(p[0]);
    close(p[1]);
    
    if(fork() == 0){
      filter(p1);
      exit(0);
    }
    else{
      close(p1[0]);
      fprintf(1, "prime %d\n", buf[0]);
      int j = 0;
      for(int i = 0; i < len; i++){
        int out = buf[i] % buf[0];
        if(out != 0){
          outbuf[j] = buf[i];
          j++;
        }
      }
      write(p1[1], outbuf, j);
      close(p1[1]);
      wait(0);
    }
  }
  exit(0);
}


int 
main(int argc, char *argv[])
{
  if (argc > 1)
    {
        fprintf(2, "Usage: primes\n");
        exit(1);
    }
  
  int p[2], i;
  char buf[34]; // Important! For int is 32bit, while read/write takes bytes as argument!
  pipe(p);
  
  if(fork() == 0){
    filter(p);
    exit(0);
  }
  else{
    for(i = 0; i < 34; i++){
      buf[i] = i+2;
    }
    write(p[1], buf, i);
    //printf("Waiting in main\n");
    wait(0);
  }
  exit(0);
}


