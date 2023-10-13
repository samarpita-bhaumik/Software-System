#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "structures.h"

#define MAX_CLIENTS 100
#define MAX_MESSAGE_SIZE 4096

// Read Write Block
/*-----------------------------------------------------------------*/

int rd(int client_socket, char *buffer,int len) {
    memset(buffer,0,len);
    int bytes_received = read(client_socket, buffer, len);
        if (bytes_received <= 0) {
             return 0;
        }
    buffer[bytes_received] = '\0';
}

void wr(int client_socket, const char *buffer, int len) {
    int b = write(client_socket,buffer,len);
}

/*-----------------------------------------------------------------*/


//Set and Get and Count Block
/*-----------------------------------------------------------------*/


int set_count(int type){
    int fd = open("records/account_count",O_RDWR);
    if (fd == -1) {
        exit(1);
    }
    int offset=lseek(fd,0*sizeof(struct record),SEEK_SET);
    if(offset != -1){
        // Implementing write lock

            struct flock lock;
            lock.l_type = F_WRLCK;
            lock.l_whence = SEEK_SET;
            lock.l_start = 0;
            lock.l_len = 0;
            lock.l_pid = getpid();
            int f = fcntl(fd, F_SETLKW, &lock);
            if(f==-1){
                perror("Error obtaining write lock on Count Record!");
                close(fd);
            }
            struct record rec;
            read(fd,&rec,sizeof(rec));
            lseek(fd,0*sizeof(struct record),SEEK_SET);
            
            if(type == 1)
                rec.student++;
            else if(type == 2)
                rec.teacher++;
            else if(type == 3) 
                rec.courses++;
            else rec.enrollments++;
            
            write(fd,&rec,sizeof(rec));
            // Unlocking
            lock.l_type = F_UNLCK;
            fcntl(fd,F_SETLK,&lock);
            close(fd);
            if(type == 1)
                return rec.student;
            else if(type == 2)
                return rec.teacher;
            else if(type == 3) 
                return rec.courses;
            else return rec.enrollments;
    }
}

int get_count(int type){
    int fd = open("records/account_count",O_RDONLY);
    if (fd == -1) {
        exit(1);
    }
    int offset=lseek(fd,0*sizeof(struct record),SEEK_SET);
    if(offset != -1){
        // Implementing write lock

            struct flock lock;
            lock.l_type = F_RDLCK;
            lock.l_whence = SEEK_SET;
            lock.l_start = 0;
            lock.l_len = 0;
            lock.l_pid = getpid();
            int f = fcntl(fd, F_SETLKW, &lock);
            if(f==-1){
                perror("Error obtaining Read lock on Count Record!");
                close(fd);
            }
            struct record rec;
            read(fd,&rec,sizeof(rec));
            // Unlocking
            lock.l_type = F_UNLCK;
            fcntl(fd,F_SETLK,&lock);
            close(fd);
            if(type == 1)
                return rec.student;
            else if(type == 2)
                return rec.teacher;
            else if(type == 3) 
                return rec.courses;
            else return rec.enrollments;           
    }
}

/*---------------------------------------------------------------------------------*/


//Checking if buffer content is number or not
/*---------------------------------------------------------------------------------*/

bool isnum(const char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isdigit(str[i])) {
            return false;
        }
    }
    return true;
}


/*---------------------------------------------------------------------------------*/


// Default Admin Cred Setup to control the system
/*---------------------------------------------------------------------------------*/

void setAdminCreds(){
    struct AdminCredentials admin;
    strcpy(admin.username,"Admin");
    strcpy(admin.password,"Admin");

    // Open the flat file in write mode
    int fd = open("records/admin_cred", O_RDWR);

    int bytes_written = write(fd, &admin, sizeof(struct AdminCredentials));

    if (bytes_written == -1) {
        perror("Failed to write to the file");
        close(fd);
        exit(1);
    }

    // Close the file
    close(fd);
    
    // Setting initial value of account counts as 0
    fd = open("records/account_count", O_RDWR);
    struct record rec={0,0,0,0};
    bytes_written = write(fd, &rec, sizeof(rec));

    if (bytes_written == -1) {
        perror("Failed to write to the file");
        close(fd);
        exit(1);
    }

    // Close the file
    close(fd);
    
}



/*---------------------------------------------------------------------------------*/


// Starting point of execution
/*---------------------------------------------------------------------------------*/

int main() {
    setAdminCreds();
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    char buffer[MAX_MESSAGE_SIZE];

    int client_count = 0;

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set up server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8000);

    // Bind socket to address
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Socket binding failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_socket, MAX_CLIENTS) == -1) {
        perror("Listening failed");
        exit(EXIT_FAILURE);
    }
    printf("Server listening on port 8000...\n");

     while (1) {
        // Accept a new connection
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_socket == -1) {
            perror("Accepting failed");
            continue;
        }
        // Fork a new process to handle the client
        pid_t child_pid = fork();
        if (child_pid == -1) {
            perror("Fork failed");
            close(client_socket);
            continue;
        }


        if (child_pid == 0) {
            // Child process
            close(server_socket); // Close the server socket in the child process
            /*if (login(client_socket)==0){
                exit(EXIT_FAILURE);
            }
            else{
                exit(EXIT_SUCCESS);
            }*/
        } else {
            // Parent process
            close(client_socket); // Close the client socket in the parent process
            client_count++;
            if (client_count >= MAX_CLIENTS) {
                printf("Maximum number of clients reached. Rejecting new connections.\n");
                close(client_socket);
            }
        }
    }

    // Close the server socket (not reached in this example)
    close(server_socket);

    return 0;
}

/*---------------------------------------------------------------------------------*/

