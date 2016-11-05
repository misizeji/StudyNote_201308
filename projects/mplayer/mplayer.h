#ifndef __MPLAYER_H__
#define	__MPLAYER_H__

/*只要是引用了窗口来进行贴图就需要包含TFT_API.h*/

extern int fd_msg[2];				//无名管道的描述符;	(主函数中声明外部使用无名管道描述符）

/*****************************__MPLAYER_DISPLAY__**********************************/
extern WIN_HANDLE DemoWindow1; 		//背景图片
extern WIN_HANDLE DemoWindow2;		//左上角detail详情框;
extern WIN_HANDLE DemoWindow4;		//左下角的lyrics歌词显示框;
extern WIN_HANDLE DemoWindow6;		//播放按钮
extern WIN_HANDLE DemoWindow7;		//快退按钮
extern WIN_HANDLE DemoWindow8;		//快进按钮
extern WIN_HANDLE DemoWindow9;		//上一曲按钮
extern WIN_HANDLE DemoWindow10;		//下一曲按钮
extern WIN_HANDLE DemoWindow11;		//喇叭按钮
extern WIN_HANDLE DemoWindow12;		//音量显示
extern WIN_HANDLE DemoWindow13;		//进度条显示

extern char *song_name[1024];				//最多能存入1024首歌曲
extern int song_num;						//歌曲总数
extern int play_now;						//当前播放歌曲参数
extern void player_display(void);			//初始化mplayer
extern void list_show(int play_now);		//为后面切换歌曲反显调用
/***************************************************************************************/

/******************************__MPLAYER_KEY__*****************************************/
extern int fd_fifo_cmd;						//命令管道返回值（管道描述符）
extern int volume_value;					//设置全局音量变量

extern void write_cmd(char cmd[1024]);		//进行管道fifo_cmd 写命令函数
extern void *player_key(void *arg);			//检测键盘按键
/***************************************************************************************/

/*******************************__MPLAYER_TOUCH__***************************************/
extern int play_pause_flg;					//设置播放暂停标志位
extern int volume_flg;						//静音的标志位

extern int open_dev(const char *devname);	//打开设备函数声明外部可用
extern void *player_touch(void *arg);		//检测触屏函数
/****************************************************************************************/

/*******************************__MPLAYER_PROCESS__**************************************/	
extern int percent_pos;						//获取的当前播放进度
extern int length;							//获取的歌曲文件长度
extern int time_pos;						//获取的当前播放的位置
extern int length;							//获取的歌曲文件长度

extern void *msg_send_cmd(void *arg);		//给有名管道获取当前歌曲信息
extern void *msg_output(void *arg);			//从无名管道中读取出信息
extern void *msg_output_deal(void *arg);	//进行detail的框打印
/****************************************************************************************/

/********************************_LYRICS_PROCESS_H_**************************************/
extern void *display_lyrics(void *arg);			//歌词显示线程函数
/****************************************************************************************/

#endif