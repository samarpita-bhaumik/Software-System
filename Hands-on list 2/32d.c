/*
============================================================================
Name : 32d.c
Author : Samarpita Bhaumik
Description :Write a program to implement semaphore to protect any critical section.
	     d. remove the created semaphore
Date: 10th Oct,2023
============================================================================
*/

#include <sys/types.h> 
#include <sys/ipc.h>   
#include <sys/sem.h> 
#include <unistd.h>   
#include <stdio.h>
int main()
{

    key_t key = ftok(".", 'z');
    int id = semget(key, 1, IPC_CREAT | 0744);
    printf("Press enter to delete the semaphore!\n");
    getchar();
    semctl(id, 0, IPC_RMID);
    return 0;
}
