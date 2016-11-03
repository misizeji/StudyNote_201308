
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	int fd[2] = {};
	int ret_pipe = 0;

	ret_pipe = pipe (fd);
	if(ret_pipe < 0)
	{
		perror("pipe");
	}

	pid_t pid;
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
	}
	
	
	if(pid == 0)
	{
		dup2(fd[1],1);
		int ret_son = 0;
		ret_son = execlp("ls","ls","--help",NULL);
		if (ret_son < 0)
		{
			perror("execlp");
		}
		exit(-1);
	} 
	else 
	{
		wait(NULL);
		close(fd[1]);
		int ret_father = 0;
		dup2 (fd[0],0);
		ret_father = execlp("more","more",NULL);
		if(ret_father < 0)
		{
			perror("execlp");
		}
	
	}
	return 0;
}
