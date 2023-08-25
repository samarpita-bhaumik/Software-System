#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdio.h>
#include<unistd.h>
int main(int argc,char *argv[])
{
    if(argc!=2)
    {
         printf("You are useless\n");
         return 0;
    }
    int fd_read = open(argv[1],O_RDONLY);
    if(fd_read == -1)
        {printf("Useless"); return 0;}
    while(1)
    {
     char buf;
     int char_read = read(fd_read,&buf,1);
     if(buf != '\n')
        write(1,&buf,1);
     else 
         printf("\n");
     if(char_read == 0)
       break;
     
    }
    int fd_read_close = close(fd_read);
    if(fd_read_close == -1)
        printf("Totally Useless");
 return 0;
}
