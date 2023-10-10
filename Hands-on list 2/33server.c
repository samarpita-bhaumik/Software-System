/*
============================================================================
Name : 33server.c
Author : Samarpita Bhaumik
Description :Write a program to communicate between two machines using socket.
Date: 10th Oct,2023
============================================================================
*/

#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/ip.h>
#include <stdio.h>      
#include <unistd.h>   

//code for server part
int main()
{
	struct sockaddr_in s,c;
	int sd=socket(AF_UNIX,SOCK_STREAM,0);
	s.sin_family=AF_UNIX;
	s.sin_addr.s_addr=INADDR_ANY; //server and client in the same system
	s.sin_port=htons(7858); //port number=7858
	bind(sd,(struct sockaddr*)&s,sizeof(s));
	listen(sd,5); 
	int size=(int)sizeof(c);
	int nsd=accept(sd,(void*)&c,&size); //accept connections from client
	char buf[100];
	read(nsd,buf,sizeof(buf));
	printf("Message from client: %s\n",buf);
	write(nsd,"Hi Client \n",sizeof("Hi Client \n"));

	close(nsd);
	close(sd);
	return 0;
}
