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
     int fd;
     //create the file if it doesn't exist to store the ticket number
     fd = open("s17.txt",O_CREAT|O_RDWR,0744);
     if(fd == -1)
     {
        perror("error in opening file");
        return 0;
     }
     lseek(fd,0,SEEK_SET);
     write(fd,&db,sizeof(db));
     close(fd);
     
     //store the ticket number to struct db from file
     fd = open("s17.txt",O_RDONLY);
     lseek(fd,0,SEEK_SET);
     read(fd,&db,sizeof(db));
     printf("Ticket no:%d\n",db.ticket_no);
     close(fd);
     return 0;
 }
     
       
