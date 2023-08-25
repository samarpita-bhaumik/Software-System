#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<stdio.h>
#include <sys/file.h>
#include<unistd.h>

int main()
{
	struct {
		int tno;
		int tikno;
		}db[3];
	for(int i=0;i<3;i++)
	{
		db[i].tno=i+1;
		db[i].tikno=0;
	}
	int fd = open("train.txt",O_CREAT|O_RDWR,0744);
	if(fd==-1)
	{
	  perror("error");
	  return 0;
	}
	write(fd,&db,sizeof(db));
	return 0;
}
