
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
int main(int argc, char *argv[])
{
	key_t key;
	key = ftok("./",2013);
	if(key < 0)
	{
		perror("ftok");
	}
	
	int shmid = -1;
	shmid = shmget(key,1024,IPC_CREAT|0666);
	if (shmid < 0)
	{
		perror("shmget");
	}

	char *addr = shmat(shmid ,NULL,0);
	
	printf("%s\n",addr);

	return 0;
}
