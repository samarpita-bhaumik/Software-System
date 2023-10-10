/*
============================================================================
Name : 28.c
Author : Samarpita Bhaumik
Description : Write a program to change the existing message queue permission. (use msqid_ds structure)
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
	struct msqid_ds mq;
	key_t key=ftok(".",'a');
	int id=msgget(key,0);
	printf("Key: %d\n", key);
    	printf("Message Queue Identifier: %d\n", id);
	msgctl(id, IPC_STAT, &mq);
	printf("Message queue permission:%d\n",mq.msg_perm.mode);
	mq.msg_perm.mode=0666;
	msgctl(id, IPC_SET, &mq);
	printf("Message queue permission:%d\n",mq.msg_perm.mode);
	return 0;
}



