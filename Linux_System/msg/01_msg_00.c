
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

typedef struct 
{
	long type;
	char buf[100];
}MSG;

int main(int argc, char *argv[])
{
	key_t key;
	int msgqid;
	MSG msg;
	
	key = ftok(".",111);
	msgqid = msgget (key,IPC_CREAT|0666);
	if(msgqid < 0)
	{
		perror("msgget");
		exit(-1);
	}

	msg.type = 10;
	strcpy(msg.buf,"hello world");
	msgsnd(msgqid,&msg,sizeof(msg.buf),0);

	return 0;
}
