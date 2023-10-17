/*
============================================================================
Name : 11c.c
Author : Samarpita Bhaumik
Description :Write a program to open a file, duplicate the file descriptor and append the file with both the
             descriptors and check whether the file is updated properly or not.
             c. use fcntl
Date: 8th September, 2023.
============================================================================
*/
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdio.h>
#include<unistd.h>
int main(int argc,char *argv[])
{
    if(argc!=2)
    {
         printf("You are useless\n");
         return 0;
    }
    int fd1 = open(argv[1],O_RDWR|O_APPEND,0666);
    
    if(fd1 == -1)
        {printf("Useless"); return 0;}
    
    //Dupicating the file descriptor
    int fd2 = fcntl(fd1,F_DUPFD,5);
    
    printf("fd1 = %d , fd2 = %d \n",fd1,fd2);
    system("ls -l /proc/$$/fd");
    printf("fd1:\n");
    //Writing data to file using fd1 descriptor
     while(1)
     {
     char buf;
     int char_read = read(0,&buf,1);
     if(char_read ==0)
      break;
     write(fd1,&buf,1);
    }
    
    int fd_close = close(fd1);
    
    printf("fd2:\n");
   //Writing data to file using fd2 descriptor 
   while(1)
     {
     char buf;
     int char_read = read(0,&buf,1);
     if(char_read ==0)
      break;
     write(fd2,&buf,1);
    }
    
    
    int fd_close2 = close(fd2);
    if((fd_close == -1)||(fd_close2 == -1))
        printf("Totally Useless");
        
    int fd3 = open(argv[1],O_RDWR);
    
    if(fd3 == -1)
        {printf("Useless"); return 0;}
        
    printf("fd3:\n");
    while(1)
     {
     char buf;
     int char_read = read(fd3,&buf,1);
     if(char_read ==0)
      break;
     write(1,&buf,1);
    }
    int fd_close3 = close(fd3);
    if(fd_close3 == -1)
        printf("Totally Useless");
 return 0;
}
