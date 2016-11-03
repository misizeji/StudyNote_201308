#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "s5pv210-gpio.h"

/*************************************************
	函数功能：封装打开设备函数
**************************************************/
int open_dev(const char *devname)
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

/************************************************************
*函数名称:led控制函数
*函数功能：将需要的点灯led灯点亮
*函数返回值：0
*************************************************************/
int key_led_ctl(int num)
{	
	int gpio_fd = 0;
	int led = 0x00;
	int lednum = num;
	
	gpio_fd = open_dev("/dev/gpH0");
	
	ioctl(gpio_fd, GPIO_SET_PIN_OUT, 0x1);
	ioctl(gpio_fd, GPIO_SET_PIN_OUT, 0x2);
	ioctl(gpio_fd, GPIO_SET_PIN_OUT, 0x3);

	read(gpio_fd, &led, sizeof(led));

	switch(lednum)
	{
		case 1:
			led ^= 0x02;
			break;
		case 2:
			led ^= 0x04;
			break;
		case 3:
			led ^= 0x08;
			break;
		case 4:
		if((led&0x0e) == 0x0e)
		{
			led = 0x01;
		}
		else
		{
			led = 0x0e;
		}
		break;
	
		default:
			break;
	}

	write(gpio_fd,&led,sizeof(led));
	close(gpio_fd);
	return 0;
}

/******************************************************
*函数名称：检测按键函数
*函数功能：检测按键并点亮灯
*函数返回值：灯的数字
******************************************************/
int key_deal()
{
	int fd_dev = 0;
	fd_dev = open_dev("/dev/key4+5");				//打开设备文件
	
	int read_key = 0;								//定义读取键值
	int ret_read = -1;
	ret_read = read(fd_dev,&read_key,sizeof(read_key));		//读取键值
	if (ret_read < 0)
	{
		perror("read");
		_exit(0);
	}
	
	if(read_key == 2)
	{
		key_led_ctl(1);
	}
	else if(read_key == 4)
	{
		key_led_ctl(2);
	}
	else if(read_key == 5)
	{
		key_led_ctl(3);
	}
	else if(read_key == 6)
	{
		key_led_ctl(4);
	}
	else if(read_key == 3)
	{
		return read_key;
	}
	else
	{
		;
	}
	close(fd_dev);
	return read_key;
}