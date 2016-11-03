
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <stdio.h>
int main(int argc, char *argv[])
{
	key_t key = -1;
	int msgqid = 0;

	key = ftok(".",111);
	if (key < 0)
	{
		perror("ftok");
	}

	msgqid = msgget(key,IPC_CREAT|0666);
	if (msgqid < 0)
	{
		perror("msgget");
	}
	
	printf("msgqid = %d\n",msgqid);
	return 0;
}
