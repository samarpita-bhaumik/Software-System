/*
============================================================================
Name : 5.c
Author : Samarpita Bhaumik
Description :Write a program to create five new files with infinite loop. Execute the program in the background
             and check the file descriptor table at /proc/pid/fd.
Date: 8th September, 2023.
============================================================================
*/
#include<unistd.h>
#include<stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
int main()
{	
	char *filename[5] = {"file6.txt","file2.txt","file3.txt","file4.txt","file5.txt"};
	
	for(int i=0;i<5 ;i++)
	{
		open(filename[i],O_CREAT|O_RDWR,0744);	
	}	
	while(1);
	return 0;
}
