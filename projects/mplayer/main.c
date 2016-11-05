#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <pthread.h>
#include <sys/stat.h>
#include "TFT_API.h"
#include "mplayer.h"

int fd_msg[2] = {};										//定义无名管道的描述符
int main()
{
	int ret_fifo_cmd = -1;								//定义建立命名管道返回值
	int ret_fifo_msg = -1;								//定义无名管道的返回值
		
	player_display();									//调用初始化函数
	ret_fifo_cmd= mkfifo("./fifo_cmd",0666);				//播放器之间传递命令的管道
	if (ret_fifo_cmd < 0)
	{
		perror("mkfifo");
	}	
	ret_fifo_msg = pipe (fd_msg);							//建立无名管道
	if (ret_fifo_msg < 0)
	{
		perror ("pipe");
	}
	
	//创建父子进程
	pid_t pid;
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
	}

	if (pid == 0)	//子进程负责播放并检测命令
	{
		char song[100] = "";
		char song_title_path[20] = "../song/";
		dup2(fd_msg[1], 1);
		strcpy(song,strcat(song_title_path,song_name[0]));
		execl("./mplayer","mplayer","-ac","mad","-idle",
				"-slave","-quiet","-input","file=./fifo_cmd",
				song,NULL);
		perror("execl");
	}
	else		//父进程进行6个线程的同步执行
	{
		pthread_t tid_key,tid_touch,tid_msg_send_cmd,tid_msg_output,tid_msg_output_deal,tid_display_lyrics;		//建立6个线程
		
		pthread_create(&tid_key,NULL,player_key,NULL);				//按键线程
		pthread_create(&tid_touch,NULL,player_touch,NULL);			//触摸屏线程
		pthread_create(&tid_msg_send_cmd,NULL,msg_send_cmd,NULL);	//发送命令线程
		pthread_create(&tid_msg_output,NULL,msg_output,NULL);			//读取信息并处理线程
		pthread_create(&tid_msg_output_deal,NULL,msg_output_deal,NULL);	//将处理之后的信息打印detail框线程
		//pthread_create(&tid_display_lyrics,NULL,display_lyrics,NULL);	//进行实时歌词显示线程
		
		pthread_join(tid_key,NULL);
		pthread_join(tid_touch,NULL);
		pthread_join(tid_msg_send_cmd,NULL);
		pthread_join(tid_msg_output,NULL);
		pthread_join(tid_msg_output_deal,NULL);
		//pthread_join(tid_display_lyrics,NULL);
			
		wait(NULL);
	}
	return 0;
}