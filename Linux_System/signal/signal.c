
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
int main(int argc, char *argv[])
{
	pid_t pid;
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
	}

	if (pid == 0)
	{
		while(1)
		{
			sleep(1);
			printf("hello world\n");
		}
	}
	else
	{
		sleep(3);
		kill(pid,SIGINT);
	}

	return 0;
}
