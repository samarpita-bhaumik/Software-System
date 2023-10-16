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



// Enrollment block
/*---------------------------------------------------------------------------------*/


struct Enrollment get_student_enrollment(int client_socket, int sid, int cid){
    int fd = open("records/enrollments",O_RDWR|O_APPEND);
    struct Enrollment e={0,0,0,0,0};
    if (fd == -1) {
        exit(1);
    }
    if(get_count(3)<cid){
        wr(client_socket,"Wrong Course id..#\n",20);
        return e;
    }
    if(get_count(1)<sid){
        wr(client_socket,"Wrong Student id..#\n",21);
        return e;
    }
    struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_pid = getpid();
    int f = fcntl(fd, F_SETLKW, &lock);
    if(f==-1){
        perror("Error obtaining Read lock on Enrollments Record!");
        close(fd);
    }
    lseek(fd,0,SEEK_SET);
    int bytesRead;

    while((bytesRead=read(fd,&e,sizeof(struct Enrollment))) == sizeof(struct Enrollment) || bytesRead == -1){
        if(e.studentID == sid && e.courseID == cid){
            break;
        }
    }
    lock.l_type = F_UNLCK;
    fcntl(fd,F_SETLK,&lock);
    close(fd);
    return e;
}


/*---------------------------------------------------------------------------------*/


// Courses Block
/*---------------------------------------------------------------------------------*/


struct Course get_courses(int client_socket, int id){
    struct Course c;
    int fd = open("records/courses",O_RDONLY);
    if (fd == -1) {
        exit(1);
    }
    if(get_count(3)<id){
        wr(client_socket,"Wrong Course id..#\n",20);
        return c;
    }
    struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = (id-1)*sizeof(struct Course);
    lock.l_len = sizeof(struct Course);
    lock.l_pid = getpid();
    int f = fcntl(fd, F_SETLKW, &lock);
    if(f==-1){
        perror("Error obtaining Read lock on Courses Record!");
        close(fd);
    }
    lseek(fd,(id-1)*sizeof(struct Course),SEEK_SET);
    int bytesRead;
    read(fd,&c,sizeof(struct Course));
    lock.l_type = F_UNLCK;
    fcntl(fd,F_SETLK,&lock);
    close(fd);
    return c;
}

void set_availability(int client_socket, int id,int cnt){
    int fd = open("records/courses",O_RDWR);
    if (fd == -1) {
        exit(1);
    }
    if(get_count(3)<id){
        wr(client_socket,"Wrong Course id..#\n",20);
    }
    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = (id-1)*sizeof(struct Course);
    lock.l_len = sizeof(struct Course);
    lock.l_pid = getpid();
    int f = fcntl(fd, F_SETLKW, &lock);
    if(f==-1){
        perror("Error obtaining Read lock on Courses Record!");
        close(fd);
    }
    lseek(fd,(id-1)*sizeof(struct Course),SEEK_SET);
    struct Course c;
    int bytesRead;
    read(fd,&c,sizeof(struct Course));
    c.available+=cnt;
    lseek(fd,(id-1)*sizeof(struct Course),SEEK_SET);
    write(fd,&c,sizeof(struct Course));
    lock.l_type = F_UNLCK;
    fcntl(fd,F_SETLK,&lock);
    close(fd);
    return;


}

/*---------------------------------------------------------------------------------*/


//Student Block
/*-----------------------------------------------------------------*/


int student_login(int client_socket,char *username, char *password,int *session){
    struct Student ad;
    int fd = open("records/student_details",O_RDONLY);
    if (fd == -1) {
        perror("Failed to open the file");
        exit(1);
    }
    int id;
    char *substr = strstr(username,"MT");
    if(substr){
        if(sscanf(substr+2,"%d",&id)==1){
            int offset=lseek(fd,(id - 1)*sizeof(struct Student),SEEK_SET);
            if(offset!=-1){
                // Implementing read lock
                    struct flock lock;
                    lock.l_type = F_RDLCK;
                    lock.l_whence = SEEK_SET;
                    lock.l_start = (id - 1)*sizeof(struct Student);
                    lock.l_len = 0;
                    lock.l_pid = getpid();
                    int f = fcntl(fd, F_SETLKW, &lock);
                    if(f==-1){
                        perror("Error obtaining read lock on Admin Record!");
                        close(fd);
                        return 0;
                    }
                    int readBytes=read(fd,&ad,sizeof(ad));
                    // Unlocking
                    lock.l_type = F_UNLCK;
                    fcntl(fd,F_SETLK,&lock);
                    close(fd);
                    if(readBytes > 0){
                                if(get_count(1)+1>id){
                                    if(strcmp(ad.username,username)!=0 || strcmp(ad.password,password)!=0 ){
                                        wr(client_socket,"Username or Password wrong.....#\n",34);
                                        return 0;
                                    }
                                    else if(strcmp(ad.username,username)==0 && strcmp(ad.password,password)==0){
                                        if(ad.isActive){
                                            *session = id;
                                            wr(client_socket,"Successfully Authenticated as Student....~\n",44);
                                            return 1;
                                        }
                                        else wr(client_socket,"Your ID is Blocked.....#\n",26);

                                    }
                                }
                                else wr(client_socket,"Username not found.....#\n",26);
                        }
                        else wr(client_socket,"Username wrong.....#\n",22);                       
            }

        }
        else wr(client_socket,"Username wrong.....#\n",22);
    }
    else wr(client_socket,"Username wrong.....#\n",22);
    return 0;
}

void view_all_courses(int);
void enroll_course(int,int);
void drop_course(int,int);
int * get_enrollments(int,int,int *,int);

void handle_student(int client_socket, int session){
    char resp[4];
    int choice;
    rd(client_socket,resp,4);
    if(strchr(resp,'~')!=NULL){
        
    }
    while (1)
    {
        wr(client_socket,"**************************Welcome to Student Menu**************************\n1. View All Offering Courses\n2. Enroll New course\n3. Drop Course\n4. View Course Enrollments\n5. Exit\nEnter Choice: \n",193);
        memset(resp,0,4);
        if(rd(client_socket,resp,4)==0){
            wr(client_socket,"No input try again...#\n",24);
            break;
        }
        if(resp[0] == '~'){

        }
        else if(!isnum(resp)){
            wr(client_socket,"Wrong option entered...~\n",26);
            // break;
        }
        else{
            choice = atoi(resp);
            switch (choice) {
                case 1:
                    view_all_courses(client_socket);
                    break;
                case 2:
                    enroll_course(client_socket,session);
                    break;
                case 3:
                    drop_course(client_socket,session);
                    break;
                case 4:
                    // view enrollments
                    int cnt;
                    char skip[2];
                    get_enrollments(client_socket,session,&cnt,1);
                    if(cnt == 0){
                        wr(client_socket,"No Courses Enrolled...~\n",26);
                        rd(client_socket,skip,2);
                    }
                    break;
                case 5: 
                    wr(client_socket,"Exiting...#\n",13);
                    return;
                    break;
                default:
                    printf("Invalid choice. Please select a valid option (1-9).\n");
                }

        }
    }
}

void view_all_courses(int client_socket){
    int fd = open("records/courses",O_RDONLY);
    if (fd == -1) {
        exit(1);
    }
    struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_pid = getpid();
    int f = fcntl(fd, F_SETLKW, &lock);
    if(f==-1){
        perror("Error obtaining Read lock on Courses Record!");
        close(fd);
    }
    lseek(fd,0,SEEK_SET);
    int course_cnt = get_count(3);
    if(course_cnt == 0){
        wr(client_socket,"No Courses Available...~\n",26);
    }
    struct Course c;
    int bytesRead;
    while((bytesRead=read(fd,&c,sizeof(struct Course))) == sizeof(struct Course) || bytesRead == -1){
        if(c.isActive){
            char send[100+sizeof(struct Course)],skip[2];
            sprintf(send,"Course Name: %s\nDepartment: %s\nCredits: %d\nTotal seats: %d\nAvailable Seats: %d\nId: %d\n~\n",c.cname,c.department,c.credits,c.seats,c.available,c.id);
            wr(client_socket,send,strlen(send)+1);
            rd(client_socket,skip,2);
            memset(send,0,100+sizeof(struct Course));
        }
    }
    lock.l_type = F_UNLCK;
    fcntl(fd,F_SETLK,&lock);
    close(fd);
}

void enroll_course(int client_socket,int session){
    int fd = open("records/enrollments",O_RDWR|O_APPEND);
    if (fd == -1) {
        exit(1);
    }
    int course_cnt = get_count(3);
    if(course_cnt == 0){
        wr(client_socket,"No Courses Available...~\n",26);
        return;
    }
    int id;
    char buff[10];
    while(1){
        wr(client_socket,"Enter the course ID number to Enroll: ",39);
        memset(buff,0,10);
        rd(client_socket,buff,10);
        if(buff[0] == '~'){
            continue;
        }
        if(!isnum(buff)){
                wr(client_socket,"Wrong id entered, Try again...~\n",33);
        }
        else{
            id = atoi(buff);
            if(id<=0 || get_count(3)<id){
                wr(client_socket,"Wrong id entered, Try again...~\n",33);
            }
            else{break;}
        }
    }
    int cnt;
    int *enrolled = get_enrollments(client_socket,session,&cnt,0);
    int flag = 0; // Flag to indicate if the number is found
    for (int i = 0; i < cnt; i++) {
        if (enrolled[i] == id) {
            flag = 1;
            break; // Number is found, exit the loop
        }
    }
    if(flag == 1){
        wr(client_socket,"Course already Enrolled...\n~\n",30);
        return ;
    }
    else{
        struct flock lock;
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_END;
        lock.l_start = 0;
        lock.l_len = sizeof(struct Student);
        lock.l_pid = getpid();
        int f = fcntl(fd, F_SETLKW, &lock);
        if(f==-1){
            perror("Error obtaining write lock on Student Record!");
            close(fd);
        }
        struct Course c = get_courses(client_socket,id);
        if(c.available > 0){
            int enrollId = set_count(4);
            set_availability(client_socket,id,-1);
            struct Enrollment e = {session,c.id,c.profId,enrollId,1};
            write(fd,&e,sizeof(struct Enrollment));
            char tempBuffer[100];
            sprintf(tempBuffer,"\nSuccessfully Enrolled in the course - %s\n~\n",c.cname);
            wr(client_socket,tempBuffer,strlen(tempBuffer)+1);
        }

        else{
            char skip[2];
            wr(client_socket,"No seats left...\n~\n",20);
            rd(client_socket,skip,2);
        }
        lock.l_type = F_UNLCK;
        fcntl(fd,F_SETLK,&lock);
    }
    close(fd);



}

int * get_enrollments(int client_socket, int session, int *cnt,int flag){
    int fd = open("records/enrollments",O_RDONLY);
    if (fd == -1) {
        exit(1);
    }
    int course_cnt = get_count(3);
    if(course_cnt == 0){
        wr(client_socket,"No Courses Available...~\n",26);
    }
    struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_pid = getpid();
    int f = fcntl(fd, F_SETLKW, &lock);
    if(f==-1){
        perror("Error obtaining Read lock on Courses Record!");
        close(fd);
    }
    lseek(fd,0,SEEK_SET);
    struct Enrollment e;
    int bytesRead;
    int arraySize =1;
    int count = 0;
    int *cid = (int *)malloc(arraySize * sizeof(int));
    while((bytesRead=read(fd,&e,sizeof(struct Enrollment))) == sizeof(struct Enrollment) || bytesRead == -1){
       
        if(e.isEnrolled && e.studentID == session){
            char send[100+sizeof(struct Course)],skip[2];
            struct Course c = get_courses(client_socket,e.courseID);
            if(flag){
                sprintf(send,"Course Name: %s\nDepartment: %s\nCredits: %d\nTotal seats: %d\nAvailable Seats: %d\nId: %d\n~\n",c.cname,c.department,c.credits,c.seats,c.available,c.id);
                wr(client_socket,send,strlen(send)+1);
                rd(client_socket,skip,2);
                memset(send,0,100+sizeof(struct Course));
            }
            arraySize += 1;
            cid = (int *)realloc(cid, arraySize * sizeof(int));
            if (cid == NULL) {
                perror("Memory reallocation failed");
            }
            cid[count++] = e.courseID;
        }
    }
    
    lock.l_type = F_UNLCK;
    fcntl(fd,F_SETLK,&lock);
    close(fd);
    *cnt = count;
    return cid;

}

void drop_course(int client_socket, int session){
    int fd = open("records/enrollments",O_RDWR);
    if (fd == -1) {
        exit(1);
    }
    int course_cnt = get_count(3);
    if(course_cnt == 0){
        wr(client_socket,"No Courses Available...~\n",26);
        return;
    }
    int cnt;
    int *enrolled = get_enrollments(client_socket,session,&cnt,0);
    if(cnt == 0){
        wr(client_socket,"No Courses Enrolled...~\n",26);
        return;
    }
    int cid;
    char buff[10];
    struct flock lock;
    while(1){
        wr(client_socket,"Enter the course ID number to Unenroll: ",39);
        memset(buff,0,10);
        rd(client_socket,buff,10);
        if(buff[0] == '~'){
            continue;
        }
        if(!isnum(buff)){
                wr(client_socket,"Wrong id entered, Try again...~\n",33);
        }
        else{
            cid = atoi(buff);
            if(cid<=0 || get_count(3)<cid){
                wr(client_socket,"Wrong id entered, Try again...~\n",33);
            }
            else{break;}
        }
    }
    int flag = 0; // Flag to indicate if the number is found
    for (int i = 0; i < cnt; i++) {
        if (enrolled[i] == cid) {
            flag = 1;
            break; // Number is found, exit the loop
        }
    }
    if(flag == 0){
        wr(client_socket,"Course not Enrolled...\n~\n",26);
        return ;
    }
    else{
        struct Enrollment e = get_student_enrollment(client_socket,session,cid);
        int id = e.enrollmentID;
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = (id-1)*sizeof(struct Enrollment);
        lock.l_len = sizeof(struct Enrollment);
        lock.l_pid = getpid();
        int f = fcntl(fd, F_SETLKW, &lock);
        if(f==-1){
            perror("Error obtaining write lock on Enrollment Record!");
            close(fd);
        }
        if(e.isEnrolled){
            set_availability(client_socket,id,1);
            e.isEnrolled=0;
            lseek(fd,(id-1)*sizeof(struct Enrollment),SEEK_SET);
            write(fd,&e,sizeof(struct Enrollment));
        }      
    }
    lock.l_type = F_UNLCK;
    fcntl(fd,F_SETLK,&lock);
    close(fd);
}

/*-----------------------------------------------------------------*/


// Admin Block
/*-----------------------------------------------------------------*/

void add_student(int client_socket);
void add_teacher(int client_socket);
void view_participant(int ,int,int);
void activate_deactivate(int,int);
void modify(int,int);
// Funtion to handle login
int admin_login(int client_socket,char *username, char *password){
    struct AdminCredentials ad;
    int fd = open("records/admin_cred",O_RDONLY);
    if (fd == -1) {
        perror("Failed to open the file");
        exit(1);
    }
    int offset=lseek(fd,0*sizeof(struct AdminCredentials),SEEK_SET);
    if(offset!=-1){
        // Implementing read lock
            struct flock lock;
            lock.l_type = F_RDLCK;
            lock.l_whence = SEEK_SET;
            lock.l_start = 0;
            lock.l_len = 0;
            lock.l_pid = getpid();
            int f = fcntl(fd, F_SETLKW, &lock);
            if(f==-1){
                perror("Error obtaining read lock on Admin Record!");
                close(fd);
                return 0;
            }
            int readBytes=read(fd,&ad,sizeof(ad));
            // Unlocking
            lock.l_type = F_UNLCK;
            fcntl(fd,F_SETLK,&lock);
            close(fd);
            if(readBytes > 0){
                if(strcmp(ad.username,username)!=0 || strcmp(ad.password,password)!=0){
                    wr(client_socket,"Username or Password wrong.....#\n",34);
                    return 0;
                }
                else if(strcmp(ad.username,username)==0 && strcmp(ad.password,password)==0){
                    wr(client_socket,"Successfully Authenticated as Admin....~\n",42);
                    return 1;
                }
            }
            return 0;
    }
}

// Function to handle admin
void handle_Admin(int client_socket) {
    char resp[4];
    int choice;
    rd(client_socket,resp,4);
    if(strchr(resp,'~')!=NULL){
        
    }
    while (1)
    {
        wr(client_socket,"**************************Welcome to Admin Menu**************************\n1. Add Student\n2. View Student details\n3. Add Faculty\n4. View Faculty details\n5. Activate Student\n6. Block Student\n7. Modify Student Details\n8. Modify Faculty Details\n9. Exit\nEnter Choice\n",263);
        memset(resp,0,4);
        if(rd(client_socket,resp,4)==0){
            wr(client_socket,"No input try again...#\n",24);
            break;
        }
        if(resp[0] == '~'){

        }
        else if(!isnum(resp)){
            wr(client_socket,"Wrong option entered...~\n",26);
            // break;
        }
        else{
            choice = atoi(resp);
            switch (choice) {
                case 1:
                    add_student(client_socket);
                    break;
                case 2:
                    view_participant(client_socket,1,-1);
                    break;
                case 3:
                    add_teacher(client_socket);
                    break;
                case 4:
                    view_participant(client_socket,2,-1);
                    break;
                case 5:
                    activate_deactivate(client_socket,1);
                    break;
                case 6:
                    activate_deactivate(client_socket,0);
                    break;
                case 7:
                    modify(client_socket,1);
                    break;
                case 8:
                    modify(client_socket,2);
                    break;
                case 9:
                    wr(client_socket,"Exiting...#\n",13);
                    return;
                    break;
                default:
                    printf("Invalid choice. Please select a valid option (1-9).\n");
                }

        }


    }  
}
void add_student(int client_socket){
    
    int fd = open("records/student_details",O_RDWR|O_APPEND);
        if (fd == -1) {
            exit(1);
        }
        // Implementing write lock
        struct flock lock;
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_END;
        lock.l_start = 0;
        lock.l_len = sizeof(struct Student);
        lock.l_pid = getpid();
        int f = fcntl(fd, F_SETLKW, &lock);
        if(f==-1){
            perror("Error obtaining write lock on Student Record!");
            close(fd);
        }
        int id = set_count(1);
        struct Student st;
        char age[2],gen[2],password[40];
        sprintf(st.username,"MT%d",id);
        wr(client_socket,"Enter Name: ",13);
        rd(client_socket,st.name,sizeof(st.name));
        wr(client_socket,"Enter Email: ",14);
        rd(client_socket,st.email,sizeof(st.email));
        wr(client_socket,"Enter Address: ",16);
        rd(client_socket,st.address,sizeof(st.address));
        wr(client_socket,"Enter Gender: ",15);
        rd(client_socket,gen,sizeof(gen));
        st.gender = gen[0];
        wr(client_socket,"Enter Age: ",12);
        rd(client_socket,age,sizeof(age));
        st.age = atoi(age);
        st.rollNumber = id;
        st.isActive = true;
        wr(client_socket,"Enter Default Password( MT{name} ): ",36);
        rd(client_socket,password,sizeof(password));
        strcpy(st.password,password);
        write(fd,&st,sizeof(struct Student));
        char tempBuffer[100];
        sprintf(tempBuffer,"\nSuccessfully added student\nNote: Login-id is MT{Student-ID}\nStudent-Id Generated is: %d...\n~",id);
        wr(client_socket,tempBuffer,strlen(tempBuffer)+1);
        // Unlocking
        lock.l_type = F_UNLCK;
        fcntl(fd,F_SETLK,&lock);
        close(fd);
        
}

void add_teacher(int client_socket){
    
    int fd = open("records/teacher_details",O_RDWR|O_APPEND);
        if (fd == -1) {
            exit(1);
        }
        // Implementing write lock
        struct flock lock;
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_END;
        lock.l_start = 0;
        lock.l_len = sizeof(struct Teacher);
        lock.l_pid = getpid();
        int f = fcntl(fd, F_SETLKW, &lock);
        if(f==-1){
            perror("Error obtaining write lock on Student Record!");
            close(fd);
        }
        int id = set_count(2);
        char pass[40];
        struct Teacher t;
        sprintf(t.username,"PROF%d",id);
        wr(client_socket,"Enter Name: ",13);
        rd(client_socket,t.name,sizeof(t.name));
        wr(client_socket,"Enter Email: ",14);
        rd(client_socket,t.email,sizeof(t.email));
        wr(client_socket,"Enter Department: ",19);
        rd(client_socket,t.department,sizeof(t.department));
        wr(client_socket,"Enter Designation: ",20);
        rd(client_socket,t.desig,sizeof(t.desig));
        wr(client_socket,"Enter Address: ",16);
        rd(client_socket,t.address,sizeof(t.address));
        t.id = id;
        wr(client_socket,"Enter Default Password( PROF{name} ): ",38);
        rd(client_socket,pass,sizeof(pass));
        strcpy(t.password,pass);
        write(fd,&t,sizeof(struct Teacher));
        char tempBuffer[100];
        sprintf(tempBuffer,"\nSuccessfully added Teacher\nNote: Login-id is PROF{Prof-ID}\nProf-Id Generated is: %d...\n~",id);
        wr(client_socket,tempBuffer,strlen(tempBuffer)+1);
        // Unlocking
        lock.l_type = F_UNLCK;
        fcntl(fd,F_SETLK,&lock);
        close(fd);
        
}

void view_participant(int client_socket, int type, int sID){
    int fd,len;
    if(type == 1){
        fd = open("records/student_details",O_RDONLY);
        if (fd == -1) {
            exit(1);
        }
        len = sizeof(struct Student);
    }
    else if(type == 2){
        fd = open("records/teacher_details",O_RDONLY);
        if (fd == -1) {
            exit(1);
        }
        len = sizeof(struct Teacher);
    }
    int id;
    char buff[10];
    if(sID == -1){
        while(1){
            wr(client_socket,"Enter the ID number to access: ",32);
            memset(buff,0,10);
            rd(client_socket,buff,10);
            if(buff[0] == '~'){
                continue;
            }
            if(!isnum(buff)){
                    wr(client_socket,"Wrong id entered, Try again...~\n",33);
            }
            else{
                id = atoi(buff);
                if(id<=0 || get_count(type)<id){
                    wr(client_socket,"Wrong id entered, Try again...~\n",33);
                }
                else{break;}
            }
        }
    }
    else id = sID;
    struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = (id-1)*len;
    lock.l_len = len;
    lock.l_pid = getpid();
    int f = fcntl(fd, F_SETLKW, &lock);
    if(f==-1){
        perror("Error obtaining write lock on Student Record!");
        close(fd);
    }
    lseek(fd,(id-1)*len,SEEK_SET);
    if(type==1){
        struct Student st;
        read(fd,&st,sizeof(struct Student));
        char send[100+sizeof(struct Student)];
        sprintf(send,"Name: %s\nEmail: %s\nAddress: %s\nRollno: %d\nUsername: %s~\n",st.name,st.email,st.address,st.rollNumber,st.username);
        wr(client_socket,send,strlen(send)+1);
    }
    else{
        struct Teacher t;
        read(fd,&t,sizeof(struct Teacher));
        char send[100+sizeof(struct Teacher)];
        sprintf(send,"Name: %s\nEmail: %s\nDepartment: %s\nDesignation: %s\nAddress: %s\nUsername: %s~\n",t.name,t.email,t.department,t.desig,t.address,t.username);
        wr(client_socket,send,strlen(send)+1);
    }
    lock.l_type = F_UNLCK;
    fcntl(fd,F_SETLK,&lock);
    close(fd);
}

void activate_deactivate(int client_socket, int status){
    int fd = open("records/student_details",O_RDWR);
    if (fd == -1) {
        exit(1);
    }
    int id;
    char buff[10];
    while(1){
        if(status == 1)
            wr(client_socket,"Enter the Student ID number to Activate: ",42);
        else
            wr(client_socket,"Enter the Student ID number to Deactivate: ",44);
        memset(buff,0,10);
        rd(client_socket,buff,10);
        if(buff[0] == '~'){
            continue;
        }
        if(!isnum(buff)){
                wr(client_socket,"Wrong id entered, Try again...~\n",33);
        }
        else{
            id = atoi(buff);
            if(id<=0 || get_count(1)<id){
                wr(client_socket,"Wrong id entered, Try again...~\n",33);
            }
            else{break;}
        }
    }
    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = (id-1)*sizeof(struct Student);
    lock.l_len = sizeof(struct Student);
    lock.l_pid = getpid();
    int f = fcntl(fd, F_SETLKW, &lock);
    if(f==-1){
        perror("Error obtaining write lock on Student Record!");
        close(fd);
    }
    lseek(fd,(id-1)*sizeof(struct Student),SEEK_SET);
    struct Student st;
    read(fd,&st,sizeof(struct Student));
    if(status == 1)st.isActive=true;
    else st.isActive = false;
    lseek(fd,(id-1)*sizeof(struct Student),SEEK_SET);
    write(fd,&st,sizeof(struct Student));
    lock.l_type = F_UNLCK;
    fcntl(fd,F_SETLK,&lock);
    close(fd);
}

void modify(int client_socket,int type){
    int fd,len;
    if(type == 1){
        fd = open("records/student_details",O_RDWR);
        if (fd == -1) {
            exit(1);
        }
        len = sizeof(struct Student);
    }
    else if(type == 2){
        fd = open("records/teacher_details",O_RDWR);
        if (fd == -1) {
            exit(1);
        }
        len = sizeof(struct Teacher);
    }
    int id;
    char buff[10];
    while(1){
        wr(client_socket,"Enter the ID number to access: ",32);
        memset(buff,0,10);
        rd(client_socket,buff,10);
        if(buff[0] == '~'){
            continue;
        }
        if(!isnum(buff)){
                wr(client_socket,"Wrong id entered, Try again...~\n",33);
        }
        else{
            id = atoi(buff);
            if(id<=0 || get_count(type)<id){
                wr(client_socket,"Wrong id entered, Try again...~\n",33);
            }
            else{break;}
        }
    }
    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = (id-1)*len;
    lock.l_len = len;
    lock.l_pid = getpid();
    int f = fcntl(fd, F_SETLKW, &lock);
    if(f==-1){
        perror("Error obtaining write lock on Student Record!");
        close(fd);
    }
    lseek(fd,(id-1)*len,SEEK_SET);
    if(type == 1){
        struct Student st;
        read(fd,&st,len);
        char resp[2];
        int choice;
        while(1){
            wr(client_socket,"Enter option{1. Name, 2. Email, 3. Address, 4. Age}: ",54);
            memset(resp,0,2);
            rd(client_socket,resp,2);
            if(resp[0] == '~'){
                continue;
            }
            if(!isnum(resp)){
                    wr(client_socket,"Wrong option entered, Try again...~\n",37);
            }
            else{
                choice = atoi(resp);
                if(choice > 5 || choice < 1){
                    wr(client_socket,"Wrong option entered, Try again...~\n",37);
                }
                else{break;}
            }
        }
        switch (choice)
        {
        case 1:
            wr(client_socket,"Enter Name: ",13);
            rd(client_socket,st.name,sizeof(st.name));
            break;
        case 2:
            wr(client_socket,"Enter Email: ",14);
            rd(client_socket,st.email,sizeof(st.email));
            break;
        case 3: 
            wr(client_socket,"Enter Address: ",16);
            rd(client_socket,st.address,sizeof(st.address));
            break;
        case 4:
            char age[2];
            wr(client_socket,"Enter Age: ",12);
            rd(client_socket,age,sizeof(age));
            st.age = atoi(age);
            break;
        default:
            break;
        }
        lseek(fd,(id-1)*len,SEEK_SET);
        write(fd,&st,sizeof(st));
    }

     if(type == 2){
        struct Teacher st;
        read(fd,&st,len);
        char resp[2];
        int choice;
        while(1){
            wr(client_socket,"Enter option{1. Name, 2. Email, 3. Address, 4. Department, 5. Designation}: ",77);
            memset(resp,0,2);
            rd(client_socket,resp,2);
            if(resp[0] == '~'){
                continue;
            }
            if(!isnum(resp)){
                    wr(client_socket,"Wrong option entered, Try again...~\n",37);
            }
            else{
                choice = atoi(resp);
                if(choice > 5 || choice < 1){
                    wr(client_socket,"Wrong option entered, Try again...~\n",37);
                }
                else{break;}
            }
        }
        switch (choice)
        {
        case 1:
            wr(client_socket,"Enter Name: ",13);
            rd(client_socket,st.name,sizeof(st.name));
            break;
        case 2:
            wr(client_socket,"Enter Email: ",14);
            rd(client_socket,st.email,sizeof(st.email));
            break;
        case 3: 
            wr(client_socket,"Enter Address: ",16);
            rd(client_socket,st.address,sizeof(st.address));
            break;
        case 4:
            wr(client_socket,"Enter Deparment: ",12);
            rd(client_socket,st.department,sizeof(st.department));
            break;
        case 5:
            wr(client_socket,"Enter Designation: ",20);
            rd(client_socket,st.desig,sizeof(st.desig));
            break;
        default:
            break;
        }

        lseek(fd,(id-1)*len,SEEK_SET);
        write(fd,&st,sizeof(st));
    }
    lock.l_type = F_UNLCK;
    fcntl(fd,F_SETLK,&lock);
    close(fd);
}


/*---------------------------------------------------------------------------------*/


//Faculty Block
/*-----------------------------------------------------------------*/

void add_course(int client_socket, int session);
int* view_courses(int , int, int *,int);
void delete_course(int,int);
struct Enrollment* view_enrollments(int, int , int *,int);
void modify_course(int , int);
int handle_seats(int , int , int , int, int, int);
int faculty_login(int client_socket,char *username, char *password,int *session){
    struct Teacher ad;
    int fd = open("records/teacher_details",O_RDONLY);
    if (fd == -1) {
        perror("Failed to open the file");
        exit(1);
    }
    int id;
    char *substr = strstr(username,"PROF");
    if(substr){
        if(sscanf(substr+4,"%d",&id)==1){
            int offset=lseek(fd,(id - 1)*sizeof(struct Teacher),SEEK_SET);

            if(offset!=-1){
                // Implementing read lock
                    struct flock lock;
                    lock.l_type = F_RDLCK;
                    lock.l_whence = SEEK_SET;
                    lock.l_start = (id - 1)*sizeof(struct Teacher);
                    lock.l_len = 0;
                    lock.l_pid = getpid();
                    int f = fcntl(fd, F_SETLKW, &lock);
                    if(f==-1){
                        perror("Error obtaining read lock on Admin Record!");
                        close(fd);
                        return 0;
                    }
                    int readBytes=read(fd,&ad,sizeof(ad));
                    // Unlocking
                    lock.l_type = F_UNLCK;
                    fcntl(fd,F_SETLK,&lock);
                    close(fd);
                    if(readBytes > 0){
                                if(get_count(2)+1>id){
                                    if(strcmp(ad.username,username)!=0 || strcmp(ad.password,password)!=0 ){
                                        wr(client_socket,"Username or Password wrong.....#\n",34);
                                        return 0;
                                    }
                                    else if(strcmp(ad.username,username)==0 && strcmp(ad.password,password)==0){
                                        *session = ad.id;
                                        wr(client_socket,"Successfully Authenticated as Faculty....~\n",42);
                                        return 1;
                                    }
                                }
                                else wr(client_socket,"Username not found.....#\n",26);
                        }
                        else wr(client_socket,"Username wrong.....#\n",22);                       
            }

        }
        else wr(client_socket,"Username wrong.....#\n",22);
    }
    else wr(client_socket,"Username wrong.....#\n",22);
    return 0;
}



void handle_faculty(int client_socket, int session){
    char resp[4];
    int choice;
    rd(client_socket,resp,4);
    if(strchr(resp,'~')!=NULL){
        
    }
    while (1)
    {
        wr(client_socket,"**************************Welcome to Faculty Menu**************************\n1. View Offering Courses\n2. Add New course\n3. Remove Course from Catalog\n4. Modify Course Details\n5. View Course Enrollments\n6. Exit\nEnter Choice: \n",226);
        memset(resp,0,4);
        if(rd(client_socket,resp,4)==0){
            wr(client_socket,"No input try again...#\n",24);
            break;
        }
        if(resp[0] == '~'){

        }
        else if(!isnum(resp)){
            wr(client_socket,"Wrong option entered...~\n",26);
            // break;
        }
        else{
            choice = atoi(resp);
            switch (choice) {
                case 1:
                    int courseCnt;
                    view_courses(client_socket,session,&courseCnt,1);
                    break;
                case 2:
                    add_course(client_socket,session);
                    break;
                case 3:
                    delete_course(client_socket,session);
                    break;
                case 4:
                    modify_course(client_socket,session);
                    break;
                case 5:
                    int cnt;
                    view_enrollments(client_socket,session,&cnt,1);
                    break;
                case 6:
                    wr(client_socket,"Exiting...#\n",13);
                    return;
                    break;
                default:
                    printf("Invalid choice. Please select a valid option (1-9).\n");
                }

        }
    }
    
}

void add_course(int client_socket, int session){
    int fd = open("records/courses",O_RDWR|O_APPEND);
        if (fd == -1) {
            exit(1);
        }
        // Implementing write lock
        struct flock lock;
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_END;
        lock.l_start = 0;
        lock.l_len = sizeof(struct Course);
        lock.l_pid = getpid();
        int f = fcntl(fd, F_SETLKW, &lock);
        if(f==-1){
            perror("Error obtaining write lock on Course Record!");
            close(fd);
        }
        int id = set_count(3);
        struct Course c;
        char seats[4],credits[2];
        wr(client_socket,"Enter Name: ",13);
        rd(client_socket,c.cname,sizeof(c.cname));
        wr(client_socket,"Enter Department: ",19);
        rd(client_socket,c.department,sizeof(c.department));
        wr(client_socket,"Enter Credits: ",16);
        rd(client_socket,credits,2);
        wr(client_socket,"Enter Total Seats: ",20);
        rd(client_socket,seats,4);
        c.credits = atoi(credits);
        c.seats = atoi(seats);
        c.available = c.seats;
        c.profId = session;
        c.isActive = 1;
        c.id = id;
        write(fd,&c,sizeof(struct Course));
        char tempBuffer[100];
        sprintf(tempBuffer,"\nSuccessfully added Course\nCourse-Id Generated is: %d\n~",id);
        wr(client_socket,tempBuffer,strlen(tempBuffer)+1);
        // Unlocking
        lock.l_type = F_UNLCK;
        fcntl(fd,F_SETLK,&lock);
        close(fd);

}

int* view_courses(int client_socket, int session, int *courseCount,int flag){
    int fd = open("records/courses",O_RDONLY);
    if (fd == -1) {
        exit(1);
    }
    struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_pid = getpid();
    int f = fcntl(fd, F_SETLKW, &lock);
    if(f==-1){
        perror("Error obtaining Read lock on Courses Record!");
        close(fd);
    }
    lseek(fd,0,SEEK_SET);
    struct Course c;
    int bytesRead;
    int arraySize =1;
    int count = 0;
    int *cid = (int *)malloc(arraySize * sizeof(int));
    while((bytesRead=read(fd,&c,sizeof(struct Course))) == sizeof(struct Course) || bytesRead == -1){
        if(c.isActive && c.profId==session){
            if(flag){
            char send[100+sizeof(struct Course)],skip[2];
            sprintf(send,"Course Name: %s\nDepartment: %s\nCredits: %d\nTotal seats: %d\nAvailable Seats: %d\nId: %d\n~\n",c.cname,c.department,c.credits,c.seats,c.available,c.id);
            wr(client_socket,send,strlen(send)+1);
            rd(client_socket,skip,2);
            memset(send,0,100+sizeof(struct Course));
            }

            arraySize += 1;
            cid = (int *)realloc(cid, arraySize * sizeof(int));
            if (cid == NULL) {
                perror("Memory reallocation failed");
            }
            cid[count++] = c.id;
        }
    }
    lock.l_type = F_UNLCK;
    fcntl(fd,F_SETLK,&lock);
    close(fd);
    *courseCount=count;
    return cid;
}

void delete_course(int client_socket, int session){
    int fd = open("records/courses",O_RDWR);
    if (fd == -1) {
        exit(1);
    }
    int courseCount;
    int *cids = view_courses(client_socket,session,&courseCount,1);
    int enrollCnt;
    struct Enrollment *e = view_enrollments(client_socket,session,&enrollCnt,0);
    int id;
    char buff[10];
    while(1){
        wr(client_socket,"Enter the course ID number to Deactivate: ",43);
        memset(buff,0,10);
        rd(client_socket,buff,10);
        if(buff[0] == '~'){
            continue;
        }
        if(!isnum(buff)){
                wr(client_socket,"Wrong id entered, Try again...~\n",33);
        }
        else{
            id = atoi(buff);
            if(id<=0 || get_count(3)<id){
                wr(client_socket,"Wrong id entered, Try again...~\n",33);
            }
            else{break;}
        }
    }
    int flag = 0; // Flag to indicate if the number is found
    for (int i = 0; i < courseCount; i++) {
        if (cids[i] == id) {
            flag = 1;
            break; // Number is found, exit the loop
        }
    }
    if(flag == 0){
        wr(client_socket,"Course not Found...\n~\n",23);
        return ;
    }
    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = (id-1)*sizeof(struct Course);
    lock.l_len = sizeof(struct Course);
    lock.l_pid = getpid();
    int f = fcntl(fd, F_SETLKW, &lock);
    if(f==-1){
        perror("Error obtaining write lock on Course Record!");
        close(fd);
    }
    lseek(fd,(id-1)*sizeof(struct Course),SEEK_SET);
    struct Course c;
    read(fd,&c,sizeof(struct Course));
    c.isActive = 0;
    lseek(fd,(id-1)*sizeof(struct Course),SEEK_SET);
    write(fd,&c,sizeof(struct Course));
    lock.l_type = F_UNLCK;
    fcntl(fd,F_SETLK,&lock);
    close(fd);

    
    fd = open("records/enrollments",O_RDWR);
    if (fd == -1) {
        exit(1);
    }
    // Un-enrolling students from deleted course
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_pid = getpid();
    f = fcntl(fd, F_SETLKW, &lock);
    if(f==-1){
        perror("Error obtaining Read lock on Courses Record!");
        close(fd);
    }
    lseek(fd,0,SEEK_SET);    
    for(int i=0;i<enrollCnt;i++){
        struct Enrollment w=e[i];
        if(w.courseID == id){
            int eid = w.enrollmentID;
            w.isEnrolled = 0;
            lseek(fd,(eid-1)*sizeof(struct Enrollment),SEEK_SET);
            write(fd,&w,sizeof(struct Enrollment));
        }
    }
    lock.l_type = F_UNLCK;
    fcntl(fd,F_SETLK,&lock);
    close(fd);
}

struct Enrollment* view_enrollments(int client_socket, int session , int *enrollCount,int flag){
    int fd = open("records/enrollments",O_RDONLY);
    if (fd == -1) {
        exit(1);
    }
    int courseCount=0,arraySize=1,count=0;
    struct Enrollment *e = (struct Enrollment *)malloc(arraySize * sizeof(struct Enrollment));
    
    int *cids=view_courses(client_socket,session,&courseCount,0);
    if(courseCount == 0){
        wr(client_socket,"No Courses Created...\n~\n",25);
        return e;
    }
    struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_pid = getpid();
    int f = fcntl(fd, F_SETLKW, &lock);
    if(f==-1){
        perror("Error obtaining Read lock on Courses Record!");
        close(fd);
    }
    int bytesRead;
    for(int i=0;i<courseCount;i++){
        struct Course c = get_courses(client_socket,cids[i]);
        char send[100+sizeof(struct Course)],skip[2];
        if(flag){
                sprintf(send,"Course Name: %s\nDepartment: %s\nCredits: %d\nTotal seats: %d\nAvailable Seats: %d\nId: %d\n~\n",c.cname,c.department,c.credits,c.seats,c.available,c.id);
                wr(client_socket,send,strlen(send)+1);
                rd(client_socket,skip,2);
                memset(send,0,100+sizeof(struct Course));
            }
        struct Enrollment er;
        lseek(fd,0,SEEK_SET);
        int ec=0;
        while((bytesRead=read(fd,&er,sizeof(struct Enrollment))) == sizeof(struct Enrollment) || bytesRead == -1){
            if(er.isEnrolled && er.courseID == c.id && er.profID == session){
                char sk[2];
            if(flag){
                wr(client_socket,".........................Enrollment Details.........................\n~\n",72);
                view_participant(client_socket,1,er.studentID);
                rd(client_socket,sk,2);
            }
            arraySize += 1;
            e = (struct Enrollment *)realloc(e, arraySize * sizeof(struct Enrollment));
            if (e == NULL) {
                perror("Memory reallocation failed");
            }
            e[count++] = er;
            ec++;
            }
        }
        if(ec==0){
            char sk[2];
            wr(client_socket,".........................Enrollment Details.........................\n~\n",72);
            wr(client_socket,"No Enrollments for this course...\n~\n",37);
            rd(client_socket,sk,2);
        }
    }

    lock.l_type = F_UNLCK;
    fcntl(fd,F_SETLK,&lock);
    close(fd);
    *enrollCount = count;
    return e; 
}

void modify_course(int client_socket, int session){

    int courseCount;
    int *cids = view_courses(client_socket,session,&courseCount,1);
    
    int id;
    char buff[10];
    while(1){
        wr(client_socket,"Enter the course ID number to Modify: ",39);
        memset(buff,0,10);
        rd(client_socket,buff,10);
        if(buff[0] == '~'){
            continue;
        }
        if(!isnum(buff)){
                wr(client_socket,"Wrong id entered, Try again...~\n",33);
        }
        else{
            id = atoi(buff);
            if(id<=0 || get_count(3)<id){
                wr(client_socket,"Wrong id entered, Try again...~\n",33);
            }
            else{break;}
        }
    }
    int flag = 0; // Flag to indicate if the number is found
    for (int i = 0; i < courseCount; i++) {
        if (cids[i] == id) {
            flag = 1;
            break; // Number is found, exit the loop
        }
    }
    if(flag == 0){
        wr(client_socket,"Course not Found...\n~\n",23);
        return ;
    }
    int fd = open("records/courses",O_RDWR);
    if (fd == -1) {
        exit(1);
    }
    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = (id-1)*sizeof(struct Course);
    lock.l_len = sizeof(struct Course);
    lock.l_pid = getpid();
    int f = fcntl(fd, F_SETLKW, &lock);
    if(f==-1){
        perror("Error obtaining write lock on Course Record!");
        close(fd);
    }
    lseek(fd,(id-1)*sizeof(struct Course),SEEK_SET);
    struct Course c;
    read(fd,&c,sizeof(struct Course));
    char resp[2];
    int choice;
    while(1){
        wr(client_socket,"Enter option{1. Course Name, 2. Department, 3. Credits, 4. Seats}: ",68);
        memset(resp,0,2);
        rd(client_socket,resp,2);
        if(resp[0] == '~'){
            continue;
        }
        if(!isnum(resp)){
                wr(client_socket,"Wrong option entered, Try again...~\n",37);
        }
        else{
            choice = atoi(resp);
            if(choice > 4 || choice < 1){
                wr(client_socket,"Wrong option entered, Try again...~\n",37);
            }
            else{break;}
        }
    }
    char credits[4];
    switch (choice)
        {
        case 1:
            wr(client_socket,"Enter Course Name: ",20);
            rd(client_socket,c.cname,sizeof(c.cname));
            break;
        case 2:
            wr(client_socket,"Enter Department: ",19);
            rd(client_socket,c.department,sizeof(c.department));
            break;
        case 3: 
            wr(client_socket,"Enter Credits: ",16);
            rd(client_socket,credits,sizeof(credits));
            c.credits = atoi(credits);
            break;
        case 4:
            char s[4];
            wr(client_socket,"Enter Seats: ",14);
            rd(client_socket,s,sizeof(s));
            int seats = atoi(s);
            int retV = handle_seats(client_socket,session,id,c.seats,seats,c.available);
            c.available +=  retV;
            c.seats = seats;
            break;
        default:
            break;
        }
        lseek(fd,(id-1)*sizeof(struct Course),SEEK_SET);
        write(fd,&c,sizeof(struct Course));

        lock.l_type = F_UNLCK;
        fcntl(fd,F_SETLK,&lock);
        close(fd);
}

int handle_seats(int client_socket, int session,int cid, int prevSeats, int currSeats, int available){
    if(prevSeats <= currSeats){
        return currSeats-prevSeats;
    } 
    else if(prevSeats > currSeats){

        int seatsDecr = prevSeats - currSeats;
        if(available >= seatsDecr)return -seatsDecr;
        else{
        int unEnrollNum = seatsDecr - available;
        int enrollCnt;
        struct Enrollment *e = view_enrollments(client_socket,session,&enrollCnt,0);
        int struct_size = sizeof(struct Enrollment);

        int fd = open("records/enrollments",O_RDWR);
        if (fd == -1) {
            exit(1);
        }
        // Un-enrolling students from deleted course
        struct flock lock;
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = 0;
        lock.l_len = 0;
        lock.l_pid = getpid();
        int f = fcntl(fd, F_SETLKW, &lock);
        if(f==-1){
            perror("Error obtaining Read lock on Courses Record!");
            close(fd);
        }

        int file_size = lseek(fd,0,SEEK_END);

        if (file_size == -1) {
            perror("Error getting file size");
            lock.l_type = F_UNLCK;
            fcntl(fd,F_SETLK,&lock);
            close(fd);
            return -2;
        }
        for (int i = file_size - struct_size; i >= 0; i -= struct_size) {
            lseek(fd, i, SEEK_SET);  // Seek to the current position
            if(unEnrollNum>0){
                struct Enrollment unEnroll;
                if (read(fd, &unEnroll, struct_size) == -1) {
                    perror("Error reading the file");
                    lock.l_type = F_UNLCK;
                    fcntl(fd,F_SETLK,&lock);
                    close(fd);
                    return 1;
                }
                struct Enrollment w = unEnroll;
                if(w.courseID == cid){
                    int eid = w.enrollmentID;
                    w.isEnrolled = 0;
                    lseek(fd,(eid-1)*sizeof(struct Enrollment),SEEK_SET);
                    write(fd,&w,sizeof(struct Enrollment));
                }
                unEnrollNum--;
            }
            else break;
        }

        lock.l_type = F_UNLCK;
        fcntl(fd,F_SETLK,&lock);
        close(fd);
        return -available;
        }
    }
}

/*-----------------------------------------------------------------*/


//Initial Login Block
/*---------------------------------------------------------------------------------*/

int login(int client_socket){
    char buffer[MAX_MESSAGE_SIZE];
    wr(client_socket,"\n**************************Welcome to Academia Portal**************************\n\nDefault Admin Login Creds: \nUserName-Admin\nPassword-Admin\n\nLogin type:\n1. Admin\n2. Faculty\n3. Student\nEnter choice: ",198);
    rd(client_socket, buffer,2);
    int res = atoi(buffer);
    if(res<1 || res > 3){
        wr(client_socket,"Wrong option entered\n#",23);
        return 0;
    }
    char username[50], password[50];
    wr(client_socket,"Username: ",11);
    rd(client_socket, username,sizeof(username));
    wr(client_socket,"Password: ",12);
    rd(client_socket, password,sizeof(password));
    switch (res)
    {
        
    case 1:
        // Admin
        if(admin_login(client_socket,username,password)){
            handle_Admin(client_socket);
            return 1;
        }
        else{
            return 0;
        }
        
        break;
    case 2:
        // Faculty
        int profSession;
        if(faculty_login(client_socket,username,password, &profSession)){
            handle_faculty(client_socket,profSession);
            return 1;
        }
        else{
            return 0;
        }
        break;
    case 3:
        // Student
        int session;
        if(student_login(client_socket,username,password, &session)){
            handle_student(client_socket,session);
            return 1;
        }
        else{
            return 0;
        }
        break;
    default:
        break;
    }
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
            if (login(client_socket)==0){
                exit(EXIT_FAILURE);
            }
            else{
                exit(EXIT_SUCCESS);
            }
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

