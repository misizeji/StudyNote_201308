#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/time.h>

int main(int argc, char *argv[])
{
	int sockfd = 0;			//定义套接字返回描述符
	int err_log = 0;		//定义绑定返回值
	int file_fd = 0;		//定义打开文件描述符
	unsigned short port = 8000;		//定义绑定端口
	unsigned short send_port = 69;	//发送的ftp知名端口
	struct sockaddr_in my_addr;
	char send_buf[150] = "";		//发送请求字符串
	int send_len = 0;				//组包长度
	char *server_ip = NULL; 		//服务器IP
	char *file_name = NULL;			//打开或创建文件名称
		
	if(argc > 2)
	{
		server_ip = argv[1];		//地址给地址复制就行，空间给地址赋值的话就需要malloc空间
		file_name = argv[2];
	}
	
	
	//printf("_______%s________\n",server_ip);
	//printf("_______%s________\n",file_name);
	
	sockfd = socket (AF_INET, SOCK_DGRAM, 0);	//创建套接字
	if (sockfd < 0)
	{
		perror("socket");
		exit(-1);
	}
	
	bzero(&my_addr, sizeof(my_addr));
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(port);
	my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
		
	err_log = bind (sockfd, (struct sockaddr*)&my_addr, sizeof(my_addr));		//绑定自己的8000端口
	if(err_log != 0)
	{
		perror("bind");
		close(sockfd);
		exit(-1);
	}
	
	struct sockaddr_in dest_addr;
	bzero(&dest_addr, sizeof(dest_addr));
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(send_port);	//发送到ftp端口69
	inet_pton(AF_INET, server_ip, &dest_addr.sin_addr);
	
	send_buf[0] = 0x00;
	send_buf[1] = 0x01;
	send_len = sprintf(send_buf+2,"%s%c%s%c",file_name,'\0',"octet",'\0');	//命令组包
	
	//printf("___________%s___________\n",send_buf+2);
	sendto(sockfd, send_buf, send_len+2, 0,(struct sockaddr*)&dest_addr, sizeof(dest_addr));	//发送命令
	//printf("____________________001____________________\n");
	
	file_fd = open(file_name, O_CREAT | O_WRONLY,0666);	//打开或创建，只写
	
	while(1)
	{
		char recv_buf[516] = "";
		int recv_len = 0;
		struct sockaddr_in client_addr;
		socklen_t cliaddr_len = sizeof (client_addr);
		
		recv_len = recvfrom(sockfd, recv_buf, sizeof(recv_buf), 0,(struct sockaddr*)&client_addr, &cliaddr_len);//接收数据包
		//printf("__________________%d_________________\n",recv_len);
		//printf("__________%s__________\n",recv_buf+4);
		//printf("__________________%d_________________\n",recv_buf[1]);
		
		if(recv_buf[1] == 0x03)
		{
			write(file_fd,recv_buf+4,recv_len-4);	//对文件进行写操作
			if (recv_len < 516)
			{
				printf("recv over\n");	//接收完成
				break;
			}
			else 
			{
				recv_buf[1] = 0x04;
				sendto(sockfd, recv_buf,4, 0,(struct sockaddr*)&client_addr, cliaddr_len);	//发送确认
			}
		}		
		if(recv_buf[1] == 0x05)		//错误信息判断
		{
			printf("%s\n",recv_buf+4);
			printf("recv error\n");		
			break;
		}
		
	}
	
	close(file_fd);	//关闭描述符
	close(sockfd);	//关闭描述符
	
	return 0;
	
}