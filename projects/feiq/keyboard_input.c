#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include<dirent.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <signal.h>
#include "IPMSG.H"
#include "UDP_msg.h"
#include "TCP_file.h"
#include "sqlite3.h"

/* 变量在main.c中声明外部可用 */
extern IPMSG_USER HOST;

/*本函数中外部声明变量*/
int auto_reply_flg = 0;			//自动回复标志位

/****************************************************************
*函数名称：发送广播函数
*功能：向局域网内通知信息
*参数：相应的宏命令
*返回值：无
*****************************************************************/
void broadcast(int sockfd,long CMD)
{
	char send_buf[512] = "";
	struct sockaddr_in send_addr;		//发送广播的结构体
	bzero(&send_addr, sizeof(send_addr));
	send_addr.sin_family = AF_INET;
	send_addr.sin_port = htons(2425);
	send_addr.sin_addr.s_addr  = inet_addr("10.220.2.255");		//赋值广播地址
	
	int broadcast = 1;
	setsockopt(sockfd,SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast));		//广播函数
	sprintf(send_buf,"1:%d:%s:%s:%ld:%s",(int)time((time_t *)NULL),HOST.name,HOST.host,CMD,HOST.usr_name);//组包
	
	sendto(sockfd,send_buf,strlen(send_buf),0,(struct sockaddr *)&send_addr,sizeof(send_addr));		//发送广播
}

/****************************************************************
*函数名称：发送文件链表函数
*功能：发送文件的链表信息
*参数：发送文件链表信息的udp套接字
*返回值：无
****************************************************************/

IPMSG_FILE *sendfile_head = NULL;		//发送文件链表

void send_file(char *send_ip,int sockfd, char *input_filename,long file_num)
{

	char send_file_buf[2048] = "";			//发送字符串数组
	char file_path[512] = "";				//文件路径
	IPMSG_FILE sendfile_temp;				//发送文件链表结构体
	struct stat file_msg;					//文件状态信息结构体
	struct sockaddr_in client_addr;			//发送结构体
	
	bzero(&client_addr,sizeof(client_addr));	//清空	
	client_addr.sin_family = AF_INET;			
	client_addr.sin_port = htons(2425);
	inet_pton(AF_INET, send_ip, &client_addr.sin_addr);		//点分十进制转换赋值
	
	
	sprintf(file_path,"./%s",input_filename);	//组包文件路径
	int i = stat(file_path, &file_msg);			//获取文件信息
	if(i < 0)
	{
		perror("stat error");
		return ;
	}
	//发送链表结构体赋值
	strncpy(sendfile_temp.filename,input_filename,sizeof(sendfile_temp.filename));
	sendfile_temp.num = file_num;
	sendfile_temp.pkgnum = (int)time((time_t *)NULL);
	sendfile_temp.size = file_msg.st_size;
	sendfile_temp.ltime = file_msg.st_mtime;
	strncpy(sendfile_temp.user,HOST.usr_name,sizeof(sendfile_temp.user));
	
	sendfile_head = file_link_insert(sendfile_head, sendfile_temp);									//插入发送链表
	
	int send_len = sprintf(send_file_buf,"1:%ld:%s:%s:%ld:%c%lx:%s:%lx:%lx:%ld",sendfile_temp.pkgnum,HOST.name,HOST.host, \
	IPMSG_SENDMSG|IPMSG_SENDCHECKOPT|IPMSG_FILEATTACHOPT,'\0',file_num,input_filename, \
	file_msg.st_size,file_msg.st_mtime,IPMSG_FILE_REGULAR);											//组包发送接收链表
	sendto(sockfd, send_file_buf,send_len, 0,(struct sockaddr*)&client_addr, sizeof(client_addr));	//发包
	printf("\t\t\e[36msend file_msg over!!!\e[0m\n");
	return ;

}
/*****************************************************************
*函数名称：自动回复消息消息函数
*功能：自动回复消息
*参数：IP udp 套接字
*返回值：无
******************************************************************/
void auto_reply_msg(char *send_ip,int sockfd)
{
	char send_msg_buf[2048] = "";							//发送字符串数组
	char input_msg[1024] = "I am busy now ,talk later!";			
	struct sockaddr_in client_addr;							//发送结构体
	
	bzero(&client_addr,sizeof(client_addr));				//清空	
	client_addr.sin_family = AF_INET;			
	client_addr.sin_port = htons(2425);
	inet_pton(AF_INET, send_ip, &client_addr.sin_addr);		//点分十进制转换	
	
	// char cli_ip[INET_ADDRSTRLEN] = "";		//交流用户地址
	// printf("client ip = %s\n",inet_ntop(AF_INET,&client_addr.sin_addr, cli_ip, INET_ADDRSTRLEN));	//打印用户的IP
	
	sprintf(send_msg_buf,"1:%d:%s:%s:%ld:%s",(int)time((time_t *)NULL),HOST.name,HOST.host,IPMSG_SENDMSG,input_msg);//组包
	sendto(sockfd, send_msg_buf,strlen(send_msg_buf), 0,(struct sockaddr*)&client_addr, sizeof(client_addr));//发包
	printf("\t\t\e[36mauto send over!!!\e[0m\n");
	
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
	sprintf(sqlite_insert,"insert into talk values ('%s','%s','%s');",my_ip,send_ip,input_msg);
	
	sqlite3_exec(TALK,sqlite_insert,NULL,NULL,&errmsg);
	if (errmsg != NULL)
	{
		printf("%s\n",errmsg);
	}
	sqlite3_close(TALK);
	return ;
}


/*****************************************************************
*函数名称：发送消息函数
*功能：获取键盘输入发送给指定IP
*参数：IP udp套接字
*返回值：无
******************************************************************/
void send_msg(char *send_ip,int sockfd)
{
	int s_addr = 0;
	IPMSG_USER *ret_search = NULL;
	inet_pton(AF_INET, send_ip, &s_addr);
	
	if((ret_search = usr_link_search_addr(usr_head,s_addr)) != NULL)
	{
		char send_msg_buf[2048] = "";							//发送字符串数组
		char input_msg[1024] = "";								//屏幕信息输入内容
		struct sockaddr_in client_addr;							//发送结构体
		
		bzero(&client_addr,sizeof(client_addr));				//清空	
		client_addr.sin_family = AF_INET;			
		client_addr.sin_port = htons(2425);
		inet_pton(AF_INET, send_ip, &client_addr.sin_addr);		//点分十进制转换	
		
		// char cli_ip[INET_ADDRSTRLEN] = "";		//交流用户地址
		// printf("client ip = %s\n",inet_ntop(AF_INET,&client_addr.sin_addr, cli_ip, INET_ADDRSTRLEN));	//打印用户的IP
		
		printf("\t\t\e[35minput message:\e[0m");					
		fgets(input_msg,sizeof(input_msg),stdin);				//获取屏幕输入
		input_msg[strlen(input_msg)-1] = '\0';
		sprintf(send_msg_buf,"1:%d:%s:%s:%ld:%s",(int)time((time_t *)NULL),HOST.name,HOST.host,IPMSG_SENDMSG,input_msg);//组包
		sendto(sockfd, send_msg_buf,strlen(send_msg_buf), 0,(struct sockaddr*)&client_addr, sizeof(client_addr));//发包
		printf("\t\t\e[36msend over!!!\e[0m\n");
		
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
		sprintf(sqlite_insert,"insert into talk values ('%s','%s','%s');",my_ip,send_ip,input_msg);
		
		sqlite3_exec(TALK,sqlite_insert,NULL,NULL,&errmsg);
		if (errmsg != NULL)
		{
			printf("%s\n",errmsg);
		}
		sqlite3_close(TALK);
		return ;
	}
	else
	{
		printf("\t\t\e[35m用户不存在!!!\e[0m\n");
		return ;
	}
}

/******************************************************************
*函数名称：help函数
*功能：显示帮助信息
*参数：无
*返回值：无
******************************************************************/
void cmd_help(void)
{
	printf("\t\t\e[31m********************************************************\e[0m\n");
	printf("\t\t\e[32m****   help                        打印帮助信息     ****\e[0m\n");
	printf("\t\t\e[33m****   lsusr                       打印当前用户链表 ****\e[0m\n");
	printf("\t\t\e[34m****   lsrecvfile                  打印接收文件链表 ****\e[0m\n");
	printf("\t\t\e[35m****   cls                         清屏	            ****\e[0m\n");
	printf("\t\t\e[36m****   sendto:ip                   发送消息         ****\e[0m\n");
	printf("\t\t\e[31m****   sendfile:ip                 发送文件         ****\e[0m\n");
	printf("\t\t\e[32m****   getfile:pkgnum              获取文件         ****\e[0m\n");
	printf("\t\t\e[36m****   lslist		           获取当前文件目录 ****\e[0m\n");
	printf("\t\t\e[36m****   autoreply		   忙碌状态自动回复 ****\e[0m\n");
	printf("\t\t\e[36m****   cancelreply		   取消自动回复     ****\e[0m\n");
	printf("\t\t\e[36m****   changeusr		   更改用户名       ****\e[0m\n");
	printf("\t\t\e[33m****   quit                        退出             ****\e[0m\n");
	printf("\t\t\e[34m********************************************************\e[0m\n");
	printf("\t\t\e[35m****  项目组：狂奔的蜗牛        飞鸽传书  v1.0版本  ****\e[0m\n");
	printf("\t\t\e[36m****      版权所有                 仿冒必究         ****\e[0m\n");
	printf("\t\t\e[31m********************************************************\e[0m\n");
	return ;
}
/*****************************************************************
*函数名称：获取文件函数
*功能：向其他在线用户获取文件
*参数：获取的文件名称
*返回值：无
*****************************************************************/
void get_file(long pkgnum)
{
	IPMSG_FILE *ret_file_search;				//查找接收链表返回的节点
	if((ret_file_search = file_link_search(recvfile_head,pkgnum)) == NULL)	//查询接收链表
	{
		printf("\t\t\e[31mthere is no such file\e[0m\n");
	}
	else
	{
		int get_sockfd = 0;		//定义套接字
		int get_err_log = 0;	//定义链接返回值	
		int open_fd = 0;		//定义打开文件描述符
		char send_buf[512] = "";//发送接收数据包
		char recv_data_buf[2048] = "";//接收数据
		char recv_back_buf[512] = "";	//接收文件回执
		char filepath[50] = "";	//创建文件路径
		struct sockaddr_in server_addr;
	
		get_sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if (get_sockfd < 0)
		{
			perror("socket");
			exit(-1);
		}
		//清空并对结构体赋值（协议，端口，IP）
		bzero(&server_addr,sizeof(server_addr));
		server_addr.sin_family = AF_INET;
		server_addr.sin_port = htons(2425);
		
		IPMSG_USER *pi = usr_link_search_usr(usr_head,ret_file_search->user);
		if(pi != NULL)
		{
			server_addr.sin_addr.s_addr = pi->s_addr;
		}
		else
		{
			printf("\t\t用户已经下线，无法接收文件\n");
		}
		
		get_err_log = connect(get_sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));	//链接
		if(get_err_log != 0)
		{
			perror("connect");
			close(get_sockfd);
			exit(-1);
		}
	
		int send_len = sprintf(send_buf,"1:%d:%s:%s:%ld:%lx:%x:%x",(int)time((time_t *)NULL),HOST.name,HOST.host, \
			IPMSG_GETFILEDATA,ret_file_search->pkgnum,ret_file_search->num, 0);//组包,附加信息：包编号：文件序号：偏移量：断点续传)
		send(get_sockfd , send_buf, send_len, 0);					//发送请求数据包
		
		sprintf(filepath,"./%s",ret_file_search->filename);//组包
		open_fd = open(filepath,O_WRONLY | O_CREAT,0777);		//只写和创建
		if(open_fd < 0)			//打开失败
		{
			perror("open");
			close(get_sockfd);
			return ;
		}
		
		int recv_length = 0;		//接收的总长度
		int recv_len = 0;			//一次接收的长度
		while(ret_file_search->size > recv_length)
		{
			recv_len = recv(get_sockfd, recv_data_buf, sizeof(recv_data_buf), 0);
			write(open_fd,recv_data_buf,recv_len);	//对文件进行写操作
			memset(recv_data_buf,'\0',2048);
			recv_length = recv_length + recv_len;
			printf("\r已经接收%4.2f%%",(float)recv_length*100/(float)ret_file_search->size);
		}
			
		sprintf(recv_back_buf,"1:%d:%s:%s:%ld",(int)time((time_t *)NULL),HOST.name,HOST.host,IPMSG_RELEASEFILES);
		send(get_sockfd,recv_back_buf,strlen(recv_back_buf), 0);
		printf("\n\t\t\e[31mrecv over!!!\e[0m\n");
		recvfile_head = file_link_delete(recvfile_head,ret_file_search->pkgnum);			//删除接收文件链表
		close(get_sockfd);	//关闭创建套接字
	}
	return ;
}
/*****************************************************************
*函数名称：获取当前目录函数
*功能：将当前的目录文件列表打印出来
*参数：无
*返回值：无
******************************************************************/
void get_file_list(void)
{
	DIR *dir;				//定义目录指针
    struct dirent *dirp;			//定义目录结构体指针
	int i = 0;
	//打开目录为空时
	if((dir=opendir("./"))==NULL)
	{
        printf("Open dir  fail\n");
        exit(-1);
	}
	//打开目录不为空时直接打印
	while((dirp=readdir(dir))!=NULL)
	{
		printf("\t\t\e[34m文件序号:%d\t文件名：%s\e[0m\n",i,dirp->d_name);
		i++;
	}
	closedir(dir);
	
}
/*****************************************************************
*函数名称：进行键盘输入处理函数
*功能：将键盘输入的字符进行处理解析命令
*参数：udp发送文件链表信息套接字
*返回：无
******************************************************************/
void *keyboard_input(void *arg)
{
	int sockfd = (int)arg;									//接收udp套接字
	char *cmd_buf[12] = {"sendfile","sendto","getfile","lsusr","lsrecvfile",\
							"help","quit","cls","lslist","autoreply","cancelreply","changeusr"};
	cmd_help();												//打印帮助信息
	
	while(1)
	{
		char input_buf[2048] = "";							//定义输入字符数组
		char send_ip[INET_ADDRSTRLEN] = "";					//发送用户地址
		fgets(input_buf,sizeof(input_buf),stdin);			//获取键盘输入
		input_buf[strlen(input_buf)-1] = '\0';
		
		if((strncmp(input_buf,cmd_buf[0],strlen(cmd_buf[0]))) == 0)			//比较是sendfile命令
		{
			sscanf(input_buf,"%*[^:]:%[^\n]",send_ip);						//拆包得IP
			char input_filename[512] = "";									//屏幕文件信息输入
			char *filename[3] = {NULL};
			char filename_buf1[20] = "";							
			char filename_buf2[20] = "";
			char filename_buf3[20] = "";
			long file_num = 0;												//文件序号
			printf("\t\t\e[35m请输入文件名以空格隔开（最多三个）\e[0m\n");
			printf("\t\t\e[35minput filename:\e[0m");					
			fgets(input_filename,sizeof(input_filename),stdin);				//获取屏幕输入
			input_filename[strlen(input_filename)-1] = '\0';
			sscanf(input_filename,"%s %s %s",filename_buf1,filename_buf2,filename_buf3);	//也可以malloc空间避免使用三个文件名数组
			filename[0] = filename_buf1;
			filename[1] = filename_buf2;
			filename[2] = filename_buf3;
			while((filename[file_num] != NULL) && (strlen(filename[file_num]) != 0))
			{
				send_file(send_ip,sockfd,filename[file_num],file_num);		//调用发送文件链表函数
				file_num++;
			}
			file_link_print(sendfile_head);									//遍历一下发送链表
		}
		else if((strncmp(input_buf,cmd_buf[1],strlen(cmd_buf[1]))) == 0)	//比较是sendto
		{	
			sscanf(input_buf,"sendto:%s",send_ip);			//拆包得IP
			send_msg(send_ip,sockfd);						//调用发送信息函数
		}
		else if((strncmp(input_buf,cmd_buf[2],strlen(cmd_buf[2]))) == 0)	//获取getfile文件
		{
			long pkgnum = 0;
			sscanf(input_buf,"getfile:%ld",&pkgnum);
			get_file(pkgnum);
		}
		else if((strncmp(input_buf,cmd_buf[3],strlen(cmd_buf[3]))) == 0)	//对比是“lsusr”命令
		{
			usr_link_print(usr_head);						//打印用户链表
		}
		else if((strncmp(input_buf,cmd_buf[4],strlen(cmd_buf[4]))) == 0)	//对比是“lsrecvfile”命令
		{
			file_link_print(recvfile_head);					//遍历接收链表
		}
		else if((strncmp(input_buf,cmd_buf[5],strlen(cmd_buf[5]))) == 0)	//对比是“help”命令
		{
			cmd_help();										//打印帮助信息
		}
		else if((strncmp(input_buf,cmd_buf[6],strlen(cmd_buf[6]))) == 0)	//对比是“quit”命令
		{
			broadcast(sockfd,IPMSG_BR_EXIT);				//调用广播函数广播下线	
			usr_link_free(usr_head);						//释放用户链表
			file_link_free(recvfile_head);					//释放接收文件链表
			file_link_free(sendfile_head);					//释放发送文件链表
			exit(-1);
		}
		else if((strncmp(input_buf,cmd_buf[7],strlen(cmd_buf[7]))) == 0)	//对比是“cls”命令
		{
			system("clear");
		}
		else if((strncmp(input_buf,cmd_buf[8],strlen(cmd_buf[8]))) == 0)	//对比是“lslist”命令
		{
			get_file_list();
		}
		else if((strncmp(input_buf,cmd_buf[9],strlen(cmd_buf[9]))) == 0)	//对比是“autoreply”命令
		{
			auto_reply_flg = 1;
			printf("\t\t\e[34myou are in busy state now!!\e[0m\n");
		}
		else if((strncmp(input_buf,cmd_buf[10],strlen(cmd_buf[10]))) == 0)	//对比是“cancelreply”命令
		{
			auto_reply_flg = 0;
			printf("\t\t\e[34mcancel busy state !!\e[0m\n");
		}
		else if((strncmp(input_buf,cmd_buf[11],strlen(cmd_buf[11]))) == 0)	//对比是“changeusr”命令
		{
			char input_name[20] = "";
			printf("\t\t\e[35minput usrname:\e[0m");					
			fgets(input_name,sizeof(input_name),stdin);			//获取屏幕输入
			input_name[strlen(input_name)-1] = '\0';
			strcpy(HOST.usr_name,input_name);
			usr_head = usr_link_delete(usr_head,HOST.s_addr);	//删除原链表
			broadcast(sockfd,IPMSG_BR_EXIT);					//调用广播函数广播下线
			broadcast(sockfd,IPMSG_BR_ENTRY);					//调用广播函数,广播上线
			usr_head = usr_link_insert(usr_head,HOST);			//插入新的用户链表
			printf("\t\t\e[34mchange success !!\e[0m\n");
		}
		else
		{
			printf("\t\t\e[31minput wrong !!!\n\t\tPlease input help to get more option!!!\e[0m\n");
			continue;
		}
	}
	return NULL;
}
