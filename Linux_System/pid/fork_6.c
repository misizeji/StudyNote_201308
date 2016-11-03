/* ************************************************************************
 *       Filename:  fork_4.c
 *    Description:  
 *        Version:  1.0
 *        Created:  2013年09月24日 12时14分43秒
 *       Revision:  none
 *       Compiler:  gcc
 *         Author:  YOUR NAME (), 
 *        Company:  
 * ************************************************************************/


#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

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
		int i = 0;
		for (i=0;i<5;i++)
		{
			printf("in son process\n");
			sleep(1);
		}
		exit(123);
	}
	else
	{
		int status;
		waitpid( -1,&status,0 );
		if(WIFEXITED(status))
		{
			printf("son ret val is %d\n",WEXITSTATUS(status));
		}
		printf("in father process\n");
	}
	return 0;
}


