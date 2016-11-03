/* ************************************************************************
 *       Filename:  fork_3.c
 *    Description:  
 *        Version:  1.0
 *        Created:  2013年09月24日 11时14分17秒
 *       Revision:  none
 *       Compiler:  gcc
 *         Author:  YOUR NAME (), 
 *        Company:  
 * ************************************************************************/



#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>


int main(int argc, char *argv[])
{

	pid_t pid;
	char buf[] = "a write to stdout\n";

	write(1,buf,strlen(buf));

	printf("before fork\n");

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
	}

	if (pid == 0)
	{
		printf("in son process\n");
	}
	else
	{
		sleep(1);
		printf("in father process\n");
	}
	return 0;
}


