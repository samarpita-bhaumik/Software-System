/*
============================================================================
Name : 15.c
Author : Samarpita Bhaumik
Description : Write a program to display the environmental variable of the user (use environ).
Date: 8th September, 2023.
============================================================================
*/
#include<unistd.h>
#include<stdio.h>
extern char **environ;
int main(){
int i=0;
while(environ[i])
   printf("%s\n",environ[i++]);
   
 }  
   
