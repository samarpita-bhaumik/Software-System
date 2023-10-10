/*
============================================================================
Name : 25.c
Author : Samarpita Bhaumik
Description : 25. Write a program to print a message queue's (use msqid_ds and ipc_perm structures)
a. access permission
b. uid, gid
c. time of last message sent and received
d. time of last change in the message queue
d. size of the queue
f. number of messages in the queue
g. maximum number of bytes allowed
h. pid of the msgsnd and msgrcv
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
int msg_qstat=  msgctl(id, IPC_STAT, &mq);//Place the current value of each member of the msqid_ds data structure associated with "id" into the structure pointed to by mq.	 
printf("Access Permission: %d\n", mq.msg_perm.mode);
printf("UID: %d\n", mq.msg_perm.uid);
printf("GID: %d\n", mq.msg_perm.gid);
printf("Time of last message sent: %ld\n", mq.msg_stime);
printf("Time of last message received: %ld\n", mq.msg_rtime);
printf("Size of queue: %ld\n", mq.__msg_cbytes);
printf("Number of messages in the queue: %ld\n", mq.msg_qnum);
printf("Maximum number of bytes allowed in the queue: %ld\n", mq.msg_qbytes);
printf("PID of last sent message: %d\n", mq.msg_lspid);
printf("PID of last received message: %d\n", mq.msg_lrpid);
return 0;
}
