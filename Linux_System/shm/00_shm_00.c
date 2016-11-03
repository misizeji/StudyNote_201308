
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	key_t key;
	key = ftok("./",2013);
	if (key < 0)
	{
		perror("ftok");
	}
	

	int fd = shmget(key,1024,IPC_CREAT|0666);
	if (fd < 0)
	{
		perror("shmget");
	}

	printf("the shmid is %d\n",fd);
	return 0;
}
