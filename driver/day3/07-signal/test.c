#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

void fun(int signo)
{
	if(signo==SIGINT)
		printf("recv SIGINT\n");
	if(signo==40)
		printf("recv from driver msg_key!\n");
}

int main()
{
	char *devname = "/dev/key1_eint";
	int fd;
	unsigned char key;
	
	fd = open(devname, O_RDWR);
	if(fd < 0){
		perror("open /dev/key1_eint failed!");
		return fd;
	}
	
	signal(40,fun);
	signal(SIGINT,fun);
	while(1){
		read(fd, &key, sizeof(key)); 
		if(key)
			printf("the key = %d\n",key);
		usleep(100*1000);
	}
	close(fd);
}