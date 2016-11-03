#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
	int sockfd = 0;		//定义套接字
	char buf[1024] = "";
	unsigned short port = 8000;		//设定端口8000
	struct sockaddr_in send_addr;
	
	bzero(&send_addr, sizeof(send_addr));
	send_addr.sin_family = AF_INET;
	send_addr.sin_port = htons(port);
	
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);		//创建套接字
	if(sockfd < 0)
	{
		perror("socket");
		exit(-1);
	}
	
	if(argc > 1) 
	{
		send_addr.sin_addr.s_addr  = inet_addr(argv[1]);	//赋值广播地址
	}
	else
	{
		printf("not have the server ip !!!!\n");
		exit(-1);
	}
	
	int yes = 1;
	setsockopt(sockfd,SOL_SOCKET, SO_BROADCAST, &yes, sizeof(yes));		//广播函数
	
	//strcpy(buf,"boardcast success!!!");
	
	while(1)	//持续广播
	{
		fgets(buf,sizeof(buf),stdin);
		buf[strlen(buf)-1] = '\0';
		int len = sendto(sockfd,buf,strlen(buf),0,(struct sockaddr *)&send_addr,sizeof(send_addr));
		if(len < 0)
		{
			printf("send error!!!\n");
			close(sockfd);
			exit(-1);
		}
		printf("send success!!!!\n");
	}
	return 0;
}