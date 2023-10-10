/*
============================================================================
Name : 20b.c
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
      char buff[100];
      int fd;
      fd=open("file10",O_WRONLY);
      read(fd,&buff,sizeof(buff));
      printf("The text from FIFO file:%s\n",buff);
}
