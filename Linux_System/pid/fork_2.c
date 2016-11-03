
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>


int val_global = 11;

int main(int argc, char *argv[])
{
	pid_t pid;

	pid = fork();
//	printf("%d\n",pid);

	if (pid < 0)
	{
		perror("fork");
	}

	if(pid == 0)
	{
		val_global++;
		printf("son val_global = %d\n",val_global);
	}
	else
	{
		sleep(1);
		printf("father val_flobal = %d\n",val_global);

	}

	return 0;
}

