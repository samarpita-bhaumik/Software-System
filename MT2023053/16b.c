/*
============================================================================
Name : 16a.c
Author : Samarpita Bhaumik
Description : Write a program to perform mandatory locking.
               b. Implement read lock
Date: 8th September, 2023.
============================================================================
*/
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<stdlib.h>
#include<stdio.h>
int main(int argc, char *argv[])
{
  if(argc!=2)
    {
         printf("You are useless\n");
         return 0;
    }
  struct flock lock;
  int fd;
  fd = open(argv[1],O_RDWR);
  lock.l_type=F_RDLCK;
  lock.l_whence=SEEK_SET;
  lock.l_start=0;
  lock.l_len=0;
  lock.l_pid=getpid();
  printf("Before entering into critical section:\n");
  fcntl(fd,F_SETLKW,&lock);
  printf("Inside the critical section..\n");
  printf("Enter to unlock..\n");
  getchar();
  printf("Unlocked\n");
  lock.l_type=F_UNLCK;
  fcntl(fd,F_SETLK,&lock);
  printf("finish\n");
    }
