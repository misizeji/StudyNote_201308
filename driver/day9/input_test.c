#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>

int main(int argc, char *argv[])
{
	int fd = -1;
	int i = 0;
	const char *device_name = NULL;
	struct input_event rd_evt;
	int pos[2] = {0};
	
	device_name = (argc==1) ? "/dev/event0" : argv[1];
	
	printf("device_name=%s\n", device_name);
	
	if ((fd = open(device_name, O_RDWR)) < 0) {
		printf("Usage: elf event\n");
		_exit(-1);
	}
	
	while(1)
	{
		//printf("%s\n",__func__);
		if (read(fd,&rd_evt,sizeof(struct input_event)) <= 0) {
			perror("read");
			_exit(-1);
		}
		
		if((rd_evt.code == 0)&&(rd_evt.value > 0))
		{
			pos[0] = 0;
			pos[1] = 0;
			pos[0] = rd_evt.value;
		}
		else if((rd_evt.code == 1)&&(rd_evt.value > 0))
		{
			pos[1] = rd_evt.value;
			printf("the position is x = %d y = %d\n",pos[0],pos[1]);	
		}			
		usleep(100);
	}
	close(fd);
	return 0;
}
