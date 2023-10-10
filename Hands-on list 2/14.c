/*
============================================================================
Name : 14.c
Author : Samarpita Bhaumik
Description : Write a simple program to create a pipe, write to the pipe, read from pipe and display on
              the monitor.
Date: 10th Oct,2023
============================================================================
*/

#include <unistd.h> 
#include <stdio.h>  

void main()
{
    // File descriptor used for pipe operations
    int pipefd[2];                
    char *write_Buff = "Samarpita"; 
    char *read_Buff;
    int pipeStat;            
    int readBytes, writeBytes; 

    // Creating a pipe
    pipeStat = pipe(pipefd);

    if (pipeStat == -1)
        perror("Error while creating the pipe! ");
    else
    {
        printf("Writing to the pipe!\n");
        writeBytes = write(pipefd[1], &write_Buff, sizeof(write_Buff));
        if (writeBytes == -1)
            perror("Error while writing to pipe!");
        else
        {
            printf("Now reading from the pipe!\n");
            readBytes = read(pipefd[0], &read_Buff, writeBytes);
            if (readBytes == -1)
                perror("Error while reading from pipe!");
            else
                printf("Data from pipe: %s\n", read_Buff);
        }
    }
}
