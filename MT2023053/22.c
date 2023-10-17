/*
============================================================================
Name : 22.c
Author : Samarpita Bhaumik
Description : Write a program, open a file, call fork, and then write to the file by both the child as well as the
              parent processes. Check output of the file.
Date: 8th September, 2023.
============================================================================
*/
#include<stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
int main(int argc,char *argv[])
{
   int fd= open("22.txt",O_CREAT|O_RDWR|O_APPEND,0744);
   pid_t p= fork();
   if(!p)
   {
     write(fd,"From Child\n",11);
   }
   else
   {
    wait(0);
    write(fd,"From Parent\n",12);
   }
   close(fd);
  return 0;
  }
