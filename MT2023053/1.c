/*
============================================================================
Name : 1.c
Author : Samarpita Bhaumik
Description :Create the following types of a files using (ii) system call
             a. soft link (symlink system call)
             b. hard link (link system call)
             c. FIFO (mkfifo Library Function or mknod system call)
Date: 8th September, 2023.
============================================================================
*/
#include<unistd.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
int main()
{
	int l1,l2,l3;
	char *myfifo2 ="./pipe1";//. means present working directory
	l1 = link("file4.txt","hl1");	//hard link
	
	if(l1<0)
	{
		perror("failed to create hardlink");
		return 1;
	}
	l2 = symlink("file4.txt","sl1");	//soft link
	if(l2<0)
	{
		perror("failed to create softlink");
		return 1;
	}
	l3 = mkfifo(myfifo2,0666);
	if(l3<0)
		perror("failed");
	return 0;
} 	
