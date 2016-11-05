#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void *client_fun(void * arg)
{
	int connfd = (int)arg;
	size_t recv_len = 0;				//接收的长度
	char recv_buf[2048] = "";			//接收字符数组
	char send_buf[512] = "";			//发送字符数组
	char head[]="HTTP/1.1 200 OK\r\n"\
		"Content-Type: text/html\r\n"\
		"\r\n";							//发送成功报文
	char err[]=	"HTTP/1.1 404 Not Found\r\n"	\
			"Content-Type: text/html\r\n"		\
			"\r\n"								\
			"<HTML><BODY>File not found</BODY></HTML>";		//发送失败报文
	
	if((recv_len = recv(connfd, recv_buf, sizeof(recv_buf), 0)) > 0)	//接收长度 > 0 
	{	
		char file_name[512] = "";		//接收到请求网页地址
		char file_path[50] = "";		//寻找的文件路径
		int open_fd = 0;        		//打开文件描述符
		
		sscanf(recv_buf,"GET /%[^ ]",file_name);//拆包
		
		printf("\e[34m%s\e[0m\n",recv_buf); //测试收取数据
		//printf("the filename is %s\n",file_name);//打印文件名称路径
		
		sprintf(file_path,"./html/%s",file_name);//组包
		//printf("the path is %s\n",file_path);	//打印文件路径
		
		open_fd = open(file_path,O_RDONLY);
		if(open_fd < 0)			//打开失败
		{
			perror("open");
			send(connfd, err, strlen(err), 0);	//发送失败报文
		}
		else		//打开成功
		{
			int length = 0;		//定义读取到的长度
			send(connfd, head, strlen(head), 0);	//发送成功报文
			while((length = read(open_fd,send_buf,sizeof(send_buf))) != 0)
			{
				send(connfd, send_buf, length, 0);
				memset(send_buf,'\0',512);
			}	
		}

	}
	close(connfd);		//关闭传输套接字
	printf("\e[31mclient closed!\e[0m\n");

	return NULL;

}
