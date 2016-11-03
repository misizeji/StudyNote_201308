
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>

/* peter 给 lucy 的程序  */
int main(int argc, char *argv[])
{
	//定义peter发送给lucy的管道返回值
	int ret_peter = 0;

	//定义lucy发送给peter的管道返回值
	int ret_lucy = 0;
	
	//创建双方之间的管道
	ret_peter = mkfifo("./fifo_ptl",0666);
	ret_lucy = mkfifo("./fifo_ltp",0666);
	if((ret_peter < 0) | (ret_lucy < 0))
	{
		perror("mkfifo");
	}

	//定义打开管道的文件描述符
	int fd_ptl = 0;
	int fd_ltp = 0;
	
	//打印欢迎信息
	printf("hello,i am peter\n");
//	printf("peter:");

	//打开管道
	fd_ptl = open ("./fifo_ptl",O_WRONLY);
	fd_ltp = open("./fifo_ltp",O_RDONLY);
	if((fd_ptl < 0) | (fd_ltp < 0))
	{
		perror("open");
	}

	//创建子进程多任务
	pid_t pid;
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
	}
	
	//子进程负责写peter向lucy发送信息到缓冲区
	if (pid == 0)
	{
		while(1)
		{
			//定义输入数组
			char input[1024] = "";

			//输出peter：
			printf("peter:");
		
			//获取字符输入
			fgets(input,sizeof(input),stdin);
			input[strlen(input)-1] = '\0';

			//进行写操作
			write(fd_ptl,input,strlen(input));
		}
	
	}
	else	//父进程中读取lucy发送给peter的信息
	{
		while(1)
		{
	    	//定义输出数组
		    char output [1024] = "";

	    	//定义read返回值
	        int ret_read = 0;

		    //进行读取lucy给peter的信息
	    	bzero(output,sizeof(output));
	    	ret_read = read(fd_ltp,output,sizeof(output));
		
		    //打印到perter的屏幕
	    	printf("\rlucy:%s\n",output); 
			
			printf("peter: ");
		
			fflush(stdout);
		    //判断跳出
	    	if (ret_read == 0)
	    	{
		    	break;
	    	}
		}
	}


	return 0;
}
