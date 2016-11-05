#ifndef  ___UDP_MSG_H______
#define  ___UDP_MSG_H______

extern void *UDP_msg(void *arg);		//消息处理线程函数

//用户信息结构体
typedef struct userlist
{
	int num;
	char usr_name[20];	//用户名
	char name[20];		//登录名
	char host[20];		//主机名
	int s_addr;			//IP地址(32位网络字节序)
	struct userlist *next;
}IPMSG_USER;


extern IPMSG_USER *usr_head;						//用户链表头部声明外部可用
extern void usr_link_print(IPMSG_USER *head);		//链表打印函数lsusr命令需要
extern void usr_link_free(IPMSG_USER *head);		//用户链表释放malloc空间
extern IPMSG_USER *usr_link_insert(IPMSG_USER *head, IPMSG_USER temp);		//用户链表插入函数
extern IPMSG_USER *usr_link_search_usr(IPMSG_USER *head,char *usr_name); 	//用户名查找用户链表函数(按用户名)
extern IPMSG_USER *usr_link_search_addr(IPMSG_USER *head, int s_addr);		//用户名查找用户链表函数(按IP)
extern IPMSG_USER *usr_link_delete(IPMSG_USER *head, int s_addr);			//用户链表删除函数



#endif