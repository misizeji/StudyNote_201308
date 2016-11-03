#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
	char *devname = "/dev/key1_eint";
	int fd;
	unsigned char key;
	
	fd = open(devname, O_RDWR);
	while(1)
	{
		read(fd, &key, sizeof(key)); 
		if(key)
			printf("the key = %d\n",key);
		key = 0;
	}
	close(fd);
}
