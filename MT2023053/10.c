/*
============================================================================
Name : 10.c
Author : Samarpita Bhaumik
Description :Write a program to open a file with read write mode, write 10 bytes, move the file pointer by 10
             bytes (use lseek) and write again 10 bytes.
             a. check the return value of lseek
             b. open the file with od and check the empty spaces in between the data.
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
    int fd = open(argv[1],O_RDWR,0777);
    int c=1;
    if(fd == -1 )
        {printf("Useless"); return 0;}
        
     while(c<=10)
     {
     char buf;
     read(0,&buf,1);
     write(fd,&buf,1);
     c++;
    }
    
    off_t curr = lseek(fd,10,SEEK_CUR);
    if(curr==(off_t)-1)
    {
        printf("lseek failed");
        return 0;
    }
    c=1;
    while(c<=10)
     {
     char buf;
     read(0,&buf,1);
     write(fd,&buf,1);
     c++;
    }
    int fd_close = close(fd);
    
    if(fd_close == -1)
        printf("Totally Useless");
    system("od -c s1.txt");
 return 0;
}
