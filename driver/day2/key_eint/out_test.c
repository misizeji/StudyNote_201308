#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>


static int open_dev(const char *devname)
{
	int fd = -1;
	
	fd = open(devname, O_RDWR);
	if(fd < 0)
	{
		perror("open_dev");
		_exit(-1);
	}
	return fd;
}

int main()
{
	int gpio_fd = 0;
	int key = 0x00;
		
	gpio_fd = open_dev("/dev/key_eint");
	if(gpio_fd < 0){
		perror("open device key_eint");
		return gpio_fd;
	}
	while(1){	
		read(gpio_fd, &key, sizeof(key));
	}
	close(gpio_fd);
}