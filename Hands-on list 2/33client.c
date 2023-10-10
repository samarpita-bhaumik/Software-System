/*
============================================================================
Name : 33client.c
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

//code for client
int main() {
struct sockaddr_in s;
int sd;
char buf[80];
sd = socket(AF_UNIX, SOCK_STREAM, 0);//Normally only a single protocol exists to support a particular socket type within a given protocol family, in which case protocol can be specified as 0.
s.sin_family = AF_UNIX;
s.sin_addr.s_addr = INADDR_ANY;////server and client in the same system
s.sin_port = htons(7858); // Must be same port as specified in server
// Connect client socket sd to server
connect(sd, (void*) (&s), sizeof(s)); 
// Send message to server
write(sd, "Hello Server \n", sizeof("Hello Server \n"));
read(sd, buf, sizeof(buf));
printf("Message from server: %s\n", buf);
close(sd);
return 0;
}
