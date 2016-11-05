#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include "IPMSG.H"
#include "TCP_file.h"
#include "keyboard_input.h"

/***************************************************************
*函数名称：接收文件链表插入函数(无序链表)
*功能：记录接收文件链表，并维护链表
*参数：链表头节点
*返回值：链表头地址
***************************************************************/
IPMSG_FILE *file_link_insert(IPMSG_FILE *head, IPMSG_FILE temp)			
{
	//printf("int the file_link_insert\n");
	IPMSG_FILE *pb=NULL, *pi=NULL;		//pb后面的，pi待插入的

	pi = (IPMSG_FILE *)malloc(sizeof(IPMSG_FILE));
	*pi = temp;		//将需要插入的结构体赋值pi
	pi->next = NULL;

	if (head == NULL)//该链表为空时
	{
		head = pi;//pi为头节点
		return head;
	}
	else//该链表不为空
	{
		pb = head;
		//寻找最后插入点pb->next 为空时跳出
		while(pb->next != NULL)
		{
			pb = pb->next;
		}
		pb->next = pi;	//将pi挂到最后pb的后面
	}
	return head;
}

/***************************************************************
*函数名称：接收文件链表打印函数
*功能：输入lsfile命令时需要打印当前所有的用户
*参数：链表头
*返回值：无
***************************************************************/
void file_link_print(IPMSG_FILE *head)
{
	IPMSG_FILE *pb = head;
	//A:判断链表是否为空
	if (pb == NULL)
	{
		printf("the file_link is empty\n");
		return;
	}
	else
	{
		printf("\t\t文件序号*****文件名*****包编号********文件大小*****最后修改时间***用户名\n");
		while (pb != NULL)//B:排除链表只有一个节点无法打印情况
		{
			//文件序号、打印文件名、包编号、文件大小、最后修改时间、发送者用户名
			printf("\t\t\e[34m%d\t     %s\t%ld\t%ld\t    %ld\t  %s\e[0m\n",pb->num,pb->filename,pb->pkgnum,pb->size,pb->ltime,pb->user);
			pb = pb->next;
		}
	}
	return;
}

/**************************************************************
*函数名称：接收文件链表查找函数（依据相同的文件名和修改时间）
*功能：（依据相同的文件名和修改时间）
*参数：链表头节点，文件名，修改时间
*返回值：没有链表为空返回NULL，查找有返回节点；
***************************************************************/
IPMSG_FILE *file_link_search(IPMSG_FILE *head,long pkgnum)
{
	if (head == NULL)
	{
		//printf("the link is empty\n");
		return NULL;
	}
	else
	{
		IPMSG_FILE *pb = head;
		while (pb != NULL)
		{
			if (pb->pkgnum == pkgnum)
			{
				return pb;
			}
			pb = pb->next;
		}
	}
	return NULL;
}
/***************************************************************
*函数名称：接收文件链表删除函数
*功能：将已经接收到的文件链表删除
*参数：链表头节点文件名，修改时间
*返回值：链表的头节点
****************************************************************/
IPMSG_FILE *file_link_delete(IPMSG_FILE *head,long pkgnum)
{
	//A:链表是否存在
	if (head == NULL)
	{
		//printf("the link is empty\n");
		return NULL;
	}
	else//链表存在要找删除点
	{
		IPMSG_FILE *pb = NULL, *pf = NULL;
		pf = pb = head;
		while (1)//寻找删除点
		{
			//用两个条件判断是相同的文件接收链表
			if(pb->pkgnum == pkgnum)	
			{
				if(pb == head)//C:删除头部节点
				{
					head = pb->next;
					free(pb);
					break;
				}
				else//D:中部节点与尾部节点删除方式一样
				{
					pf->next = pb->next;
					free(pb);
					break;
				}
			}
			else if(pb->next == NULL)
			{
				//printf("not found\n");
				break;
			}
			pf = pb;
			pb = pb->next;
		}
		//找到删除点
	}
	return head;
}
/***************************************************************
*函数名称：链表释放malloc空间函数
*功能：将建立链表的malloc空间释放
*参数：链表头节点
*返回值：无
****************************************************************/
void file_link_free(IPMSG_FILE *head)
{
	if(head == NULL)
	{
		//printf("the link is empty\n");
		return;
	}
	else
	{
		IPMSG_FILE *pb = NULL;
		while(head != NULL)
		{
			pb = head;
			head = head->next;
			free(pb);
		}
	}
	return;
}
/********************************************************************
*函数名称：文件发送线程函数
*功能：将需要问价发送给请求客户端
*参数：已经连接的套接字
*返回值：无
*********************************************************************/
void *sendfile(void *arg)
{
	int connfd = (int)arg; 
	char recv_buf[2048] = "";
	char send_buf[2048] = "";
	long cmd_mode = 0;
	long pkgnum = 0;
	
	recv(connfd, recv_buf, sizeof(recv_buf), 0);
	//printf("recvbuf is %s\n",recv_buf);
	sscanf(recv_buf,"%*[^:]:%*[^:]:%*[^:]:%*[^:]:%ld:%lx:",&cmd_mode,&pkgnum);	//取出命令字与包编号
	if(cmd_mode == IPMSG_GETFILEDATA)
	{
		IPMSG_FILE *ret_file_search;				//查找发送链表返回的节点
		if((ret_file_search = file_link_search(sendfile_head,pkgnum)) != NULL)	//查询发送链表
		{
			int open_fd = open(ret_file_search->filename, O_RDONLY);
			if(open_fd < 0)
			{
				perror("open");
				close(connfd);
				return NULL;
			}
			
			int length = 0;		//定义读取到的长度
			while((length = read(open_fd,send_buf,sizeof(send_buf))) != 0)
			{
				send(connfd, send_buf, length, 0);
				memset(send_buf,'\0',2048);
			}
			sendfile_head = file_link_delete(sendfile_head,pkgnum);
			printf("\t\t\e[34msend file over!!!\e[0m\n");
		}
		else
		{
			close(connfd);
			return NULL;
		}
	}
	
	close(connfd);
	return NULL;
}
/********************************************************************
*函数名称：TCP文件接收函数
*功能：创建TCP套接字传送文件
*参数：
*返回值：
********************************************************************/
void *TCP_file(void *arg)
{
	int send_sockfd = 0;		//定义发送套接字
	int send_err_log = 0;		//绑定监听的返回值
	unsigned short port = 2425;	
	struct sockaddr_in my_addr;		
	
	send_sockfd = socket(AF_INET, SOCK_STREAM, 0);	//创建套接字
	if (send_sockfd < 0)
	{
		perror("socket");
		exit(-1);
	}
	
	//对结构体进行赋值（函数组，端口，IP）
	bzero (&my_addr,sizeof(my_addr));	
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(port);
	my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	send_err_log = bind(send_sockfd, (struct sockaddr*)&my_addr, sizeof(my_addr));	//绑定
	if(send_err_log != 0)
	{
		perror("bind");
		close(send_sockfd);
		exit(-1);
	}
	
	send_err_log = listen (send_sockfd, 10);//监听
	if(send_err_log != 0)
	{
		perror("listen");
		close(send_sockfd);
		exit(-1);
	}
	
	while(1)
	{
		int connfd  = 0;				//accept返回链接好的套接字描述符
		struct sockaddr_in client_addr;	//客户端结构体
		socklen_t cliaddr_len = sizeof(client_addr);	//传输使用IP
		connfd = accept(send_sockfd, (struct sockaddr*)&client_addr, &cliaddr_len);	//取出已连接的套接字
		if(connfd < 0)
		{
			perror("accept");
			continue;
		}
		
		pthread_t tid_sendfile;
		pthread_create(&tid_sendfile, NULL, (void *)sendfile, (void *)connfd);//调用线程函数
		pthread_detach(tid_sendfile);
	}
	
	close(send_sockfd);		//关闭创建发送套接字
	return NULL;
}