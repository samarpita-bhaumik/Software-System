/*
============================================================================
Name : 29.c
Author : Samarpita Bhaumik
Description : Write a program to get scheduling policy and modify the scheduling policy (SCHED_FIFO,
SCHED_RR).
Date: 8th September, 2023.
============================================================================
*/
#include <sched.h> 
#include <sys/types.h> 
#include <unistd.h>    
#include <stdio.h>     
void main()
{
    int c_Policy;
    c_Policy = sched_getscheduler(27);
    struct sched_param priority;
    priority.sched_priority = 27;
    printf("Current policy is %d\n", c_Policy);
    switch (c_Policy)
    {
    case SCHED_FIFO:
        printf("Current policy is FIFO\n");
        sched_setscheduler(27, SCHED_RR, &priority);
        c_Policy = sched_getscheduler(27);
        printf("Current policy is %d\n", c_Policy);
        break;
    case SCHED_RR:
        printf("Current policy is RR\n");
        sched_setscheduler(27, SCHED_FIFO, &priority);
        c_Policy = sched_getscheduler(27);
        printf("Current policy is %d\n", c_Policy);
        break;
    case SCHED_OTHER: 
        printf("Current policy is OTHER\n");
        sched_setscheduler(27, SCHED_FIFO, &priority);
        c_Policy = sched_getscheduler(27);
        printf("Current policy is %d\n", c_Policy);
        break;
    default:
        perror("Error while getting current policy\n");
    }
}
