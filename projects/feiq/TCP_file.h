#ifndef  ____TCP_FILE_H______
#define  ____TCP_FILE_H______




extern void *TCP_file(void *arg);		//TCP_file文件发送函数

//文件信息结构体
typedef struct filelist
{
	char filename[50];//文件名
	int num;// 文件序号
	long pkgnum;//包编号
	long size;//文件大小
	long ltime;//最后修改时间
	char user[10];//发送者用户名
	struct filelist *next;
	
}IPMSG_FILE;

extern IPMSG_FILE *recvfile_head;														//接收文件链表头部声明外部可用
extern IPMSG_FILE *file_link_insert(IPMSG_FILE *head, IPMSG_FILE temp);					//接收文件链表插入函数
extern IPMSG_FILE *file_link_search(IPMSG_FILE *head,long pkgnum);						//接收文件链表查询函数
extern void file_link_print(IPMSG_FILE *head);											//打印接收文件链表
extern void file_link_free(IPMSG_FILE *head);											//接收文件链表释放malloc空间
extern IPMSG_FILE *file_link_delete(IPMSG_FILE *head,long pkgnum);


#endif