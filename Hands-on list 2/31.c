/*
============================================================================
Name : 31a.c
Author : Samarpita Bhaumik
Description : Write a program to create a semaphore and initialize value to the semaphore.
	      a. create a binary semaphore

Date: 10th Oct,2023
============================================================================
*/

#include<sys/types.h> 
#include<sys/ipc.h>   
#include<sys/sem.h>   
#include<unistd.h>    
#include<stdio.h>    

int main()
{
    key_t key = ftok(".",'d');
    if(key==-1)
    {
    	perror("Error :");
    	return -1;
    }
    int id = semget(key, 1, IPC_CREAT | 0777);
    if(id == -1) {
        perror("Error :");
	return -1;
    }
    
    union semun {
        int val; 
    } sem;
    
    sem.val = 1; //binary semaphore

    int stat = semctl(id, 0, SETVAL, sem); 

	if(stat == -1) {
        	perror("Error :");
		return -1;
    	}
    return 0;
   }
