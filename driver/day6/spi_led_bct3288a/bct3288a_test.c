#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "bct3288a.h"

#define DEV_PATH "/dev/bct3288a"

int main(void)
{
	int fd;
	int i,j;
	unsigned char buf[2];
	
	fd = open(DEV_PATH,O_RDWR);
	if(fd<0)
	{
		perror("dev bct3288a open:");
		return 1;
	}
	
	while(1)
	{
		buf[1] = 0;
		ioctl(fd,BCT3288A_CLEAR,0);
		for(i=4;i<16;i++)
		{
			buf[0] = i;
			if(i<10)
				buf[1] = buf[1] + 1;
			else
				buf[1] = buf[1] + 4;
			
			write(fd,buf,sizeof(buf));
			printf("buf[1]=%d\n",buf[1]);
			usleep(500*1000);
		}
		ioctl(fd,BCT3288A_CLOSE,0);
		sleep(1);
	}
	ioctl(fd,BCT3288A_CLOSE,0);
	close(fd);
	return 0;
}