#include<unistd.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
int main()
{
	int l1,l2,l3;
	l1 = link("file.txt","hl");	//hard link
	char *myfifo ="/tmp/myfifo";
	if(l1<0)
	{
		perror("failed to create hardlink");
		return 1;
	}
	l2 = symlink("file.txt","sl");	//soft link
	if(l2<0)
	{
		perror("failed to create softlink");
		return 1;
	}
	l3 = mkfifo(myfifo,0666);
	if(l3<0)
		perror("failed");
	return 0;
} 	
