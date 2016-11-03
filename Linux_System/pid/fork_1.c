
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	pid_t pid;
	
	pid = fork();
	if( pid < 0)
	{
		perror("fork");
	}

	if(pid == 0)
	{
		while(1)
		{
			printf("in son process\n");
			sleep(1);
		}
	}
	else
	{
		while(1)
		{
			printf("in father  process\n");
			sleep(1);
		}
	}
	return 0;
}
