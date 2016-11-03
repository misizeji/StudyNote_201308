
#include <stdlib.h>
#include <stdio.h>


void fun(void)
{
	printf("hoho\n");
}
int main(int argc, char *argv[])
{
	pid_t pid;
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
	}

	if(pid == 0)
	{	
		printf("haha\n");
		atexit(fun);
	}
	else
	{
		sleep(1);
		printf("heihei\n");
	}
	return 0;
}
