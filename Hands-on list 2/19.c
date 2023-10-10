/*
============================================================================
Name : 19.c
Author : Samarpita Bhaumik
Description : Create a FIFO file by
              a. mknod command
              b. mkfifo command
              c. use strace command to find out, which command (mknod or mkfifo) is better.
              d. mknod system call
              e. mkfifo library function
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
    char *mkfifoName = "./file5";    // File name of FIFO file created using `mkfifo`
    char *mknodName = "./file6"; // File name of FIFO file created using `mknod`

    int mkfifo_status, mknod_status; // 0 -> Success, -1 -> Error

    // Using `mkfifo` library function
    mkfifo_status = mkfifo(mkfifoName,0744);

    if (mkfifo_status == -1)
        perror("Error while creating FIFO file!");
    else
        printf("Succesfully created FIFO file. Check using `ll` or `ls -l` command!\n");

    // Using `mknod` system call
    mkfifo_status = mknod(mknodName,S_IFIFO|0744, 0);

    if (mknod_status == -1)
        perror("Error while creating FIFO file!");
    else
        printf("Succesfully created FIFO file. Check using `ll` or `ls -l` command!\n");
}
