/* ************************************************************************
 *       Filename:  cp.c
 *    Description:  
 *        Version:  1.0
 *        Created:  2013年09月22日 11时34分39秒
 *       Revision:  none
 *       Compiler:  gcc
 *         Author:  YOUR NAME (), 
 *        Company:  
 * ************************************************************************/



#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	int rd_fd = -1;
	int rd_ret = -1;
	int wr_fd = -1;

	if(argc >= 2)
	{
		rd_fd = open (argv[1],O_RDONLY);
		if (-1 == rd_fd)
			perror ("open");
		wr_fd = open (argv[2],O_WRONLY | O_CREAT,0777);
		if (-1 == wr_fd)
			perror ("open");
	        while(1)
        	{
				char buf[100];
                rd_ret = read(rd_fd,buf,sizeof(buf[100]));
				if (0 == rd_ret)
	            {
			    	break;
		    	}
                write(wr_fd,buf,rd_ret);
        	}
			close(rd_fd);
			close(wr_fd);
	}
	else
	{
		printf("文件地址输入错误");
		return 0;
	}
	return 0;
}
