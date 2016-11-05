#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include "TFT_API.h"
#include "mplayer.h"


int volume_flg = 1;								//静音的标志位
int play_pause_flg = 1;							//设置播放暂停标志位

//封装打开设备函数
/*************************************************
	函数功能：封装打开设备函数
**************************************************/
int open_dev(const char *devname)
{
	int fd = -1;
	fd = open(devname, O_RDWR);
	if(fd < 0)
	{
		perror("open_dev");
		_exit(-1);
	}
	return fd;
}
//=================================================================================
// 函数介绍：	判断坐标点是否在制定区域内
// 输入参数：	触摸的坐标：pt_x,pt_y
//				矩形区域的起点（x,y），矩形区域的宽度width，高度height
// 返回值：		1:点在矩形内 0:点不在矩形内
//=================================================================================
int is_in_area(CUR_VAL ts_temp, short x, short y, short Width, short Height)
{
	if((ts_temp.current_x >= x)
	&& (ts_temp.current_x <= (x+Width))
	&& (ts_temp.current_y >= y)
	&& (ts_temp.current_y <= (y+Height))
		&& (ts_temp.key == 0))//按键抬起或按下标志,1为按下,0为抬起,仅对单点起作用
	{
		return 1;
	}
	return 0;
}
/************************************************************************
	函数功能：能进行触屏的检测，实现相应按钮功能并将贴图改变
	参数：	无
	返回值：无
*************************************************************************/
void *player_touch(void *arg)
{
	CUR_VAL val_single;							//定义触摸屏数据结构体
	
	int touch_fd = -1;
	touch_fd = open_dev("/dev/event0"); 				//打开触摸屏设备文件
	
	fd_fifo_cmd = open("./fifo_cmd",O_RDWR);			//打开管道	
	if(fd_fifo_cmd < 0)
	{
		perror("open");
	}
	
	while(1)
	{
		//单点触摸分析程序
		analysis_event_single(touch_fd, &val_single);
		//数据有效状态标志位：0为数据无效，1为数据有效
		if(val_single.state==1)
		{
			//printf("current_x=%d, current_y=%d, current_p=%d, key=%d\n",
				//val_single.current_x, val_single.current_y,
				//val_single.current_p, val_single.key);
			if(is_in_area(val_single, 30, 195, 40, 40))	//播放暂停
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
						volume_flg = 1;
				}
			}
			else if(is_in_area(val_single,90, 195, 40, 40))//快退按钮
			{
				char song_back[100] = "";
				char volume_cmd[20] = "";
				int value = -5;
				sprintf(song_back,"seek %d\n",value);						//拼接命令
				write_cmd(song_back);								//将命令写入管道
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
			else if(is_in_area(val_single,150, 195, 40, 40))//快进按钮
			{
				char song_pro[100] = "";
				char volume_cmd[20] = "";
				int value = 5;
				sprintf(song_pro,"seek %d\n",value);						//拼接命令
				write_cmd(song_pro);								//将命令写入管道
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
			else if(is_in_area(val_single,210, 195, 40, 40))//上一曲按钮
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
					list_show(play_now);			//反显
					TFT_File_Picture(DemoWindow9,  0, 0, "../picture/back_focus.bmp",1);	
					usleep(300*1000);
					TFT_File_Picture(DemoWindow9,  0, 0, "../picture/back.bmp",1);
				/*********确保播放的状态与静音图标的显示*************/
					TFT_File_Picture(DemoWindow6,  0, 0, "../picture/play.bmp",1);
					play_pause_flg = 1;
					TFT_File_Picture(DemoWindow11, 0, 0, "../picture/mute_1.bmp",1);
					volume_flg = 1;
			}
			else if(is_in_area(val_single,270, 195, 40, 40))//下一曲按钮
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
				sprintf(song_down,"loadfile ../song/%s\n",song_name[play_now]);			//拼接命令
				//printf("_________this  is  a  test_______%s\n",song_down);
				write_cmd(song_down);											//将命令写入管道
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
			else if(is_in_area(val_single,330, 195, 40, 40))//喇叭按钮静音
			{
				int volume_before = 0;		//定义变量存储之前音量
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
			else if(is_in_area(val_single,364,  45, 21, 121))	//音量显示
			{
				if (val_single.current_y >= 51 && val_single.current_y <= 151)
				{
					volume_value = (151 - val_single.current_y);
					char volume_cmd[20] = "";
					sprintf(volume_cmd,"volume %d 1\n",volume_value);
					write_cmd(volume_cmd);
					
					/*********确保播放的状态与静音图标的显示*************/
						TFT_File_Picture(DemoWindow11, 0, 0, "../picture/mute_1.bmp",1);
						volume_flg = 1;
						TFT_File_Picture(DemoWindow6,  0, 0, "../picture/play.bmp",1);
						play_pause_flg = 1;
				}
			}
			else if(is_in_area(val_single,417, 41, 367, 423))	//触屏选择歌曲
			{
				play_now = ((val_single.current_y - 41)/30);
				//printf("song_num is %d\n",play_now);
				if(play_now <= (song_num -1))
				{
					char song_play[100] = "";
					char volume_cmd[20] = "";
					sprintf(song_play,"loadfile ../song/%s\n",song_name[play_now]);
					write_cmd(song_play);									//将命令写入管道
					sprintf(volume_cmd,"volume %d 1\n",volume_value);
					write_cmd(volume_cmd);

						list_show(play_now);															//反显
					/*********确保播放的状态与静音图标的显示*************/
						TFT_File_Picture(DemoWindow6,  0, 0, "../picture/play.bmp",1);
						play_pause_flg = 1;
						TFT_File_Picture(DemoWindow11, 0, 0, "../picture/mute_1.bmp",1);
						volume_flg = 1;
				}
			}	
			if(is_in_area(val_single,51, 252, 351, 40))				
			{
				/********************创新:进度条触控*************************/
				if ((val_single.current_x >= (percent_pos*3+16+35)) && (val_single.current_x <= (300+16+35)))
				{
					char song_cmd[100] = "";
					char volume_cmd[20] = "";
					int time_value = 0;
					time_value = (int)((val_single.current_x -(percent_pos*3+35+16))*length)/300;					
					//printf("_____________pos is %d\n",percent_pos);
					sprintf(song_cmd,"seek +%d\n",time_value);					//拼接命令
					write_cmd(song_cmd);													//将命令写入管道
					sprintf(volume_cmd,"volume %d 1\n",volume_value);
					write_cmd(volume_cmd);

				/*********************图标互动****************************/
					TFT_File_Picture(DemoWindow8,  0, 0, "../picture/forward_focus.bmp",1);
					usleep(200*1000);
					TFT_File_Picture(DemoWindow8,  0, 0, "../picture/forward.bmp",1);
				/*********确保播放的状态与静音图标的显示*************/
					TFT_File_Picture(DemoWindow6,  0, 0, "../picture/play.bmp",1);
					play_pause_flg = 1;
					TFT_File_Picture(DemoWindow11, 0, 0, "../picture/mute_1.bmp",1);
					volume_flg = 1;
				}
				else if ((val_single.current_x >= 51) && (val_single.current_x < (percent_pos*3+35+16)))
				{
					char song_cmd[100] = "";
					char volume_cmd[20] = "";
					int time_value = 0;
					time_value = (((percent_pos*3+35+16) - val_single.current_x)*length)/300;					
					//printf("_____________pos is %d\n",percent_pos);
					sprintf(song_cmd,"seek -%d\n",time_value);					//拼接命令
					write_cmd(song_cmd);													//将命令写入管道
					sprintf(volume_cmd,"volume %d 1\n",volume_value);
					write_cmd(volume_cmd);

				/*********************图标互动****************************/
					TFT_File_Picture(DemoWindow7,  0, 0, "../picture/backward_focus.bmp",1);
					usleep(200*1000);
					TFT_File_Picture(DemoWindow7,  0, 0, "../picture/backward.bmp",1);
				/*********确保播放的状态与静音图标的显示*************/
					TFT_File_Picture(DemoWindow6,  0, 0, "../picture/play.bmp",1);
					play_pause_flg = 1;
					TFT_File_Picture(DemoWindow11, 0, 0, "../picture/mute_1.bmp",1);
					volume_flg = 1;
				}
				else 
				{
					continue;
				}
			}
			else 
			{
				continue;
			}
		}
	}
	close(touch_fd);
}
