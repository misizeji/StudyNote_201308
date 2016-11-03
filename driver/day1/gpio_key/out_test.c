#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "key_ioctl.h"

//硬件连接
//GPH0_0	LED5
//GPH0_1	LED6
//GPH0_2	LED7
//GPH0_3	LED8

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
		
	gpio_fd = open_dev("/dev/key_ctl");
	if(gpio_fd < 0){
		perror("open device key_gph");
		return gpio_fd;
	}
	/* 配置GPH3_0为输入 */
	ioctl(gpio_fd, GPH3CON_CTL, 0);
	/* 配置GPH3_0为输入 */
	ioctl(gpio_fd, GPH3DAT_CTL, 0);
	while(1){	
		read(gpio_fd, &key, sizeof(key));
	
			switch(key&0xF8)
			{
				case 0xF0:
				printf("key = 3\n");
				break;
				case 0xE8:
				printf("key = 4\n");
				break;
				case 0xD8:
				printf("key = 5\n");
				break;
				case 0xB8:
				printf("key = 6\n");
				break;
				case 0x78:
				printf("key = 7\n");
				break;
				default:
				break;
			}
		
	}
	close(gpio_fd);
}