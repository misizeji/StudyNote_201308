
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>

int main(int argc, char *argv[])
{


	while(1)
	{
		//定义获取hostname变量
		char hostname[20] = " ";

		//打印用户的信息
		printf("[%s@",getenv("USER"));
		
		//获取主机名称并打印
		gethostname(hostname,sizeof(hostname));
		printf("%s:",hostname);
		
		//打印当前路径
		printf("%s]$",getenv("PWD"));
		
		//清空缓存区
		fflush(stdout);
	
		//定义输入命令的字符数组
		char cmd_strtok[100] = " ";

		//设置进程号
		pid_t pid;
		pid = fork();
		if(pid < 0)
		{
			perror("fork");	
		}

		if(pid == 0)
		{
			//等待获取输入的命令
			scanf("%[^\n]",cmd_strtok);

			//定义切割命令的指针数组
			char *cmd[10] = {NULL};

			//初始化一个循环变量
			int i = 0;

			//进行命令的循环切割
			cmd[0] = strtok(cmd_strtok," ");
			while(cmd[i] != NULL)
			{
				cmd[++i] = strtok (NULL," ");
			}
		
			//执行命令
			execvp(cmd[0],cmd);
		
			//检查报错
			perror("execvp");
			
			exit(0);
		}
		else
		{  
			sleep(1);
			//回收子进程资源
			wait(NULL);
		}
	}
	return 0;
}


