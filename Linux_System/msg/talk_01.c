#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct msg
{
	long type;
	char name[20];
	char buf[1024];
}MSG;

#ifdef  PETER
char write_name[] = "peter";
long rcv = 'P';
#endif  

#ifdef  LUCY
char write_name[] = "lucy"; 
long rcv = 'L';
#endif

#ifdef BOB
char write_name[] = "bob";
long rcv = 'B';
#endif

int main(int argc, char *argv[])
{
	//定义获取唯一IPC值
	key_t key;

	//消息队列的标识符
	int msgqid;

	//定义新的结构体msg
	MSG msg_send, msg_rcv;
	
	//获取IPC值
	key = ftok (".",2013);
	if (key < 0)
	{
		perror("ftok");
	}

	//创建消息队列
	msgqid = msgget(key,IPC_CREAT|0666);
	if (msgqid < 0)
	{
		perror("msgget");
	}
	
	//创建父子进程
	pid_t pid;
	pid = fork();
	if(pid < 0)
	{
		perror("fork");
	}
	
	if(pid == 0)		//子进程操作：发送信息
	{
		while(1)
		{
			//打印提示符
			printf("%s: ",write_name);
			fflush(stdout);
			
			//定义获取输入的字符数组
			char input[1024] = "";

			//获取输入的发送信息
			fgets(input,sizeof(input),stdin);
			input[strlen(input)-1] = '\0';
	
			//进行字符串解析进行发送
			if (input[0] == 'P' || input[0] == 'L' || input[0] == 'B' && input[0] != rcv)
			{
				//给类型赋值
				msg_send.type = input[0];
				strcpy(msg_send.buf,input+2);
				strcpy(msg_send.name,write_name);
				msgsnd(msgqid,&msg_send,sizeof(msg_send.name)+sizeof(msg_send.buf),0);
			}
		}
	}
	else		//父进程读取另外两个人发送给自己的信息
	{
		while(1)
		{
			msgrcv(msgqid,&msg_rcv,sizeof(msg_rcv.name)+sizeof(msg_rcv.buf),rcv,0);
			//取出send_who[0]发过来的信息
			printf("\r%s: %s\n",msg_rcv.name,msg_rcv.buf);
			printf("%s: ",write_name);
			fflush(stdout);
		}
	}
	return 0;
	
}

