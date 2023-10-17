/*
============================================================================
Name : 18a.c
Author : Samarpita Bhaumik
Description :Write a program to perform Record locking.
             a. Implement write lock
Date: 8th September, 2023.
============================================================================
*/
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdio.h>
#include<sys/file.h>
#include<unistd.h>

int main()
{
  struct 
  {
    int ticket_no;
    int train_no;
  }db;
  struct flock lock;
  int i;
  printf("\n Enter the train number to book ticket(1,2,3):");
  scanf("%d",&i);
  if(i>3)
  {
    printf("\nOut of range");
    return 0;
  }
  lock.l_type = F_WRLCK;
  lock.l_whence=SEEK_SET;
  lock.l_start=(i-1)*sizeof(db);
  lock.l_len=sizeof(db);
  lock.l_pid=getpid();
  
  int fd = open ("train.txt",O_RDWR);
  if(fd==-1)
  {
    printf("Error");
    return 0;
  }
  //start write lock
  printf("\n Before entering into critical section:");
  fcntl(fd,F_SETLKW,&lock);
  printf("\n Inside critical section:");
  lseek(fd,(i-1)*sizeof(db),SEEK_SET);
  read(fd,&db,sizeof(db));
  printf("\n Current ticket number: %d.",db.ticket_no);
  db.ticket_no++;
  printf("\n Press enter to exit the critical section:");
  getchar();
  getchar();
  lseek(fd,(i-1)*sizeof(db),SEEK_SET);
  write(fd,&db,sizeof(db));
  lock.l_type= F_UNLCK;
  printf("\n Exited the critical section.");
  fcntl(fd,F_SETLK,&lock);
  printf("\n Book ticket as:%d.\n",db.ticket_no);
  close(fd);
  return 0;
  }
  
