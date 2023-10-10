/*
============================================================================
Name : 34_a_server.c
Author : Samarpita Bhaumik
Description :Write a program to create a concurrent server.
	a. use fork
	b. use pthread_create
Date: 10th Oct,2023
============================================================================
*/
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/ip.h>
#include <stdio.h>      
#include <unistd.h>   
#include<stdlib.h>
void main()
{
	int sd=socket(AF_UNIX, SOCK_STREAM, 0);
	struct sockaddr_in s,c;

	s.sin_addr.s_addr = INADDR_ANY;
    	s.sin_family = AF_UNIX;
    	s.sin_port = htons(7000);
	bind(sd, (struct sockaddr*)&s, sizeof(s));

	listen(sd,2);
	printf("server listening \n" );
	while(1)
	{
		int size=(int)sizeof(c);
		int nsd=accept(sd,(struct sockaddr*)&c,&size);
		printf("Server to client Connection successfully established\n");
		if(!fork())
		{	close(sd);
			char data[100];
        		char send[20]="This is the SERVER:";
        		write(nsd, &send, 20);
        		read(nsd, &data, 100);
        		printf("%s: %d\n",data,getpid());
			exit(0);						
		}
		close(nsd);

	}
	close(sd);
	//return 0;
}
