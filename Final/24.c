/*
============================================================================
Name : 24.c
Author : Samarpita Bhaumik
Description : Write a program to create an orphan process
Date: 8th September, 2023.
============================================================================
*/
#include<sys/types.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include <sys/wait.h>

int main()
{
  pid_t p= fork();
   if(!p)
   {
     printf("Before orphan, parent process id: %d\n",getppid());
     printf("Putting child to sleep for 10 sec.\n");
     sleep(10);
     printf("After orphan, parent process id: %d\n",getppid());
   }
   else
   {
     printf("Parent process id: %d\n",getpid());
     exit(0); 
   }
   return 0;
  }
