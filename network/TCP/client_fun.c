#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void *client_fun(void * arg)
{
	int connfd = (int)arg;
	size_t recv_len = 0;			//接收的长度
	char recv_buf[2048] = "";		//接收字符数组
		
	//持续判断直到收完
	while((recv_len = recv(connfd, recv_buf, sizeof(recv_buf), 0)) > 0)	//接收长度 > 0 
	{	
		send(connfd, recv_buf, recv_len, 0);	//发送数据
		printf("\e[34m%s\e[0m\n",recv_buf); //测试收取数据
	}
	close(connfd);		//关闭传输套接字
	printf("\e[31mclient closed!\e[0m\n");

	return NULL;

}
