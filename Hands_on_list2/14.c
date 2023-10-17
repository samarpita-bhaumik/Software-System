#include<stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main()
{
  int i,j,fd[2];
  char buf[50];
  i= pipe(fd);
  printf("i=%d\n",i);
  j=write(fd[1],"Hello\n",6);
  read(fd[0],&buf,j);
  printf("From pipe:%s\n",buf);
  printf("\nFile descriptor\n");
  system("ls -l /proc/$$/fd");
  }
