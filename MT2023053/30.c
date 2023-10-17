/*
============================================================================
Name : 30.c
Author : Samarpita Bhaumik
Description: Write a program to run a script at a specific time using a Daemon process.
Date: 8th Sept, 2023.
============================================================================
*/
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>

// Argument to be passed as hours minutes seconds
void main(int argc, char *argv[]) {

    // declare current time and deadline time in Epochs
    time_t curr, d;
    // User readable format
    struct tm *deadline;

    pid_t child;

    if (argc < 2)
        printf("Pass at least one argument(which is hour)\n");
    else {
        time(&curr); 				//to store current time time_t cur
        deadline = localtime(&curr);     	// user readable format and on success,localtime() return a pointer to a struct tm.
        deadline->tm_hour = atoi(argv[1]);      //convert string to integer
        deadline->tm_min = argv[2] == NULL ? 0 : atoi(argv[2]);
        deadline->tm_sec = argv[3] == NULL ? 0 : atoi(argv[3]);

        d = mktime(deadline); 			//convert the deadline from user readable format to time_t and On success, mktime() returns the calendar time (seconds since the Epoch), 
                                                //expressed as a value of type time_t.

        //create a daemon process
        if ((child= fork())==0) {
            setsid(); //set the current process as session leader
            chdir("/"); //change cur dir to root dir
            umask(0);   //all the files created by this process have execution permission 0777
            do{
                time(&curr); //calculate current time until deadline is reached
            }while (difftime(d, curr) > 0);
            
            printf("Done!\n");
            exit(0);
        }
        exit(0); //terminate the parent as child is a daemon process and hence should be orphan
    }
}

