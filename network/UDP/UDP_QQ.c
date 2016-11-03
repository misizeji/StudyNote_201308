#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/types.h>

int sockfd = 0;						//定义套接字
unsigned short port = 8000;			//通信端口定义
struct sockaddr_in my_addr;			//定义套接字结构体

void *server(void *arg)				//服务器线程
{	
	printf("waiting data from other client\n");
	while(1)
	{
		char recv_buf[512] = "";					
		char cli_ip[INET_ADDRSTRLEN] = "";
		
		struct sockaddr_in client_addr;
		socklen_t cliaddr_len = sizeof (client_addr);
		
		recvfrom(sockfd, recv_buf, sizeof(recv_buf), 0,(struct sockaddr*)&client_addr, &cliaddr_len);//接收
		printf("recvfrom ip = %s\n",inet_ntop(AF_INET,&client_addr.sin_addr, cli_ip, INET_ADDRSTRLEN));
		printf("%s\n",recv_buf);
	}
	return NULL;
}

void *usr(void *arg)			//客户端线程
{
	struct sockaddr_in dest_addr;
	char server_ip[20] = "";
	
	bzero(&dest_addr, sizeof(dest_addr));
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(port);
	
	printf("ready to send data to UDP server\nport :%d\n",port);
	while(1)
	{
		char buf[512] = "";
		char send_buf[512] = "";
		
		fgets(buf, sizeof(buf), stdin);
		send_buf[strlen(buf) - 1] = '\0';
		sscanf(buf,"%[^:]:",server_ip);
		sscanf(buf,"%*[^:]:%[^\n]",send_buf);
		
		//printf("_______________%s______________\n",server_ip);
		//printf("_______________%s______________\n",send_buf);
		
		inet_pton(AF_INET, server_ip, &dest_addr.sin_addr);		
		sendto(sockfd, send_buf, strlen(send_buf), 0,(struct sockaddr*)&dest_addr, sizeof(dest_addr));//发送
	}
	return NULL;
}

int main(int argc, char *argv[])
{	
	int err_log = 0;
	pthread_t tid_server,tid_usr;		//定义两个线程号

	sockfd = socket (AF_INET, SOCK_DGRAM, 0);	//创建套接字
	if(sockfd < 0)
	{
		perror("socket");
		exit(-1);
	}
	
	bzero(&my_addr, sizeof(my_addr));	//清空my_addr
	my_addr.sin_family = AF_INET;		//设置协议族	
	my_addr.sin_port = htons(port);		//设置端口
	my_addr.sin_addr.s_addr = htonl(INADDR_ANY);//设置默认IP
	
	err_log = bind (sockfd, (struct sockaddr*)&my_addr, sizeof(my_addr));//绑定套接字
	if(err_log != 0)
	{
		perror("bind");
		close(sockfd);
		exit(-1);
	}
	
	pthread_create(&tid_server,NULL,server,NULL);	//运行线程
	pthread_create(&tid_usr,NULL,usr,NULL);
	
	
	pthread_join(tid_server,NULL);
	pthread_join(tid_usr,NULL);
	
	close(sockfd);
	return 0;
}
