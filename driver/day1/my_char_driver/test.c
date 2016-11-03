#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc , char *argv[])
{
	int fd;
	char aver_test[20]={"hello world"};
	if((fd = open("/dev/char_driver", O_RDWR)) ==-1)
	{
		printf("file open error\n");
		exit(1);
	}	
	write(fd,aver_test,15);
	read(fd,aver_test,15);
	printf("<userspace>: test_read = %s\n",aver_test);
	close(fd);
	return 0;
}
