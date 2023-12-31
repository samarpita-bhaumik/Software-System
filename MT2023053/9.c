/*
============================================================================
Name : 9.c
Author : Samarpita Bhaumik
Description :Write a program to print the following information about a given file.
              a. inode
              b. number of hard links
              c. uid
              d. gid
              e. size
              f. block size
              g. number of blocks
              h. time of last access
              i. time of last modification
              j. time of last change
Date: 8th September, 2023.
============================================================================
*/
#include<stdio.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#define st_atime st_atim.tv_sec      
#define st_mtime st_mtim.tv_sec
#define st_ctime st_ctim.tv_sec
    
int main(int argc, char* argv[])
{
	struct stat s;
	stat("s1.txt",&s);
	printf("\n inode number: %ld",s.st_ino);
	printf("\n number of hard links: %ld",s.st_nlink);
	printf("\n uid: %u",s.st_uid);
	printf("\n gid: %u",s.st_gid);
	printf("\n size: %ld",s.st_size);
	printf("\n block size: %lu",s.st_blksize);
	printf("\n number of blocks: %lu",s.st_blocks);
	printf("\n time of last access: %s",ctime(&s.st_atim));
	printf("\n time of last modification: %s",ctime(&s.st_mtim));
	printf("\n time of last change: %s",ctime(&s.st_ctim));
	return 0;
}
