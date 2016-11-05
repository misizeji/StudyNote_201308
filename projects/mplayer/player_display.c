#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include "TFT_API.h"

//定义窗口描述符			touch、key.....函数中将引用
WIN_HANDLE DemoWindow1; 	//背景图片
WIN_HANDLE DemoWindow2;	//左上角detail详情框;
WIN_HANDLE DemoWindow4;	//左下角的lyrics歌词显示框;
WIN_HANDLE DemoWindow5;	//右边的list歌曲列表;
WIN_HANDLE DemoWindow6;	//播放按钮
WIN_HANDLE DemoWindow7;	//快退按钮
WIN_HANDLE DemoWindow8;	//快进按钮
WIN_HANDLE DemoWindow9;	//上一曲按钮
WIN_HANDLE DemoWindow10;	//下一曲按钮
WIN_HANDLE DemoWindow11;	//喇叭按钮
WIN_HANDLE DemoWindow12;	//音量显示
WIN_HANDLE DemoWindow13;	//进度条显示


char *song_name[1024] = {NULL};	//歌曲名称列表		播放下一曲时将引用
int song_num = 0;				//歌曲总数
int play_now = 0;				//初始化时当前播放歌曲

/************************************************************************************
函数功能： 进行初始化的贴图：
						左上角detail详情框;
						左中的button歌曲按钮条;
						左下角的lyrics歌词显示框;
						右边的list歌曲列表;
						播放按钮;
						快退按钮;
						快进按钮;
						上一曲按钮;
						下一曲按钮;
						喇叭按钮;
						音量显示;
参数：	无
返回值：无
**************************************************************************************/
void bmp_paste(void)
{
	WIN_HANDLE DemoWindow0; 
	WIN_HANDLE DemoWindow3;	//左中的button歌曲按钮条;
	
	//屏幕初始化
	TFT_Init("/dev/fb0");

	//创建窗口
	DemoWindow0 = TFT_CreateWindowEx(0,0,800,480,COLOR_WHITE);
	//删除窗口
	TFT_DeleteWindow(DemoWindow0);
	//创建窗口
	DemoWindow1 = TFT_CreateWindowEx(0, 0, 800, 480, COLOR_BLACK);					
	DemoWindow2 = TFT_CreateWindowEx(8, 35, 344, 135, COLOR_BLACK);	
	DemoWindow3 = TFT_CreateWindowEx(10, 190, 380, 50, COLOR_BLACK);	
	DemoWindow4 = TFT_CreateWindowEx(15, 275, 371, 191, COLOR_BLACK);	
	DemoWindow5 = TFT_CreateWindowEx(417, 41, 367, 423, COLOR_BLACK);
	
	DemoWindow6  = TFT_CreateWindowEx(30, 195, 38, 38, COLOR_BLACK);
	DemoWindow7  = TFT_CreateWindowEx(90, 195, 38, 38, COLOR_BLACK);
	DemoWindow8  = TFT_CreateWindowEx(150, 195, 38, 38, COLOR_BLACK);
	DemoWindow9  = TFT_CreateWindowEx(210, 195, 38, 38, COLOR_BLACK);
	DemoWindow10 = TFT_CreateWindowEx(270, 195, 38, 38, COLOR_BLACK);
	DemoWindow11 = TFT_CreateWindowEx(330, 195, 38, 38, COLOR_BLACK);
	DemoWindow12 = TFT_CreateWindowEx(364, 44, 19, 121, COLOR_BLACK);
	DemoWindow13 = TFT_CreateWindowEx(16, 252, 371, 23, COLOR_BLACK);
	
	//设置图片    1自动适应  0非自动适应
	TFT_File_Picture(DemoWindow1, 0, 0, "../picture/back_ground.bmp", 1);
	
	//工作窗口清屏
    TFT_ClearWindow(DemoWindow2);
	TFT_File_Picture(DemoWindow2, 0, 0, "../picture/detail.bmp", 1);	
	//设置窗口前景颜色
	TFT_SetColor(DemoWindow2, COLOR_YELLOW);
    TFT_ClearWindow(DemoWindow3);
	TFT_File_Picture(DemoWindow3, 0, 0, "../picture/button.bmp", 1);	
    TFT_ClearWindow(DemoWindow4);
	TFT_File_Picture(DemoWindow4, 0, 0, "../picture/lyrics.bmp", 1);	
	TFT_SetColor(DemoWindow2, COLOR_BLUE);
    TFT_ClearWindow(DemoWindow5);
	TFT_File_Picture(DemoWindow5, 0, 0, "../picture/list.bmp", 1);
	
	TFT_File_Picture(DemoWindow6,  0, 0, "../picture/play.bmp",1);			//播放按钮
	TFT_File_Picture(DemoWindow7,  0, 0, "../picture/backward.bmp",1);		//快退按钮
	TFT_File_Picture(DemoWindow8,  0, 0, "../picture/forward.bmp",1);		//快进按钮
	TFT_File_Picture(DemoWindow9,  0, 0, "../picture/back.bmp",1);			//上一曲按钮
	TFT_File_Picture(DemoWindow10, 0, 0, "../picture/front.bmp",1);			//下一曲按钮
	TFT_File_Picture(DemoWindow11, 0, 0, "../picture/mute_1.bmp",1);			//喇叭按钮
	TFT_File_Picture(DemoWindow12, 0, 0, "../picture/volume.bmp",1);			//音量显示
	TFT_File_Picture(DemoWindow13, 0, 0, "../picture/rate.bmp",1);			//进度条显示
}

/*************************************************************************************
函数功能：	读取目录将目录打印至屏幕list框中，并将歌曲名称存入song_name指针数组
参数：	无
返回值：无
*************************************************************************************/
void get_song_list(void)
{
	DIR *dir;				//定义目录指针
    struct dirent *dirp;			//定义目录结构体指针
	int i = 0;
	//打开目录为空时
	if((dir=opendir("../song"))==NULL)
	{
        printf("Open dir song fail\n");
        exit(1);
	}
	//打开目录不为空时直接打印
	
	while((dirp=readdir(dir))!=NULL)
	{
		if(strstr(dirp->d_name,".mp3")||strstr(dirp->d_name,".wma"))	//排除.与..格式的文件名
		{
			song_name[i] = (char*)malloc(strlen(dirp->d_name)+1);		//  ？？？为什么这样就能使用全局变量
			strcpy(song_name[i] ,dirp->d_name);						//存入song_name指针数组
			TFT_SetColor(DemoWindow5, COLOR_BLACK);
			TFT_SetTextPos(DemoWindow5, 0, 30*i);			//设置字符在窗口打印位置
			set_tft_print_utf8();    
			TFT_Print(DemoWindow5, "%2d: %s",i,dirp->d_name);
			set_tft_print_ansi();
			i++;
		}
	}
	closedir(dir);
	song_num = i;										//记录song_num歌曲总数	
/*******************检测歌曲总数是否正确*********************
	printf("the song_num is %d\n",song_num);
************************************************************/	
}

/*******************************************************
函数功能：	将当前播放的歌曲进行反显：红色
参数：	当前播放的歌曲序号
返回值：无
********************************************************/
void list_show(int play_now)
{
	int i;
	for(i = 0;i < song_num; i++)
	{
		if(i == play_now)
	    {
			TFT_SetColor(DemoWindow5,COLOR_RED);
			TFT_SetTextPos(DemoWindow5, 0,i*30);
			set_tft_print_utf8();
			TFT_Print(DemoWindow5, "%2d: %s",i,song_name[i]);
			set_tft_print_ansi();
		}
		else 
		{
			TFT_SetColor(DemoWindow5,COLOR_BLACK);
			TFT_SetTextPos(DemoWindow5, 0,i*30);
			set_tft_print_utf8();
			TFT_Print(DemoWindow5, "%2d: %s",i,song_name[i]);
			set_tft_print_ansi();
		}
	}
	
}

/*******************************************************
函数功能：进行屏幕的初始化：贴图、获取文件列表；
		  将当前的播放歌曲进行反显；
		  初始化音量图标，显示默认%74音量；
参数：	无
返回值：无
*********************************************************/		  
void player_display(void)
{	
	bmp_paste();
	get_song_list();
	list_show(play_now);	
	/******************初始化音量条图标显示********************/
		TFT_SetColor(DemoWindow12, COLOR_GREEN);
		TFT_Rectangle(DemoWindow12,6,33,13,107,PAINT_SOLID);
		
		
/******检测存放的指针数组是否正确******
	int i = 0; 
	while(song_name[i] != NULL)
	printf("%s\n",song_name[i++]);
***************************************/	
}