#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#ifdef SERVER	//使用条件编译	编译时 gcc 文件名 -o 生成名称 -D 宏参数 -Wall

int main(int argc,char *argv[])
{
	int sockfd = 0;		//定义套接字
	int err_log = 0;	//绑定监听的返回值
	int connfd  = 0;	//accept返回链接好的套接字描述符
	unsigned short port = 8000;		//端口8000
	struct sockaddr_in my_addr;		
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);	//创建套接字
	if (sockfd < 0)
	{
		perror("socket");
		exit(-1);
	}
	
	//对结构体进行赋值（函数组，端口，IP）
	bzero (&my_addr,sizeof(my_addr));	
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(port);
	my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	err_log = bind(sockfd, (struct sockaddr*)&my_addr, sizeof(my_addr));	//绑定
	if(err_log != 0)
	{
		perror("bind");
		close(sockfd);
		exit(-1);
	}
	
	err_log = listen (sockfd, 10);		//监听
	if(err_log != 0)
	{
		perror("listen");
		close(sockfd);
		exit(-1);
	}
	
	while(1)
	{
		size_t recv_len = 0;			//接收的长度
		char recv_buf[2048] = "";		//接收字符数组
		struct sockaddr_in client_addr;	//客户端结构体
		char cli_ip[INET_ADDRSTRLEN] = "";
		socklen_t cliaddr_len = sizeof(client_addr);	//传输使用IP
		
		connfd = accept(sockfd, (struct sockaddr*)&client_addr, &cliaddr_len);	//取出已连接的套接字
		if(connfd < 0)
		{
			perror("accept");
			//continue;
		}
		inet_ntop (AF_INET, &client_addr.sin_addr, cli_ip,INET_ADDRSTRLEN);
		printf("client ip = %s ,port = %d \n",cli_ip, ntohs(client_addr.sin_port));
		
		//持续判断直到收完
		while((recv_len = recv(connfd, recv_buf, sizeof(recv_buf), 0)) > 0)	//接收长度 > 0 
		{	
			send(connfd, recv_buf, recv_len, 0);	//发送数据
		}
		close(connfd);		//关闭传输套接字
		printf("client closed!\n");
		
	}
	
	close(sockfd);		//关闭创建套接字
	return 0;
}

#endif



#ifdef USR

int main(int argc,char *argv[])
{
	int sockfd = 0;		//定义套接字
	int err_log = 0;	//定义链接返回值	
	unsigned short port = 8000;	//端口
	struct sockaddr_in server_addr;
	char *server_ip = "10.220.2.40";	//服务器IP
	
	if(argc > 1)
	{
		server_ip = argv[1];
	}
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		perror("socket");
		exit(-1);
	}
	//清空并对结构体赋值（协议，端口，IP）
	bzero(&server_addr,sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	inet_pton(AF_INET, server_ip, &server_addr.sin_addr);
	
	err_log = connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));		//链接
	if(err_log != 0)
	{
		perror("connect");
		close(sockfd);
		exit(-1);
	}
	
	char send_buf[512] = "hello world ";
	char recv_buf[512] = "";
	
	send(sockfd , send_buf, strlen(send_buf), 0);	//发送数据
	recv(sockfd, recv_buf, sizeof(recv_buf), 0);	//接收数据
	printf("message:%s \n",recv_buf);				//打印接收数据
	
	close(sockfd);	//关闭创建套接字
	
	return 0;
}

#endif