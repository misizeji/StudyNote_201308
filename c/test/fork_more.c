#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <pthread.h>

/********************************************************************
 编制一段程序，实现进程的管道通信：                                                                                           
	使用系统调用pipe()建立一条管道线，2个子进程分别向管道各写一句话： 
	Child process 1 is sending a message!
	Child process 2 is sending a message! 
	而父进程则从管道中读出来自于2个子进程的信息，显示在屏幕上。 
要求：父进程先接收子进程P1发来的消息，然后再接收子进程P2发来的消息。
**********************************************************************/

int main(int argc,char *argv[])
{
	int fd[2] = {};
	int ret = 0;
	//pthread_mutex_t mutex;

	ret = pipe (fd);
	if (ret < 0)
	{
		perror ("pipe");
	}

	pid_t pid1,pid2;
	pid1 = fork();
	if (pid1 < 0)
	{
		perror("fork");
	}
	
	if(pid1 == 0)
	{
		char buf[100] = "Child process 1 is sending a message!\n";
		write (fd[1],buf,strlen(buf));
	}
	else
	{
		char read_buf1[100] = "";
		//pthread_mutex_lock(&mutex);
		read(fd[0],read_buf1,sizeof(read_buf1));
		//pthread_mutex_unlock(&mutex);
		printf("read_buf is %s\n",read_buf1);
		
		pid2 = fork();
		if (pid2 < 0)
		{
			perror("fork");
		}
		
		if(pid2 == 0)
		{
			char buf[100] = "Child process 2 is sending a message!\n";
			write (fd[1],buf,strlen(buf));
		}
		else
		{
			char read_buf2[100] = "";
			//pthread_mutex_lock(&mutex);
			read(fd[0],read_buf2,sizeof(read_buf2));
			//pthread_mutex_unlock(&mutex);
			printf("read_buf is %s\n",read_buf2);
		}
	}
	
	wait(NULL);
	return 0;
}