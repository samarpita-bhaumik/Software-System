/*
============================================================================
Name : 6.c
Author : Samarpita Bhaumik
Description :Write a program to take input from STDIN and display on STDOUT. Use only read/write system calls
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
  while(1)
    {
     char buf;
     int char_read = read(0,&buf,1);
     if(char_read ==0)
      break;
     write(1,&buf,1);
    }
   return 0;
}
