

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int main(int argc, char *argv[])
{
	int fd = 0,ret = 0;

	fd = open("txt.txt",O_CREAT | O_RDWR,0777);	
	if(fd < 0)
	{
		perror("open");
	}

	close(1);

	ret = dup(fd);
	if (ret < 0)
	{
		perror("dup");
	}
	printf("hello world\n");

	return 0;
}
