/*
============================================================================
Name : 27d.c
Author : Samarpita Bhaumik
Description : Write a program to execute ls -Rl by the following system calls
              d. execv
Date: 8th September, 2023.
============================================================================
*/
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
int main() {
    pid_t child_pid;
    char *argv[]={"ls", "-Rl", NULL};
    child_pid = fork();
    if (child_pid == -1) {
        perror("fork");
        return 1;
    }
    if (child_pid == 0) {
      execv("/usr/bin/ls", argv);
      perror("execv");
        return 1;
    } 
    else {
        int status;
        waitpid(child_pid, &status, 0);
        if (status == 0)
            printf("Child has terminated with exit status = 0\n");
        else
            printf("Child has exited with exit status : %d\n",status);
    }
    return 0;
}

