/*
============================================================================
Name : 4a.c
Author : Samarpita Bhaumik
Description :Write a program to open an existing file with read write mode.
Date: 8th September, 2023.
============================================================================
*/
#include<stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<unistd.h>

int main()
{
	int fd;
	fd = open("file.txt",O_CREAT|O_RDWR,0744);
	if(fd==-1)
	{
		perror("file doesn't exist");
		return 1;
	}
	printf("open and read : %d\n",fd);
	close(fd);
	return 0;
}
