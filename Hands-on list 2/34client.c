/*
============================================================================
Name : 34client.c
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

//code for client
int main() {
	struct sockaddr_in s;
	int sd;
	char buf[80];
	sd = socket(AF_UNIX, SOCK_STREAM, 0);
	s.sin_family = AF_UNIX;
	s.sin_addr.s_addr = INADDR_ANY;
	s.sin_port = htons(7000); // Must be same port as specified in server
	// Connect client socket sd to server
	int con = connect(sd, (struct sockaddr*) (&s), sizeof(s)); 
	if(con==-1) perror("error in connect:");
	// Send message to server
	int wb = write(sd, "Hello Server \n", sizeof("Hello Server \n"));
	if(wb==-1) perror("error in write()");
	//read message from server
	int rb = read(sd, buf, sizeof(buf));
	if(rb==-1) perror("error in read()");
	printf("Message from server: %s\n", buf);
	close(sd);
	return 0;
}
