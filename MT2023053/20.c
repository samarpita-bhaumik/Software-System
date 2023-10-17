/*
============================================================================
Name : 20.c
Author : Samarpita Bhaumik
Description : Find out the priority of your running program. Modify the priority with nice command.
Date: 8th September, 2023.
============================================================================
*/

#include<stdio.h>
#include<unistd.h>
#include<sys/time.h>
#include<sys/resource.h>
#include<stdlib.h>
int main()
{
  int niceV;
  int pid=getpid();
  niceV=getpriority(PRIO_PROCESS,pid);
  printf("pid : %d, nice: %d\n",pid,niceV);
  getchar();
  nice(5);
  niceV=getpriority(PRIO_PROCESS,0);
  printf("pid : %d, nice: %d\n",pid,niceV);
  while(1);
  return 0;
}
