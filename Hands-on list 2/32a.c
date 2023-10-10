/*
============================================================================
Name : 32a.c
Author : Samarpita Bhaumik
Description :Write a program to implement semaphore to protect any critical section.
	     a. rewrite the ticket number creation program using semaphore
	
Date: 10th Oct,2023
============================================================================
*/

#include <sys/ipc.h>   
#include <sys/sem.h>   
#include <sys/types.h> 
#include <sys/stat.h>  
#include <fcntl.h>     
#include <unistd.h>   
#include <stdio.h>   

int main()
{
    
    int tno;

    
    int fd = open("./sam.txt", O_CREAT | O_RDWR, S_IRWXU);// Create a new ticket file / open an exisiting one
    union semun
    {
        int val;             
    } sem;

    key_t key = ftok(".", 'a');
    int id  = semget(key, 1, 0); //if semaphore exists
    if (id == -1)
    {
        id = semget(key, 1, IPC_CREAT | 0744); // Create a new semaphore if doesn't exists
        sem.val = 1; // Set a binary semaphore
        semctl(id, 0, SETVAL, sem);

    }
	
    struct sembuf s; // Defines the operation on the semaphore
    s.sem_num = 0; //default behaviour
    s.sem_flg = 0;
    s.sem_op = -1; //obtain lock
    printf("Waiting to enter critical section \n");
    semop(id, &s, 1);
    printf("Inside critical section \n");
    int i = read(fd, &tno, sizeof(tno));
    if (i == 0) // Ticket file has no data
        tno = 1;
    else
    {
        tno += 1;
        lseek(fd, 0, SEEK_SET);
    }

    write(fd, &tno, sizeof(tno));
    
    printf("Ticket number is - %d\n", tno);

    printf("Press enter to exit from critical section \n");
    getchar();

    s.sem_op = 1;
    semop(id, &s, 1);
    close(fd);
}
