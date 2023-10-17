/*
============================================================================
Name : 4b.c
Author : Samarpita Bhaumik
Description :Write a program to open an existing file with read write mode.Try O_EXCL flag also.
Date: 8th September, 2023.
============================================================================
*/
#include<stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<unistd.h>
#include<errno.h>
int main()
{
	int fd;
	fd = open("file.txt",O_RDWR|O_CREAT|O_EXCL,0744);
	
	if(fd==-1)
	{
		if(errno==EEXIST)
			printf("file already exists\n");
		
		return 1;
	}
	printf("opened file %d\n",fd);
	close(fd);
	return 0;
}
