
/***************************************************************
  *Project Name:			lcdtest_mmap  
  *File Name:				lcd_user.c  
  *Description:				
  *
  *Date and Edition:		2012-11-15  v1.1
  *Author:					Valor Lion
  **************************************************************/
  
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <string.h>
#include "lcd_user.h"

#define FB_NAME		"/dev/fb0"

#define DEBUG_MSG

#ifdef 	DEBUG_MSG
#define Dprintf(fmt,arg...)			printf(fmt, ##arg)
#else
#define Dprintf(fmt,arg...)			NULL
#endif


int main(void)
{
	int i;
	int ret;
	int fb_fd;
	screen_handler_t window;
	struct fb_var_screeninfo vinfo;				//存储屏幕可变参数
	struct fb_fix_screeninfo finfo;				//存储屏幕固定参数
	pixel_data_t *aryaddr;
	
	//打开framebuffer设备
	fb_fd = open(FB_NAME, O_RDWR);
	if(fb_fd < 0){
		perror("fail to open\n");
		return -1;
	}
	
	/*获取lcd设备相关参数*/
	ret = ioctl(fb_fd, FBIOGET_FSCREENINFO, &finfo);
	if (ret != 0){
		perror("fail to get screen info");
		return -1; 
	}
	ret = ioctl(fb_fd, FBIOGET_VSCREENINFO, &vinfo);
	if (ret != 0){
		perror("fail to get screen info");
		return -1; 
	}
	
	window.width = vinfo.xres;
	window.height = vinfo.yres;
	window.bits = vinfo.bits_per_pixel;
	
	window.pixels = window.width*window.height;
	window.size = window.pixels*(window.bits>>3);
	window.line_bytes = window.width*(window.bits>>3);
	
	//debug message
	Dprintf("****************\n\
			screen width: %d\n\
			screen height: %d\n\
			bits_per_pixel: %d\n\
			show size: %d\n\
			****************\n",window.width,window.height,window.bits,window.size);
	
	//映射内存，用户程序可直接访问设备内存  
	/*动态分配，大小，权限，共享映射，文件描述符，从驱动申请的内存首地址开始映射*/
	window.addr = (unsigned char *)mmap(NULL, window.size, PROT_READ|PROT_WRITE, MAP_SHARED, fb_fd, 0);
	if(window.addr == NULL){
		perror("mmap");
		return -1;
	}
	aryaddr = (pixel_data_t*)window.addr;
	
	/*显示测试部分*/
	memset(window.addr, 255, window.size);		//显示白色
	sleep(1);
	memset(window.addr, 0, window.size);		//显示黑色
	sleep(1);

	for(i=0; i<window.pixels; i++){				//显示颜色渐变
			aryaddr[i].blue = i*255/window.pixels;
			aryaddr[i].green = i*255/window.pixels;
			aryaddr[i].red = i*255/window.pixels;
			aryaddr[i].alpha = 200;
	}
	
	munmap(window.addr,window.size);//解除映射
	close(fb_fd);
	return 0;
}
