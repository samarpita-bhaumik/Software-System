/*
============================================================================
Name : 17c.c
Author : Samarpita Bhaumik
Description : Write a program to execute ls -l | wc.
              c. use fcntl
Date: 10th Oct,2023
============================================================================
*/
#include<stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

int main()
{
 int fd[2];
 pipe(fd);
 if(!fork())
 {
  close(1);
  close(fd[0]);
  fcntl(fd[1],F_DUPFD,1);
  execlp("ls","ls","-l",(char*)NULL);
 }
 else
 {
   close(0);
   close(fd[1]);
   fcntl(fd[0],F_DUPFD,0);
   execlp("wc","wc",(char*)NULL);
  }
 }
