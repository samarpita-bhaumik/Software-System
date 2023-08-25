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
    
    int fd1 = open(argv[1],O_RDONLY);
    int mode = fcntl(fd1, F_GETFL);
    
    switch(mode)
    {
        case 32768 : printf("r\n"); break;
        case 32769 : printf("w\n"); break;
        case 33793 : printf("a\n"); break;
        case 32770 : printf("r+ || w+\n"); break;
        case 32794 : printf("a+\n"); break;
    }
    int fd_close = close(fd1);
    if(fd_close == -1)
      printf("%d",F_GETFL);
 return 0;
}
