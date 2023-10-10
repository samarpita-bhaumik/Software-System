/*
============================================================================
Name : 32b.c
Author : Samarpita Bhaumik
Description :Write a program to implement semaphore to protect any critical section.
	     b. protect shared memory from concurrent write access
	
Date: 10th Oct,2023
============================================================================
*/

#include <sys/ipc.h>   
#include <sys/sem.h>   
#include <sys/types.h> 
#include <sys/shm.h>   
#include <unistd.h>    
#include <stdio.h>     

int main()
{
  
    
    char *data;
    union semun
    {
        int val;              
    } sem;

    key_t key_sem = ftok(".", 'g');
    int id_sem = semget(key_sem, 1, 0); // Get the semaphore if it exists
    if (id_sem == -1)
    {
        id_sem = semget(key_sem, 1, IPC_CREAT | 0744); // Create a new semaphore
        sem.val = 1; // binary semaphore
        semctl(id_sem, 0, SETVAL, sem);
    }
   //shared memory
    key_t key_shm = ftok(".", 'b');
    int id_shm = shmget(key_shm, 50, IPC_CREAT | 0744); //create a shared memory
    data=shmat(id_shm, (void *)0, 0);
    struct sembuf s; 
    s.sem_num = 0;
    s.sem_flg = 0;
    s.sem_op = -1;
    printf("\nWaiting for lock:\n");
    semop(id_sem, &s, 1);
    printf("Critical Section(shared memory) is now locked:\n");
    printf("Writing to the shared memory:\n");
    printf("Enter data to write to shared memory:\n");
    scanf("%[^\n]",data);
    printf("Press enter to read from the shared memory:\n");
    getchar();

    // Reading from the shared memory
    printf("%s\n", data);

    printf("Press enter to exit the critical section!\n");
    getchar();
    s.sem_op = 1;
    semop(id_sem, &s, 1);
    return 0;
}
