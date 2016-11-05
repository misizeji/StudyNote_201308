#ifndef _______KEYBOARD_INPUT_H________
#define _______KEYBOARD_INPUT_H________






extern IPMSG_FILE *sendfile_head;							//发送文件链表头声明外部可用
extern void broadcast(int sockfd,long CMD);					//广播函数
extern void *keyboard_input(void *arg);						//键盘线程函数

extern int auto_reply_flg;									//自动回复标志位
extern void auto_reply_msg(char *send_ip,int sockfd);		//自动回复函数


#endif