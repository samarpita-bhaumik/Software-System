/*
============================================================================
Name : 30.c
Author : Samarpita Bhaumik
Description : 30. Write a program to create a shared memory.
 	a. write some data to the shared memory
 	b. attach with O_RDONLY and check whether you are able to overwrite.
	c. detach the shared memory
	d. remove the shared memory

Date: 10th Oct,2023
============================================================================
*/

#include <sys/types.h> 
#include <sys/ipc.h>   
#include <sys/shm.h> 
#include <unistd.h>    
#include <stdio.h> 

int main()
{
	 char* data;
	 int key=ftok(".",'b');
	 int id=shmget(key,100,IPC_CREAT|0766);
	 data=shmat(id, (void *)0, 0);
	 printf("Enter data to write to shared memory \n");
	 scanf("%[^\n]",data);
	 printf("Press enter to read from the shared memory ");
    	 getchar();
    	 getchar();
	 data = shmat(id, (void *)0, SHM_RDONLY);
	 printf("%s\n",data);
	 printf("Press enter to delete shared memory");
	 getchar();
	 shmdt(data);
	 shmctl(id , IPC_RMID, NULL);
	 return 0;
}
