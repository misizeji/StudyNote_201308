#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include "TFT_API.h"
#include "mplayer.h"

int fd_fifo_cmd = -1;				//管道返回值（管道描述符）
pthread_mutex_t mutex;				//定义互斥锁
int volume_value = 74;				//设置全局音量变量

/*********************************************************
	函数功能：进行互斥将命令写入管道fifo_cmd的过程操作
	参数：	传进来的命令字符串
	返回值：无
**********************************************************/
void write_cmd(char cmd[1024])
{
	pthread_mutex_lock(&mutex);
	if(strstr(cmd,"get") && (play_pause_flg == 0))		//暂停时停止获取当前的歌曲信息
	{
		pthread_mutex_unlock(&mutex);
		return ;
	}
	write(fd_fifo_cmd,cmd,strlen(cmd));		//对命令管道(用于对mplayer发命令)写数据互斥操作
	pthread_mutex_unlock(&mutex);
}

/********************************************************
	函数功能：检测相应的物理按键；并将其相应的命令
			  写入管道在子进程中进行；
	参数：	无
	返回值：无
*********************************************************/
void *player_key(void *arg)
{
	int fd_dev = 0;
	fd_dev = open_dev("/dev/key4+5");				//打开设备文件
		
	fd_fifo_cmd = open("./fifo_cmd",O_RDWR);			//打开管道	
	if(fd_fifo_cmd < 0)
	{
		perror("open");
	}
	
	pthread_mutex_init(&mutex, NULL);
	while(1)
	{
		
		int read_key = 0;										//定义读取键值
		int ret_read = -1;
		ret_read = read(fd_dev,&read_key,sizeof(read_key));		//读取键值
		if (ret_read < 0)
		{
			perror("read");
			_exit(0);
		}
		printf("the key is %d\n",read_key);
			

		if (read_key == 3)		//播放或者暂停
		{
			write_cmd("pause\n");									
			if (play_pause_flg == 1)
			{
				TFT_File_Picture(DemoWindow6,  0, 0, "../picture/pause.bmp",1);
				play_pause_flg = 0;
			}
			else
			{
				TFT_File_Picture(DemoWindow6,  0, 0, "../picture/play.bmp",1);
				play_pause_flg = 1;
				/*****************将静音图标显示为放音***************/
					TFT_File_Picture(DemoWindow11, 0, 0, "../picture/mute_1.bmp",1);
					volume_flg = 1;	//设置全局标志位
			}
		}
		else	if (read_key == 5)		//前进
		{
			char song_pro[100] = "";
			char volume_cmd[20] = "";
			int value = 5;
			sprintf(song_pro,"seek %d\n",value);								//拼接命令
			write_cmd(song_pro);										//将命令写入管道
			sprintf(volume_cmd,"volume %d 1\n",volume_value);
			write_cmd(volume_cmd);

			/*********************图标互动****************************/
				TFT_File_Picture(DemoWindow8,  0, 0, "../picture/forward_focus.bmp",1);
				usleep(300*1000);
				TFT_File_Picture(DemoWindow8,  0, 0, "../picture/forward.bmp",1);
			/*********确保播放的状态与静音图标的显示*************/
				TFT_File_Picture(DemoWindow6,  0, 0, "../picture/play.bmp",1);
				play_pause_flg = 1;
				TFT_File_Picture(DemoWindow11, 0, 0, "../picture/mute_1.bmp",1);
				volume_flg = 1;
		}
		else if(read_key == 6)		//后退
		{
			char song_back[100] = "";
			char volume_cmd[20] = "";
			int value = -5;
			sprintf(song_back,"seek %d\n",value);						//拼接命令
			write_cmd(song_back);									//将命令写入管道
			sprintf(volume_cmd,"volume %d 1\n",volume_value);
			write_cmd(volume_cmd);

			/*********************图标互动****************************/
				TFT_File_Picture(DemoWindow7,  0, 0, "../picture/backward_focus.bmp",1);
				usleep(300*1000);
				TFT_File_Picture(DemoWindow7,  0, 0, "../picture/backward.bmp",1);
			/*********确保播放的状态与静音图标的显示*************/
				TFT_File_Picture(DemoWindow6,  0, 0, "../picture/play.bmp",1);
				play_pause_flg = 1;
				TFT_File_Picture(DemoWindow11, 0, 0, "../picture/mute_1.bmp",1);
				volume_flg = 1;
		}
		else if (read_key == 2)			//上一曲
		{
			char song_up[100] = "";
			char volume_cmd[20] = "";
			if (play_now == 0)
			{
				play_now = song_num - 1;
			}
			else
			{
				play_now--;	
			}	
			sprintf(song_up,"loadfile ../song/%s\n",song_name[play_now]);	//拼接命令
			//printf("_________this  is  a  test_______%s\n",song_up);
			write_cmd(song_up);									//将命令写入管道
			sprintf(volume_cmd,"volume %d 1\n",volume_value);
			write_cmd(volume_cmd);

			/*********************图标互动****************************/
				list_show(play_now);	//反显
				TFT_File_Picture(DemoWindow9,  0, 0, "../picture/back_focus.bmp",1);	
				usleep(300*1000);
				TFT_File_Picture(DemoWindow9,  0, 0, "../picture/back.bmp",1);
			/*********确保播放的状态与静音图标的显示*************/
				TFT_File_Picture(DemoWindow6,  0, 0, "../picture/play.bmp",1);
				play_pause_flg = 1;
				TFT_File_Picture(DemoWindow11, 0, 0, "../picture/mute_1.bmp",1);
				volume_flg = 1;			
		}
		else if (read_key == 4)		//下一曲
		{
			char song_down[100] = "";
			char volume_cmd[20] = "";
			if (play_now == 11)
			{
				play_now = 0;
			}
			else
			{
				play_now++;	
			}	
			sprintf(song_down,"loadfile ../song/%s\n",song_name[play_now]);				//拼接命令
			//printf("_________this  is  a  test_______%s\n",song_down);
			write_cmd(song_down);												//将命令写入管道
			sprintf(volume_cmd,"volume %d 1\n",volume_value);
			write_cmd(volume_cmd);

			/*********************图标互动****************************/
				list_show(play_now);
				TFT_File_Picture(DemoWindow10, 0, 0, "../picture/front_focus.bmp",1);	
				usleep(300*1000);
				TFT_File_Picture(DemoWindow10, 0, 0, "../picture/front.bmp",1);
			/*********确保播放的状态与静音图标的显示*************/
				TFT_File_Picture(DemoWindow6,  0, 0, "../picture/play.bmp",1);
				play_pause_flg = 1;
				TFT_File_Picture(DemoWindow11, 0, 0, "../picture/mute_1.bmp",1);
				volume_flg = 1;
		}
		else if(read_key == 9)		//增大音量
		{
			volume_value = volume_value + 3;
			if(volume_value > 100)
			{
				volume_value = 100;
			}
			write_cmd("volume +5\n");	

			/*********确保播放的状态与静音图标的显示*************/
				TFT_File_Picture(DemoWindow11, 0, 0, "../picture/mute_1.bmp",1);
				volume_flg = 1;
				TFT_File_Picture(DemoWindow6,  0, 0, "../picture/play.bmp",1);
				play_pause_flg = 1;
		}
		else if(read_key == 11)		//减少音量
		{
			volume_value = volume_value - 3;
			if(volume_value < 0)
			{
				volume_value = 0;
				TFT_File_Picture(DemoWindow11, 0, 0, "../picture/mute_0.bmp",1);		//静音图标显示静音
				volume_flg = 0;
			}
			else
			{
				write_cmd("volume -5\n");

				/*********确保播放的状态与静音图标的显示*************/
					TFT_File_Picture(DemoWindow11, 0, 0, "../picture/mute_1.bmp",1);
					volume_flg = 1;
					TFT_File_Picture(DemoWindow6,  0, 0, "../picture/play.bmp",1);
					play_pause_flg = 1;
			}
		}
		else if(read_key == 10)		//静音
		{
			int volume_before = 0;		//存储之前音量
			char volume_cmd[20] = "";
			if(volume_flg == 1)
			{
				write_cmd("mute\n");
				TFT_File_Picture(DemoWindow11, 0, 0, "../picture/mute_0.bmp",1);
				volume_before = volume_value;
				volume_value = 0;
				volume_flg = 0;
			}
			else
			{
				write_cmd("mute\n");
				TFT_File_Picture(DemoWindow11, 0, 0, "../picture/mute_1.bmp",1);
				volume_value = volume_before;
				sprintf(volume_cmd,"volume %d 1\n",volume_value);
				write_cmd(volume_cmd);
				volume_flg = 1;
				/*************取消静音时确保当前为播放状态********/
					TFT_File_Picture(DemoWindow6,  0, 0, "../picture/play.bmp",1);
					play_pause_flg = 1;
			}
		}
		else if(read_key == 8)		//退出应用
		{
			//write_cmd("stop \n");
			continue;
		}
		else
		{	
			continue;
		}
	}
	close(fd_dev);
}