#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
int main(int argc,char *argv[])
{
    if(argc!=2)
    {
         printf("You are useless\n");
         return 0;
    }
   
    
    struct stat file_info;
    stat(argv[1],&file_info);
    if(S_ISREG(file_info.st_mode))
     printf("Regular File\n");
    else if(S_ISDIR(file_info.st_mode))
     printf("Directory\n");
    else
      printf("Some other type of file");
 return 0;
}
