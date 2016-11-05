#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include "IPMSG.H"
#include "UDP_msg.h"
#include "TCP_file.h"
#include "keyboard_input.h"
#include "sqlite3.h"

/* 变量在main.c中声明外部可用 */

extern IPMSG_USER HOST;

/***************************************************************
*函数名称：用户链表插入函数 （有序插入，按num值插入）
*功能：将新的用户插入链表；链表按num从小到大排列
*参数：链表头与需要插入的链表
*返回值：返回链表头
***************************************************************/
IPMSG_USER *usr_link_insert(IPMSG_USER *head, IPMSG_USER temp)
{
	//printf("in the usr_link_insert\n");
	IPMSG_USER *pb=NULL, *pf=NULL, *pi=NULL;		//pf前面的，pb后面的，pi待插入的

	pi = (IPMSG_USER *)malloc(sizeof(IPMSG_USER));
	*pi = temp;		//将需要插入的结构体赋值pi
	pi->next = NULL;

	if (head == NULL)//该链表为空时
	{
		head = pi;//pi为头节点
		return head;
	}
	else//该链表不为空
	{
		pf = pb = head;
		//寻找插入点
		while ( (pi->num > pb->num) && (pb->next != NULL) ) //从小到大排序
		{
			pf = pb;
			pb = pb->next;
		}
		
		//找到插入点（存在两种情况）
		if (pi->num <= pb->num)
		{
			//如果在头部之前插入，恰好pb又是头部
			if (pb == head)
			{
				pi->next = head;
				head = pi;//让head指向新的头部
				return head;
			}
			else//链表中部插入
			{
				pf->next = pi;//pf->next保存pi的地址
				pi->next = pb;//pi->next保存pb的地址
				return head;
			}
		}
		else//pb->next == NULL(尾部插入节点)
		{
			pb->next = pi;
			return head;
		}
	}
}

/***************************************************************
*函数名称：用户链表打印函数
*功能：输入lsusr命令时需要打印当前所有的用户
*参数：链表头
*返回值：无
***************************************************************/
void usr_link_print(IPMSG_USER *head)
{
	IPMSG_USER *pb = head;
	char cli_ip[INET_ADDRSTRLEN] = "";
	//A:判断链表是否为空
	if (pb == NULL)
	{
		//printf("the link is empty\n");
		return;
	}
	else
	{
		printf("\t\t序号****用户名称***登录名****主机名********IP地址******\n");
		while (pb != NULL)//B:排除链表只有一个节点无法打印情况
		{
			printf("\t\t\e[34m%d\t%s\t    %s\t     %s\t  %s\e[0m\n",pb->num, pb->usr_name, pb->name, pb->host, \
					inet_ntop(AF_INET,&pb->s_addr, cli_ip, INET_ADDRSTRLEN));		//序号，用户名称，登录名，主机名，IP地址
			pb = pb->next;
		}
	}

	return;
}

/**************************************************************
*函数名称：用户链表查找函数（依据查找相同IP）
*功能：用于如果查找有相同的IP值则不插入链表
*参数：链表的头部，和查找的IP
*返回值：没有链表为空返回NULL，查找有返回节点；
***************************************************************/
IPMSG_USER *usr_link_search_addr(IPMSG_USER *head, int s_addr)
{
	if (head == NULL)
	{
		//printf("the link is empty\n");
		return NULL;
	}
	else
	{
		IPMSG_USER *pb = head;
		while (pb != NULL)
		{
			if (pb->s_addr == s_addr)
			{
				return pb;
			}
			pb = pb->next;
		}
	}
	return NULL;
}

/**************************************************************
*函数名称：用户链表查找函数（依据查找相同用户名）
*功能：用于如果查找有相同的用户名
*参数：链表的头部，和查找的用户名
*返回值：没有链表为空返回NULL，查找有返回节点；
***************************************************************/
IPMSG_USER *usr_link_search_usr(IPMSG_USER *head,char *usr_name)
{
	if (head == NULL)
	{
		//printf("the link is empty\n");
		return NULL;
	}
	else
	{
		IPMSG_USER *pb = head;
		while (pb != NULL)
		{
			if ((strcmp(pb->usr_name,usr_name)) == 0)
			{
				return pb;
			}
			pb = pb->next;
		}
	}
	return NULL;
}
/**************************************************************
*函数名称：用户链表删除函数（匹配IP地址删除用户数据）
*功能：用户下线之后删除相关用户数据
*参数：链表头部和删除的链表的num值
*返回值：返回更新后的链表头部
***************************************************************/
IPMSG_USER *usr_link_delete(IPMSG_USER *head, int s_addr)
{
	//A:链表是否存在
	if (head == NULL)
	{
		//printf("the link is empty\n");
		return NULL;
	}
	else//链表存在要找删除点
	{
		IPMSG_USER *pb = NULL, *pf = NULL;
		pf = pb = head;
		//B:寻找删除点
		while ((pb->s_addr != s_addr) && (pb->next != NULL))
		{
			pf = pb;
			pb = pb->next;
		}
		//找到删除点
		if(pb->s_addr == s_addr)
		{
			if(pb == head)//C:删除头部节点
			{
				head = pb->next;
				free(pb);
				return head;
			}
			else//D:中部节点与尾部节点删除方式一样
			{
				pf->next = pb->next;
				free(pb);
				return head;
			}
		}
		else
		{
			//printf("not found\n");
		}
	}
	
	return head;
}

/***************************************************************
*函数名称：链表释放malloc空间函数
*功能：将建立链表的malloc空间释放
*参数：链表头部
*返回值：无
****************************************************************/
void usr_link_free(IPMSG_USER *head)
{
	if(head == NULL)
	{
		//printf("the link is empty\n");
		return;
	}
	else
	{
		IPMSG_USER *pb = NULL;
		while(head != NULL)
		{
			pb = head;
			head = head->next;
			free(pb);
		}
	}
	return;
}


/***************************************************************
*函数名称：消息显示
*功能：显示解析后的消息
*参数：	需要显示的字符串数组地址
*返回值：无
****************************************************************/
void  msg_show(int s_addr,char *msg)
{	
	char cli_ip[INET_ADDRSTRLEN] = "";
	printf("\t\t\e[31mrecvfrom: %s\e[0m\n",inet_ntop(AF_INET,&s_addr, cli_ip, INET_ADDRSTRLEN));	//打印用户名称IP
	IPMSG_USER *pi = usr_link_search_addr(usr_head,s_addr);
	printf("\t\t\e[34m%s:%s\e[0m\n",pi->usr_name,msg);											//打印消息
	
	/**********数据库使用**********/	
	sqlite3 *TALK;
	char *errmsg = NULL;
	
	int ret = sqlite3_open("talk_msg.db",&TALK);
	if (ret !=SQLITE_OK)
	{
		perror("sqlite3_open");
	}
	
	char my_ip[INET_ADDRSTRLEN] = "";		//交流用户地址
	inet_ntop(AF_INET,&HOST.s_addr, my_ip, INET_ADDRSTRLEN);	//自己IP
	
	char sqlite_insert[512] = "";
	sprintf(sqlite_insert,"insert into talk values ('%s','%s','%s');",cli_ip,my_ip,msg);
	
	sqlite3_exec(TALK,sqlite_insert,NULL,NULL,&errmsg);
	if (errmsg != NULL)
	{
		printf("%s\n",errmsg);
	}
	sqlite3_close(TALK);
	return ;
}

/***************************************************************
*函数名称：接收UDP消息解析
*功能：对用户链表进行维护，对接收文件链表维护，进行消息显示
*参数：主函数传参
*返回值：NULL
****************************************************************/


IPMSG_USER *usr_head = NULL;		//用户链表头
IPMSG_FILE *recvfile_head = NULL;	//接收文件链表头

void *UDP_msg(void *arg)
{
	
	int sockfd = (int)arg;		//获取udp套接字
	int link_num = 1;			//链表顺序从1开始
	
	struct sockaddr_in client_addr;
	socklen_t cliaddr_len = sizeof (client_addr);
	
	while(1)
	{	
		IPMSG_USER usr_tmp;							//用户链表结构体中间变量
		IPMSG_USER *ret_usr_search;					//查找用户链表返回的节点
		IPMSG_FILE recvfile_temp;					//接收文件链表结构体
		IPMSG_FILE *ret_file_search;				//查找接收链表返回的节点
		char recv_buf[2048] = "";					//定义接收字符串
		char send_buf[2048] = "";					//发送字符串数组
		char recv_msg[1024] = "";					//接收后解析的字符串
		int pk_num = 0;								//定义包编号
 		int unsigned long cmd_mode = 0;				//定义判断命令模式
		
		bzero(&client_addr,sizeof(client_addr));
		bzero(&usr_tmp,sizeof(usr_tmp));
		recvfrom(sockfd, recv_buf, sizeof(recv_buf),0,(struct sockaddr*)&client_addr, &cliaddr_len);	//接收消息
		
		//char cli_ip[INET_ADDRSTRLEN] = "";		//交流用户地址
		//printf("client ip = %s\n",inet_ntop(AF_INET,&client_addr.sin_addr, cli_ip, INET_ADDRSTRLEN));	//打印用户的IP	
		
		//send_buf[1024] = "1:1238605487:HOST.name:HOST.host:1:HOST.usr_name";  格式参考
		sscanf(recv_buf,"%*[^:]:%d[^:]",&pk_num);									//取出包编号
		sscanf(recv_buf,"%*[^:]:%*[^:]:%[^:]:%[^:]:%ld[^:]",usr_tmp.name,usr_tmp.host,&cmd_mode);
		sscanf(recv_buf,"%*[^:]:%*[^:]:%*[^:]:%*[^:]:%*[^:]:%[^\n]",recv_msg);
		usr_tmp.num = link_num;														//链表顺序赋值给结构体序号
		usr_tmp.s_addr = client_addr.sin_addr.s_addr; 								//本身就是32位无符号整数	将IP存入结构体
	
		if(GET_MODE(cmd_mode) == IPMSG_BR_ENTRY)									//判断用户登录广播 IPMSG_BR_ENTRY 1
		{
			strncpy(usr_tmp.usr_name,recv_msg,strlen(recv_msg));					//存入用户名到tmp.usr_name
			if((ret_usr_search = usr_link_search_addr(usr_head,usr_tmp.s_addr)) == NULL)
			{
				usr_head = usr_link_insert(usr_head,usr_tmp);						//链表插入
				link_num++;															//链表顺序加1				
				sprintf(send_buf,"1:%d:%s:%s:%ld:%s",(int)time((time_t *)NULL),HOST.name,HOST.host,IPMSG_ANSENTRY,HOST.usr_name);//组包
				sendto(sockfd, send_buf,strlen(send_buf), 0,(struct sockaddr*)&client_addr, cliaddr_len);	//回复通报在线消息
			}
			else	//如果还是同一个人，不加入用户链表
			{	
				//printf("already in the link\n");
			}
		}
		else if(GET_MODE(cmd_mode) == IPMSG_ANSENTRY)								//判断用户发送通报在线消息 IPMSG_ANSENTRY 3
		{
			strncpy(usr_tmp.usr_name,recv_msg,strlen(recv_msg));					//存入用户名
			if((ret_usr_search = usr_link_search_addr(usr_head,usr_tmp.s_addr)) == NULL)
			{
				usr_head = usr_link_insert(usr_head,usr_tmp);						//链表插入
				link_num++;															//链表顺序加1				
			}
		}
		else if(GET_MODE(cmd_mode) == IPMSG_BR_EXIT)								//判断用户退出 IPMSG_BR_EXIT 2
		{
			usr_head = usr_link_delete(usr_head,usr_tmp.s_addr);					//删除链表
			//printf("\033[s\033[20,0H%s已经下线\033[u",recv_msg);
		}
		else if(cmd_mode == (IPMSG_SENDMSG|IPMSG_SENDCHECKOPT|IPMSG_FILEATTACHOPT))	//判断接收文件链表信息
		{
			char *p = memchr(recv_buf,0,sizeof(recv_buf));
				sscanf(p+1,"%d:%[^:]:%lx:%lx",&recvfile_temp.num,recvfile_temp.filename, \
					&recvfile_temp.size,&recvfile_temp.ltime);							//解析出文件序号，文件名，文件大小，最后修改时间
			if((ret_file_search = file_link_search(recvfile_head,pk_num)) == NULL)		//查询接收链表
			{
				printf("\t\t\e[31msomeone asking for recvfile\e[0m\n");
				recvfile_temp.pkgnum = pk_num;											//赋值包编号
				IPMSG_USER *pi = usr_link_search_addr(usr_head,usr_tmp.s_addr);			//查询用户名
				strncpy(recvfile_temp.user,pi->usr_name,sizeof(recvfile_temp.user));	//拷贝用户名赋值给接收链表
				recvfile_head = file_link_insert(recvfile_head, recvfile_temp);			//插入接收链表
				sprintf(send_buf,"1:%d:%s:%s:%ld:%d",(int)time((time_t *)NULL),HOST.name,HOST.host,IPMSG_RECVMSG,pk_num);//组包回执
				sendto(sockfd, send_buf,strlen(send_buf), 0,(struct sockaddr*)&client_addr, cliaddr_len);			//发送回执消息
				file_link_print(recvfile_head);
			}
		}		
		else if(cmd_mode == (IPMSG_SENDMSG|IPMSG_SENDCHECKOPT))						//判断获取信息需要回执(高版本需要)
		{	
			msg_show(usr_tmp.s_addr,recv_msg);										//显示信息
			sprintf(send_buf,"1:%d:%s:%s:%ld:%d",(int)time((time_t *)NULL),HOST.name,HOST.host,IPMSG_RECVMSG,pk_num);//组包回执
			sendto(sockfd, send_buf,strlen(send_buf), 0,(struct sockaddr*)&client_addr, cliaddr_len);				//发送回执消息
			if(auto_reply_flg == 1)
			{
				char send_ip[INET_ADDRSTRLEN] = "";		//交流用户地址
				inet_ntop(AF_INET,&client_addr.sin_addr, send_ip, INET_ADDRSTRLEN);	//用户的IP
				auto_reply_msg(send_ip,sockfd);
			}
			else
			{
				continue;
			}
		}
		else if(GET_MODE(cmd_mode) == IPMSG_SENDMSG)								//判断获取信息不需要回执
		{	
			msg_show(usr_tmp.s_addr,recv_msg);										//显示信息
		}
		else 
		{
			continue;
		}
	}	
	return NULL;
}