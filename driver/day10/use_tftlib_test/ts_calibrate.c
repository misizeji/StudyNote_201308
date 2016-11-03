#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <pthread.h>
#include <semaphore.h>
#include <dirent.h>
#include <linux/input.h>

#include "TFT_API.h"
//#include "TimerTick.h"

#define DEBUG_TEST
//#undef DEBUG_TEST

#ifdef DEBUG_TEST
#define dbg(x...) printf(x)
#else
#define dbg(x...)
#endif

#define SCR_WIDTH        800
#define SCR_HEIGHT       480
#define RADIUS_FRINGE    3	//校准圆周与触模边界距离
#define CIRCLE_RADIUS    10 //校准点半径
#define SCR_FRINGE       (RADIUS_FRINGE+CIRCLE_RADIUS)	//圆心到边界的距离
#define CIRCLE_X1        SCR_FRINGE	                //第1个点圆心坐标
#define CIRCLE_Y1        SCR_FRINGE	                //第1个点圆心坐标
#define CIRCLE_X2        SCR_FRINGE	                //第2个点圆心坐标
#define CIRCLE_Y2        (SCR_HEIGHT-SCR_FRINGE)	//第2个点圆心坐标
#define CIRCLE_X3        SCR_WIDTH-SCR_FRINGE	    //第3个点圆心坐标
#define CIRCLE_Y3        SCR_HEIGHT-SCR_FRINGE	    //第3个点圆心坐标
#define CIRCLE_X4        SCR_WIDTH-SCR_FRINGE	    //第4个点圆心坐标
#define CIRCLE_Y4        SCR_FRINGE	                //第4个点圆心坐标
#define CIRCLE_X5        (SCR_WIDTH >> 1)	        //第5个点圆心坐标
#define CIRCLE_Y5        (SCR_HEIGHT >> 1)	        //第5个点圆心坐标

#define START_XY(x)       ((x)-CIRCLE_RADIUS) //画线起始坐标
#define END_XY(x)         ((x)+CIRCLE_RADIUS) //画线结束坐标

#define TS_UP			  0
#define TS_DOWN           1 

 struct TS_RET{
  unsigned short pressure;
  unsigned short x;
  unsigned short y;
  unsigned short pad;
} ts;   



extern int ts_read(struct TS_RET *rts);
extern int ts_cal_init(void);
   


 struct ts_cal_fringe{
	unsigned int touch_calibrate_lb;// 左边缘对应的默认AD转换值(16bit)
	unsigned int touch_calibrate_rb;// 右边缘对应的默认AD转换值(16bit)
	unsigned int touch_calibrate_tb;// 上边缘对应的默认AD转换值(16bit)
	unsigned int touch_calibrate_bb;// 下边缘对应的默认AD转换值(16bit)		
} tcf;

static int ca_tp[5][2];	//保存五点AD值
static int ts_fd;
static WIN_HANDLE  Main_Frame;
static int show_event(struct input_event* event)
{
	int flag=0;
	static int current_x,current_y,current_p;
	printf("%d %d %d\n", event->type, event->code, event->value);
	
	if (event->type==EV_ABS)
	{
		switch (event->code)
		{
			case ABS_X:
				ts.x = event->value;
				break;
			case ABS_Y:
				ts.y = event->value;
				break;
			case ABS_PRESSURE:
				ts.pressure = event->value;
				break;
		}
	}
	else if(event->type==EV_SYN)
	{
		flag=1;
		printf("current_x=%d,current_y=%d,current_p=%d\n", ts.x, ts.y, ts.pressure);
		current_x=current_y=current_p=0;
	}
	return flag;
}
//获取点击平均AD值，只有当抬起的时候才返回AD值
static int ad_read_26()
{
	int x_down_start = 0,y_down_start = 0;
	int x_down_end = 0,y_down_end = 0;
	int flag=0,flag_first = 0;
	int count_num = 1;
	int ret = 0;
	int con_num = 0;
	struct input_event event = {{0}, 0};
	while(1)
	{
		//ret = read(ts_fd,&ts,sizeof(ts));
		flag=0;
		int ret = read(ts_fd, &event, sizeof(event));
		if(ret == sizeof(event))
		{	
			flag=show_event(&event);
			if(flag==0)
				continue;
		}
		else
		{
			break;
		}
	
		if(ts.pressure == TS_DOWN && flag_first == 0)
		{
			x_down_start = ts.x;
			y_down_start = ts.y;
			flag_first = 1;
		}
		else if(ts.pressure == TS_DOWN && flag_first == 1)
		{
			count_num++;
			if(count_num > 2 && count_num < 6)
			{
				x_down_end += ts.x;
				y_down_end += ts.y;	
				con_num++;
				dbg("\nx_d=%d,y_d=%d,cont=%d\n",x_down_end,y_down_end,con_num);
			}	
			flag_first = 1;
		}		
		else if(ts.pressure == TS_UP)
		{
			if(con_num > 0 && con_num <3)
			{
				ts.x = x_down_end/con_num;
				ts.y = y_down_end/con_num;
			}
			else if(con_num >=3)
			{
				ts.y = x_down_end/3; //在驱动里x,y相反
				ts.x = y_down_end/3;			
			}
			else
			{
				ts.x = 0;
				ts.y = 0;			
			}
			return 0;
		}
	}

}

//获取点击平均AD值，只有当抬起的时候才返回AD值
static int ad_read()
{
	int x_down_start = 0,y_down_start = 0;
	int x_down_end = 0,y_down_end = 0;
	int flag_first = 0;
	int count_num = 1;
	int ret = 0;
	int con_num = 0;
	
	while(1)
	{
		ret = read(ts_fd,&ts,sizeof(ts));
		if(ret<0)
		{
			printf("ts_read error!!\n");
			continue;  
		}
		if(ts.pressure == TS_DOWN && flag_first == 0)
		{
			x_down_start = ts.x;
			y_down_start = ts.y;
			flag_first = 1;
		}
		else if(ts.pressure == TS_DOWN && flag_first == 1)
		{
			count_num++;
			if(count_num > 2 && count_num < 6)
			{
				x_down_end += ts.x;
				y_down_end += ts.y;	
				con_num++;
				dbg("\nx_d=%d,y_d=%d,cont=%d\n",x_down_end,y_down_end,con_num);
			}	
			flag_first = 1;
		}		
		else if(ts.pressure == TS_UP)
		{
			if(con_num > 0 && con_num <3)
			{
				ts.x = x_down_end/con_num;
				ts.y = y_down_end/con_num;
			}
			else if(con_num >=3)
			{
				ts.y = x_down_end/3; //在驱动里x,y相反
				ts.x = y_down_end/3;			
			}
			else
			{
				ts.x = 0;
				ts.y = 0;			
			}
			return 0;
		}
	}

}
//画圆心十字线
int draw_line(unsigned int cx,unsigned int cy)
{
	TFT_Line(Main_Frame, START_XY(cx), cy, END_XY(cx), cy);
	TFT_Line(Main_Frame, cx, START_XY(cy), cx, END_XY(cy));
}
//画校准点
int draw_dot(unsigned int num)
{
	dbg("\nTFT_ClearWindow\n");
	TFT_ClearWindow(Main_Frame);
	switch(num)
	{
		case 1:
			TFT_Circle(Main_Frame, CIRCLE_X1,CIRCLE_Y1,CIRCLE_RADIUS,1);
			draw_line(CIRCLE_X1,CIRCLE_Y1);
			break;
		case 2:
			TFT_Circle(Main_Frame, CIRCLE_X2,CIRCLE_Y2,CIRCLE_RADIUS,1);
			draw_line(CIRCLE_X2,CIRCLE_Y2);
			break;
		case 3:
			TFT_Circle(Main_Frame, CIRCLE_X3,CIRCLE_Y3,CIRCLE_RADIUS,1);
			draw_line(CIRCLE_X3,CIRCLE_Y3);
			break;	
		case 4: 
			TFT_Circle(Main_Frame, CIRCLE_X4,CIRCLE_Y4,CIRCLE_RADIUS,1);
			draw_line(CIRCLE_X4,CIRCLE_Y4);
			break;			
		case 5: 
			TFT_Circle(Main_Frame, CIRCLE_X5,CIRCLE_Y5,CIRCLE_RADIUS,1);
			draw_line(CIRCLE_X5,CIRCLE_Y5);
			break;		
	}
	return 0;
}
//读取触模点坐标
int ts_read(struct TS_RET *rts)
{
	int ret = -1;
	int temp_x,temp_y;
	
	while(ret < 0)
	{
		ret = ad_read_26();
	}	
	temp_x = ts.x;
	temp_y = ts.y;
	ts.x = SCR_WIDTH*(tcf.touch_calibrate_lb - temp_x)/(tcf.touch_calibrate_lb - tcf.touch_calibrate_rb);
	ts.y = SCR_HEIGHT*(temp_y - tcf.touch_calibrate_tb)/(tcf.touch_calibrate_bb - tcf.touch_calibrate_tb);	
	rts->x = ts.x;
	rts->y = ts.y;
	return ret;
}

int ts_calibrate_read()
{
	int ret = -1;
	
	while(ret < 0)
	{
		ret = ad_read_26();
	}
	return ret;
}
//校准计算
int ts_calibrate()
{	
	int tb,bb,lb,rb; //4个校准点采得AD的平均值
	int tb_fringe,lr_fringe;   //边界AD值
	
	draw_dot(1);
	
	if(ts_calibrate_read()<0)
		dbg("read ts error\n");
	else		
	{
		ca_tp[0][0] = ts.x;
		ca_tp[0][1] = ts.y;
		dbg("press x1 is %d\n",ts.x);
		dbg("press y1 is %d\n",ts.y);	
		draw_dot(2);
	}
		
	if(ts_calibrate_read()<0)
		dbg("read ts error\n");
	else		
	{
		ca_tp[1][0] = ts.x;
		ca_tp[1][1] = ts.y;	
		dbg("press x2 is %d\n",ts.x);
		dbg("press y2 is %d\n",ts.y);		
		draw_dot(3);
	}		
	
	if(ts_calibrate_read()<0)
		dbg("read ts error\n");
	else		
	{
		ca_tp[2][0] = ts.x;
		ca_tp[2][1] = ts.y;	
		dbg("press x3 is %d\n",ts.x);
		dbg("press y3 is %d\n",ts.y);		
		draw_dot(4);
	}		
	
	if(ts_calibrate_read()<0)
		dbg("read ts error\n");
	else		
	{
		ca_tp[3][0] = ts.x;
		ca_tp[3][1] = ts.y;	
		dbg("press x4 is %d\n",ts.x);
		dbg("press y4 is %d\n",ts.y);		
		draw_dot(5);
	}	
	
	if(ts_calibrate_read()<0)
		dbg("read ts error\n");
	else		
	{
		ca_tp[4][0] = ts.x;
		ca_tp[4][1] = ts.y;	
		dbg("press x5 is %d\n",ts.x);
		dbg("press y5 is %d\n",ts.y);	

		tb = (ca_tp[0][1]+ca_tp[3][1])/2;
		bb = (ca_tp[1][1]+ca_tp[2][1])/2; 
		lb = (ca_tp[0][0]+ca_tp[1][0])/2;
		rb = (ca_tp[2][0]+ca_tp[3][0])/2;		
		dbg("\ntb is x=%d,y=%d\n",ca_tp[0][0],ca_tp[0][1]);
		dbg("tb is x=%d,y=%d\n",ca_tp[1][0],ca_tp[1][1]);
		dbg("tb is x=%d,y=%d\n",ca_tp[2][0],ca_tp[2][1]);
		dbg("tb is x=%d,y=%d\n",ca_tp[3][0],ca_tp[3][1]);
		
		dbg("\ntb is %d\n",tb);
		dbg("bb is %d\n",bb);
		dbg("lb is %d\n",lb);		
		dbg("rb is %d\n",rb);
		
		tb_fringe = (SCR_FRINGE*(bb-tb))/(320-SCR_FRINGE*2);
		lr_fringe = (SCR_FRINGE*(lb-rb))/(240-SCR_FRINGE*2);
		dbg("tb_fringe is %d\n",tb_fringe);		
		dbg("lr_fringe is %d\n",lr_fringe);		
		
		//加减边界值，得到起始值
		tcf.touch_calibrate_tb = tb - tb_fringe;
		tcf.touch_calibrate_bb = bb + tb_fringe;
		tcf.touch_calibrate_lb = lb + lr_fringe;
		tcf.touch_calibrate_rb = rb - lr_fringe;	
		dbg("\ntouch_calibrate_tb is %d\n",tcf.touch_calibrate_tb);
		dbg("touch_calibrate_bb is %d\n",tcf.touch_calibrate_bb);
		dbg("touch_calibrate_lb is %d\n",tcf.touch_calibrate_lb);		
		dbg("touch_calibrate_rb is %d\n",tcf.touch_calibrate_rb);		

		return 0;
	}	
}
//检测或生成校准文件
int make_cal_file()
{
	FILE *fp;
	DIR *dir;
	struct dirent *ptr;
	char buffer[]="hello";
	int file_flag = 0;
	
	dir = opendir("/root");
	while((ptr = readdir(dir))!= NULL)
	{
		if(memcmp(ptr->d_name,"pointercal",10))	
			file_flag = 0;
		else
		{
			dbg("\npointercal ok\n");
			file_flag = 1;
			if((fp=fopen("/root/pointercal","r"))==NULL)
			{
				dbg("create file fial !\n");
			}
			fread(&tcf,sizeof(struct ts_cal_fringe),1,fp);
			dbg("\ntouch_calibrate_tb is %d\n",tcf.touch_calibrate_tb);
			dbg("touch_calibrate_bb is %d\n",tcf.touch_calibrate_bb);
			dbg("touch_calibrate_lb is %d\n",tcf.touch_calibrate_lb);		
			dbg("touch_calibrate_rb is %d\n",tcf.touch_calibrate_rb);		
			fclose(fp);			
			break;
		}
	}
	closedir(dir);
	if(file_flag == 1)
		return 0;
	
	if((fp=fopen("/root/pointercal","w+"))==NULL)
	{
		dbg("create file fial !\n");
	}	
	ts_calibrate();
	fwrite(&tcf,sizeof(struct ts_cal_fringe),1,fp);

	fclose(fp);
	return 0;
}
#if 1
//触模屏校准
int ts_cal_init(void)
{
	int ret;
	Main_Frame = TFT_CreateWindowEx(0, 0, 800, 480, COLOR_WHITE);
	TFT_SetColor(Main_Frame, COLOR_RED);
	
	ts_fd = open("/dev/event0",O_RDWR);
	if(ts_fd<0)
			dbg("open /dev/touchscreen/raw0 error");
			
	make_cal_file();	
	TFT_DeleteWindow(Main_Frame);
	
	return 0;
}
#else
//测试
int main(void)
{
	int ret;
	char *fbname = "/dev/fb/0";
	
	getFBP(fbname);
	TFT_Init();	
	Main_Frame = TFT_CreateWindowEx(0, 0, 320, 240, COLOR_WHITE);
	TFT_SetColor(Main_Frame, COLOR_RED);
	
	ts_fd = open("/dev/touchscreen/0",O_RDWR);
	if(ts_fd<0)
			dbg("open /dev/touchscreen/raw0 error");
			
	make_cal_file();
	TFT_ClearWindow(Main_Frame);
	TFT_SetColor(Main_Frame, COLOR_BLACK);
	while(1)
	{
			ret = ts_read(NULL);
			//ret = ad_read();
			if(ret<0)
					continue;                       
			if(ts.pressure == 0)
				dbg("\n press up:\n");
			else if(ts.pressure == 1)
			{
				dbg("\n press down:\n");  
			}
			TFT_ClearWindow(Main_Frame);
			TFT_Circle(Main_Frame, ts.x,ts.y,10,0);
			dbg("\n         x is %d\n",ts.x);
			dbg("         y is %d\n",ts.y);
	}		
	return 0;
}
#endif



