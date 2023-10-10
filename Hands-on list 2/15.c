/*
============================================================================
Name : 15.c
Author : Samarpita Bhaumik
Description : Write a simple program to send some data from parent to the child process.
Date: 10th Oct,2023
============================================================================
*/
#include<stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
  char buf[80];
  int fd[2];
  pipe(fd);
  if(fork())
  {
    close(fd[0]);
    printf("Enter message to the child:");
    scanf(" %[^\n]",buf);
    write(fd[1],buf,sizeof(buf));
   }
   else
   {
     close(fd[1]);
     read(fd[0],buf,sizeof(buf));
     printf("Message from parent: %s\n",buf);
   }
   wait(0);
  }  
