/*
============================================================================
Name : 32c.c
Author : Samarpita Bhaumik
Description :Write a program to implement semaphore to protect any critical section.
	     c. protect multiple pseudo resources ( may be two) using counting semaphore
	
Date: 10th Oct,2023
============================================================================
*/

#include<sys/ipc.h>   
#include<sys/sem.h>   
#include<sys/types.h> 
#include<sys/stat.h>  
#include<fcntl.h>     
#include<unistd.h>    
#include<stdio.h>     

int main()
{	
    char buf[100];
    int fd = open("./file.txt", O_RDONLY);
    union semun
    {
        int val;               
    } sem;

    key_t key = ftok(".", 'z');
    int  id = semget(key, 1, IPC_CREAT | 0744); 
    sem.val = 2; // Set a counting semaphore
    semctl(id,0, SETVAL, sem);
    

    struct sembuf s; 
    s.sem_num = 0;
    s.sem_flg = IPC_NOWAIT;
    s.sem_op = 2;
    printf("Waiting for the lock \n");
    semop(id, &s, 1);
    
    // =========== Inside of Critical Section =============
    printf("Inside critical section \n");

    int i = read(fd, &buf, sizeof(buf));
    if (i == 0) // Sample file has no data
        printf("No data exists!\n");
    else if(i!=-1)
        printf("Data: \n %s", buf);

    printf("Press enter to exit critical section!");
    getchar();

    // =========== End of Critical Section =============

    // Use semaphore to unlock the critical section
    s.sem_op = 1;
    semop(id, &s, 1);
    close(fd);
    return 0;
}
