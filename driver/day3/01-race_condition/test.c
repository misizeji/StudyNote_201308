#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

/*Linux内核是抢占式内核，在一个系统调用未结束前，另一个系统调用也可以进入进程上下文，访问同一缓冲区*/
int main(void)
{	
	int status;
	pid_t pid;
	
	//打开文件raceStation
	int fd_driver;
	if((fd_driver = open("/dev/key_eint_race", O_RDWR)) < 0){
		printf("file open error\n");
		exit(1);
	}
	//创建子进程
 	if((pid = fork()) < 0){
		perror("fork:");
		exit(1);
	}
	else if(pid == 0){					//判断如果是子进程
		int num;
		while(1){
			read(fd_driver,&num,sizeof(num));	
			printf("the num value is <son-%d>: %d\n",getpid(), num);

		//	usleep(50*1000);
		}
		close(fd_driver);
	}else{								//判断如果是父进程
		int num;
		while(1){	
			read(fd_driver,&num,sizeof(num));	
			printf("the num value is <father-%d>: %d\n",getpid(), num);

		//	usleep(50*1000);
		}
		pid = wait(&status);
		close(fd_driver);
	}
}
