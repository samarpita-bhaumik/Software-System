/*
============================================================================
Name : 20.c
Author : Samarpita Bhaumik
Description : Write two programs so that both can communicate by FIFO -Use one way communication.
Date: 10th Oct,2023
============================================================================
*/
#include <sys/types.h> 
#include <sys/stat.h>  
#include <fcntl.h>     
#include <unistd.h>    
#include <stdio.h>     

void main()
{
      char *buff="hello";
      int fd;
      char *mkfifoName = "./file10"; 
      mkfifo(mkfifoName,0744);
      fd=open(mkfifoName,O_WRONLY);
      write(fd,&buff,sizeof(buff));
 }
