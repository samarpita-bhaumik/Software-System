/*
============================================================================
Name : 25.c
Author : Samarpita Bhaumik
Description : Write a program to create three child processes. The parent should wait for a particular child (use
              waitpid system call).
Date: 8th September, 2023.
============================================================================
*/
#include <unistd.h>    
#include <sys/types.h> 
#include <sys/wait.h>  
#include <stdio.h>     
#include<stdlib.h>
void main()
{
    pid_t A, B, C, t_Child;
    int waitStatus;

    if ((A = fork()) == 0)
    {
        // Child A will enter this branch
        printf("Child A created...\n");
        printf("Putting child A to sleep for 15 sec\n");
        sleep(15);
        printf("Child A is now awake!\n");
        exit(0);
    }
    else
    {
        // Parent will enter this branch
        if ((B = fork()) == 0)
        {
            // Child B will enter this branch
            printf("Child B created...\n");
            printf("Putting child B to sleep for 10 sec\n");
            sleep(10);
            printf("Child B is now awake!\n");
            exit(0);
        }
        else
        {
            // Parent will enter this branch

            if ((C = fork()) == 0)
            {
                // Child C will enter this branch
                printf("Child C created...\n");
                printf("Putting child C to sleep for 5 sec\n");
                sleep(5);
                printf("Child C is now awake!\n");
                exit(0);
            }
            else
            {
                // Parent will enter this branch
                t_Child = waitpid(B, &waitStatus, 0);

                if (t_Child == B)
                {
                    if (waitStatus == 0)
                        printf("Child B has terminated with exit status = 0\n");
                    else
                        printf("Child has exited with exit status : %d\n", waitStatus);
                }
            }
        }
    }
}
