/*
============================================================================
Name : 21a.c
Author : Samarpita Bhaumik
Description : Write two programs so that both can communicate by FIFO -Use two way communications
Date: 10th Oct,2023
============================================================================
*/
#include<unistd.h>
#include<stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() {
char buff1[100],buff2[100];
int fd1,fd2;
fd1 = open("./file5", O_WRONLY);
fd2 = open("./file6", O_RDONLY);
printf("Enter the text:\n");
scanf(" %[^\n]", buff1);
write(fd1, buff1, sizeof(buff1));
read(fd2, buff2, sizeof(buff2));
printf("The text from FIFO file: %s\n", buff2);
return 0;
}
