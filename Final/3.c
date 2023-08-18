#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
int main() {
    int file_desc;
    // Create a file using creat() system call
    file_desc = creat("example.txt", 0777);
    if (file_desc == -1) {
        perror("Error creating file");
        return 1;
    }
    // Print the file descriptor value
    printf("File Descriptor: %d\n", file_desc);
    printf("File Descriptor Table:\n");
    system("ls -l /proc/$$/fd");
    int fd_write_close = close(file_desc);
    if(fd_write_close== -1)
        printf("Totally Useless");
    return 0;
}

