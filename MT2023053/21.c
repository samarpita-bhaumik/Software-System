/*
============================================================================
Name : 21.c
Author : Samarpita Bhaumik
Description : Write a program, call fork and print the parent and child process id.
Date: 8th September, 2023.
============================================================================
*/
#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>
int main()
{
  int p=fork();
  if(p)
  {
     wait(0);
     printf("Parent process: %d\n", getpid());
  }
  else 
      printf("Child process: %d\n", getpid());
  
  }
