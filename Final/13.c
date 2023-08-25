#include<stdio.h>
#include <sys/select.h>

int main()
{
	fd_set rfds;
	struct timeval tv;
	int retval;
	FD_ZERO(&rfds);
	FD_SET(0,&rfds);
	tv.tv_sec=10;
	tv.tv_usec=0;
	retval = select(1,&rfds,NULL,NULL,&tv);
	if(retval)
		printf("data available within 10s");
	else
		printf(" data not available after 10s");
	return 0;
}
