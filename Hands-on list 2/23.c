/*
============================================================================
Name : 23.c
Author : Samarpita Bhaumik
Description : Write a program to print the maximum number of files can be opened within a process and
	      size of a pipe (circular buffer).
Date: 10th Oct,2023
============================================================================
*/

#include<unistd.h>
#include<stdio.h>
int main()
{
long PIPE_BUF,OPEN_MAX;
PIPE_BUF=pathconf(".",_PC_PIPE_BUF); //returns the size of the pipe buffer, where path must refer to a pipe or FIFO 
OPEN_MAX=sysconf(_SC_OPEN_MAX); //The maximum number of files that a process can have open at any time
printf("PIPE_BUF= %ld\t OPEN_MAX= %ld\n",PIPE_BUF,OPEN_MAX);
return 0;
}
