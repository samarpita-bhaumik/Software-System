/*
============================================================================
Name : 23.c
Author : Samarpita Bhaumik
Description : Write a program to create a Zombie state of the running program.
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
      printf("Child process id: %d\n",getpid());
      printf("Child exits.\n");
      exit(0); 
   }
   else
   {
     printf("Parent process id: %d\n",getpid());
     printf("Putting parent to sleep for 50 sec.\n");
     sleep(50);
     printf("Parent is awake."); 
     
   }
   return 0;
  }
