/*
============================================================================
Name : 29.c
Author : Samarpita Bhaumik
Description : Write a program to remove the message queue.
Date: 10th Oct,2023
============================================================================
*/
#include <sys/types.h> 
#include <sys/ipc.h>   
#include <sys/msg.h>   
#include <stdio.h>   
#include <unistd.h>    
#include <errno.h>

int main()
{
	int key=ftok(".",'a');
	int Id=msgget(key,0);
	printf("Key: %d\n", key);
    	printf("Message Queue Identifier: %d\n\n", Id);
	int i =  msgctl(Id, IPC_RMID, NULL);
	if(i==-1)
		perror("Error");
	else 
		printf("Sucessfully deleted \n");
	return 0;
}
	 
