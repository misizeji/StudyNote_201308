#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include "TFT_API.h"
#include "mplayer.h"

char tmp[1024] = "";
char name[128] = "";		//获取的文件名
char album[128] = "";		//获取的专辑名称
char artist[128] = "";		//获取的艺术家（歌手名）
int percent_pos = 0;		//获取的当前播放进度
int length = 0;				//获取的歌曲文件长度
int time_pos = 0;			//获取的当前播放的位置
int time[3] = {};

/***********************************************************************
函数功能：对管道进行实时的发送命令获取当前播放的歌曲信息
参数：	无
返回值：无
************************************************************************/
void *msg_send_cmd(void *arg)
{
	char *cmd[]={"get_file_name\n","get_meta_album\n","get_meta_artist\n",
	"get_percent_pos\n","get_time_length\n","get_time_pos\n"};		//获取歌曲信息
	
	int i = 0;
	while(1)
	 {
		for(i=0;i<6;i++)
		{
			usleep(100*1000);
			write_cmd(cmd[i]);
		}
	}
}	
/********************************************************************************
函数功能：对重定向的管道进行读取信息
参数：	无
返回值：无
*********************************************************************************/
void *msg_output(void *arg)
{
	while(1)
	{
	    usleep(100*1000);
		memset(tmp,'\0',sizeof(tmp));		
		read(fd_msg[0],tmp,sizeof(tmp));
		//set_tft_print_utf8();  
		//printf("_____%s_____",tmp);
		//set_tft_print_ansi();
		if (strstr(tmp,"ANS_FILENAME"))
		{
			bzero(name,sizeof(name));
			sscanf(tmp,"%*[^']'%[^']",name);
			//printf("_____%s_____\n",name);
		}
		else if(strstr(tmp,"ANS_META_ALBUM"))
		{
			bzero(album,sizeof(album));
			sscanf(tmp,"%*[^']'%[^']",album);
			//printf("_____%s_____\n",album);
		}
		else if(strstr(tmp,"ANS_META_ARTIST"))
		{
			bzero(artist,sizeof(artist));
			sscanf(tmp,"%*[^']'%[^']",artist);
			//printf("_____%s_____\n",artist);
		}
		else if(strstr(tmp,"ANS_PERCENT_POSITION"))
		{
			percent_pos = 0;
			sscanf(tmp,"%*[^=]=%d[^\n]",&percent_pos);
			//printf("_____%d_____\n",percent_pos);
		}
		else if(strstr(tmp,"ANS_LENGTH"))
		{
			length = 0;
			sscanf(tmp,"%*[^=]=%d[^\n]",&length);
			//printf("_____%d_____\n",length);
		}
		else if(strstr(tmp,"ANS_TIME_POSITION"))
		{
			//bzero(time_pos,sizeof(time_pos));
			time_pos = 0;
			sscanf(tmp,"%*[^=]=%d[^\n]",&time_pos);
			//tep=(int)atoi(time_pos);
			time[0]=time_pos%60;
			time[1]=time_pos/60%60;
			time[2]=time_pos/3600;
			//printf("_____%s_____\n",time_pos);
		}
		else
		{
			continue;
		}
	}
	
}

/*****************************************************************************
函数功能：	对取到并存起来的数据进行打印到detail窗口
			打印进度条
			循环播放
		  
参数：	无
返回值：无
******************************************************************************/
extern void *msg_output_deal(void *arg)
{
	while(1)
	{
		usleep(100*1000);
		TFT_File_Picture(DemoWindow2, 0, 0,"../picture/detail.bmp", 1);	
		TFT_SetColor(DemoWindow2, COLOR_WHITE);	
		set_tft_print_utf8();
		TFT_SetTextPos(DemoWindow2, 0, 10);												//设置字符在窗口打印位置
		TFT_Print(DemoWindow2, "姝屾洸:%s",name);
		set_tft_print_ansi();
		set_tft_print_utf8();
		TFT_SetTextPos(DemoWindow2, 0, 30);												//设置字符在窗口打印位置
		TFT_Print(DemoWindow2, "涓撹緫:%s",album);
		set_tft_print_ansi();
		set_tft_print_utf8();
		TFT_SetTextPos(DemoWindow2, 0, 50);												//设置字符在窗口打印位置
		TFT_Print(DemoWindow2, "鑹烘湳瀹?%s",artist);
		set_tft_print_ansi();
		set_tft_print_utf8();
		TFT_SetTextPos(DemoWindow2, 0, 70);												//设置字符在窗口打印位置
		TFT_Print(DemoWindow2, "杩涘害:%d %%",percent_pos);
		set_tft_print_ansi();
		set_tft_print_utf8();
		TFT_SetTextPos(DemoWindow2, 0, 90);												//设置字符在窗口打印位置
		TFT_Print(DemoWindow2, "鎬绘椂闂?%.2d:%.2d",(length/60),(length%60));
		set_tft_print_ansi();
		set_tft_print_utf8();
		TFT_SetTextPos(DemoWindow2, 0, 110);											//设置字符在窗口打印位置
		TFT_Print(DemoWindow2, "褰撳墠鏃堕棿:%.2d:%.2d:%.2d",time[2],time[1],time[0]);
		set_tft_print_ansi();
		/*******************进度条创新显示************************/
			TFT_File_Picture(DemoWindow13, 0, 0, "../picture/rate.bmp",1);				//重新清屏
			TFT_SetColor(DemoWindow13, COLOR_BLUE);
			TFT_Rectangle(DemoWindow13,35,10,335,12,PAINT_SOLID);
			TFT_SetColor(DemoWindow13, COLOR_GREEN);
			TFT_Rectangle(DemoWindow13,35,10,(3*percent_pos)+35,12,PAINT_SOLID);
			TFT_SetColor(DemoWindow13, COLOR_RED);
			TFT_Circle(DemoWindow13,(3*percent_pos)+35,11,7,PAINT_SOLID);
		/******************音量条显示*****************************/
			TFT_File_Picture(DemoWindow12, 0, 0, "../picture/volume.bmp",1);
			TFT_SetColor(DemoWindow12, COLOR_GREEN);
			TFT_Rectangle(DemoWindow12,6,(107-volume_value),13,107,PAINT_SOLID);
		
		
		/***********进行顺序播放*********/
		if((length - time_pos) <= 5 && (length - time_pos) > 0)
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
			sprintf(song_down,"loadfile ../song/%s\n",song_name[play_now]);	//拼接命令
			//printf("_________this  is  a  test_______%s\n",song_down);
			write_cmd(song_down);									//将命令写入管道
			sprintf(volume_cmd,"volume %d 1\n",volume_value);
			write_cmd(volume_cmd);
			/********************确保音量图标变量音量条实时显示*********************/
				TFT_File_Picture(DemoWindow12, 0, 0, "../picture/volume.bmp",1);				//重新清屏			
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
			time_pos = 0;
		}
		
	}
}