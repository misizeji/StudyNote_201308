#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>


/*******************************************************
*函数名称：获取网页给QT留言函数
*函数参数：无
*函数返回值：返回获取到的字符串
********************************************************/
char *msg_get()
{
	int fd = 0; 
	int len = 0;
	char buf[100] = "";
	
    fd = open("../../www/cgi-bin/msg_lognt",O_RDONLY);
	len = read(fd, buf, 100);
	close(fd);
    fd = open("../../www/cgi-bin/msg_lognt",O_WRONLY | O_TRUNC);
	close(fd);
	
	if(len > 0)
	{
		printf("%s\n",buf);
	}

	return buf;
}
