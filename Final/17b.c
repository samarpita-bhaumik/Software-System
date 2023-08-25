#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdio.h>
#include<sys/file.h>
#include<unistd.h>

int main(int argc,char *argv[])
{
    struct s
    {
      int ticket_no;
     }db;
     
     db.ticket_no = 1;
     struct flock lock;
     int fd;
     fd = open("s17.txt",O_RDWR);
     //write lock
     lock.l_type = F_WRLCK;
     lock.l_whence=SEEK_SET;
     lock.l_start=0;
     lock.l_len=0;
     lock.l_pid=getpid();
     printf("Before entering into critical section:\n");
     fcntl(fd,F_SETLKW,&lock);
     printf("Inside the critical section..\n");
     read(fd,&db,sizeof(db));
     printf("Current ticket number:%d\n",db.ticket_no);
     db.ticket_no++;
     lseek(fd,0,SEEK_SET);//bring the read file pointer at the beginning
     write(fd,&db,sizeof(db));
     
     printf("Enter to unlock..\n");
     getchar();
     printf("Unlocked\n");
     lock.l_type=F_UNLCK;
     fcntl(fd,F_SETLK,&lock);
     printf("finish\n");
     return 0;
 }
     
