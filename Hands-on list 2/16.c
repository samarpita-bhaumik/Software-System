/*
============================================================================
Name : 16.c
Author : Samarpita Bhaumik
Description : Write a program to send and receive data from parent to child vice versa. Use two way
              communication.
Date: 10th Oct,2023
============================================================================
*/
#include<stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
  char *buf;
  char *buf1;
  int fd[2],fd1[2];
  pipe(fd);
  if(!fork())
  {
   buf="from child";
    close(fd[0]);
    close(fd1[1]);
    printf("Enter message to the parent:");
    
    write(fd[1],&buf,sizeof(buf));
    read(fd1[0],&buf1,sizeof(buf1));
    printf("Message from parent:%s\n",buf1);
   }
   else
   {
    buf1="from parent";
     close(fd[1]);
    close(fd1[0]);
    read(fd[0],&buf,sizeof(buf));
    printf("Message from child:%s\n",buf);
    printf("Enter message to the child:");
    
    write(fd1[1],&buf1,sizeof(buf1));
    }

  }  
