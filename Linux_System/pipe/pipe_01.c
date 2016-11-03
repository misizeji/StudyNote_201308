

#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	int fd[2] = {};
	int ret = 0;

	ret = pipe (fd);
	if (ret < 0)
	{
		perror ("pipe");
	}

	pid_t pid;
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
	}
	
	
	
	if(pid == 0)
	{
		char buf[100] = "hello world";
		sleep(3);
		write (fd[1],buf,strlen(buf));
		exit(0);
	}
	else 
	{

//		wait(NULL);
		fcntl(fd[0],F_SETFL,O_NONBLOCK);

		while(1)
		{
			char read_buf[100] = "";
			sleep(1);
	    	read(fd[0],read_buf,sizeof(read_buf));
	    	printf("read_buf is %s\n",read_buf);
		}
	}

	return 0;
}




