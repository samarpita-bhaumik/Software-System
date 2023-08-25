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
  lock.l_type = F_RDLCK;
  lock.l_whence=SEEK_SET;
  lock.l_start=(i-1)*sizeof(db);
  lock.l_len=sizeof(db);
  lock.l_pid=getpid();
  
  int fd = open ("train.txt",O_RDONLY);
  if(fd==-1)
  {
    printf("Error");
    return 0;
  }
  //start read lock
  printf("\n Before entering into critical section:");
  fcntl(fd,F_SETLKW,&lock);
  printf("\n Inside critical section:");
  lseek(fd,(i-1)*sizeof(db),SEEK_SET);
  read(fd,&db,sizeof(db));
  printf("\n Current ticket number: %d.",db.ticket_no);
  printf("\n Press enter to exit the critical section:");
  getchar();
  getchar();
  lock.l_type= F_UNLCK;
  printf("\n Exited the critical section.\n");
  fcntl(fd,F_SETLK,&lock);
  close(fd);
  return 0;
  }
  
