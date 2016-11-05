#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <sys/types.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include "IPMSG.H"
#include "TCP_file.h"
#include "UDP_msg.h"
#include "keyboard_input.h"


/*  变量在main.c中声明外部可用  */

IPMSG_USER HOST;

static int sockfd = 0;	/*定义套接字，给（检测Ctrl+C组合键）函数使用，主函数中线程传参 */

/*******************************************************************
*函数名称：检测Ctrl+C组合键
*功能：当强制结束进程时，下线发送广播
*参数：套接字
*返回值：无
*******************************************************************/
void game_over(int argc)
{
	broadcast(sockfd,IPMSG_BR_EXIT);				//调用广播函数广播下线	
	usr_link_free(usr_head);						//释放用户链表
	file_link_free(recvfile_head);					//释放接收文件链表
	file_link_free(sendfile_head);					//释放发送文件链表
	exit(-1);
}

/*******************************************************************
*函数名称：飞鸽传书
*功能：能进行UDP消息发送，局域聊天，发送文件，文件夹
*参数：无
*返回值：无
********************************************************************/

int main(int argc, char *argv[])
{
	int err_log = 0;											//绑定套接字返回值
	unsigned short port = 2425;									//端口2425
	struct sockaddr_in my_addr;									//发送的结构体
	
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);					//创建套接字
	if(sockfd < 0)
	{
		perror("socket");
		exit(-1);
	}

	bzero(&my_addr, sizeof(my_addr));
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(port);
	my_addr.sin_addr.s_addr  = htonl(INADDR_ANY);				//赋值本机地址（居然是0）
	
	err_log = bind (sockfd, (struct sockaddr*)&my_addr, sizeof(my_addr));  //绑定
	if(err_log != 0)
	{
		perror("bind");
		close(sockfd);
		exit(-1);
	}
	
	
	inet_pton(AF_INET, "10.220.2.45", &HOST.s_addr);	//不会取值本机地址
	
	printf("\t\t\t\t\e[33minput your login_name:");							//输入登录名称
	fgets(HOST.name,sizeof(HOST.name),stdin);
	HOST.name[strlen(HOST.name)-1] = '\0';
	printf("\t\t\t\t\e[36mtinput your host_name:");							//输入主机名称
	fgets(HOST.host,sizeof(HOST.host),stdin);
	HOST.host[strlen(HOST.host)-1] = '\0';
	printf("\t\t\t\t\e[35minput your usr_name:\e[0m");						//输入用户名称
	fgets(HOST.usr_name,sizeof(HOST.usr_name),stdin);
	HOST.usr_name[strlen(HOST.usr_name)-1] = '\0';
	
	broadcast(sockfd,IPMSG_BR_ENTRY);											//调用广播函数,广播上线
	
	pthread_t tid_UDP_msg,tid_keyboard_input, tid_TCP_file;						//定义三个线程

	pthread_create( &tid_UDP_msg, NULL, (void *)UDP_msg, (void *)sockfd);					//UDP消息线程
	pthread_create( &tid_keyboard_input, NULL, (void *)keyboard_input, (void *)sockfd);		//键盘输入线程
	pthread_create( &tid_TCP_file, NULL,(void *)TCP_file, NULL);							//TCP传输文件线程

	signal(SIGINT,game_over);		//Ctrl+C 信号处理
	
	pthread_join(tid_UDP_msg,NULL);
	pthread_join(tid_keyboard_input,NULL);
	pthread_join(tid_TCP_file,NULL);
	
	close(sockfd);
	return 0;
}
