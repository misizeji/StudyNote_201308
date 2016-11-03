
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>


int val_global = 11;

int main(int argc, char *argv[])
{
	pid_t pid;

	pid = vfork();

	if (pid < 0)
	{
		perror("v_fork");
	}

	if(pid == 0)
	{
		val_global++;
		printf("son val_global = %d\n",val_global);
		_exit(1);
	}
	else
	{
		printf("father val_flobal = %d\n",val_global);
		sleep(1);
	}

	return 0;
}

