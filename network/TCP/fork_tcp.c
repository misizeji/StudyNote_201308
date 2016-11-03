#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "client_fun.h"


int main(int argc, char *argv[])
{
	int sockfd = 0;
	int err_log = 0;
	unsigned short port = 8000;		//端口8000
	struct sockaddr_in my_addr;		
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);	//创建套接字
	if (sockfd < 0)
	{
		perror("socket");
		exit(-1);
	}
	
	//对结构体进行赋值（函数组，端口，IP）
	bzero(&my_addr,sizeof(my_addr));	
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
		char cli_ip[INET_ADDRSTRLEN] = "";
		struct sockaddr_in client_addr;	//客户端结构体
		socklen_t cliaddr_len = sizeof(client_addr);	//传输使用IP
		
		int connfd = accept(sockfd, (struct sockaddr*)&client_addr, &cliaddr_len);	//取出已连接的套接字
		if(connfd < 0)
		{
			perror("accept");
			continue;
		}
		inet_ntop (AF_INET, &client_addr.sin_addr, cli_ip,INET_ADDRSTRLEN);
		printf("\e[32mclient ip = %s\e[0m ,\e[33mport = %d\e[0m\n",cli_ip, ntohs(client_addr.sin_port));//多彩显示
		if (fork() == 0)
		{
			close(sockfd);
			client_fun((void *)connfd);	//调用函数中已经有close(connfd);
			//close(connfd);
			exit(0);
		}
		close(connfd);
	}
	close(sockfd);
	return 0;

}
