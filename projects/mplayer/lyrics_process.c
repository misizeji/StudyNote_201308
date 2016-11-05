#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "lyrics_process.h"
#include "TFT_API.h"
#include "mplayer.h"

/*******************************************************
*功能：     打开歌词文件，读数据到缓冲区
*参数：		歌词文件地址：addr
*返回值：	缓冲区首地址
********************************************************/
static char *open_lrc(char *addr)
{
	FILE *fptr = NULL;
	char *lycris_buf = NULL;	//the address of malloc buf which saved lycris content
	int size =0;
	
	if(addr == NULL)
	{
		printf("error: addr or size is NULL!\n");
	}
	else
	{
		fptr = fopen(addr, "rb");
		if(fptr == NULL)
		{
		    printf("open lycris [%s] error!\n", addr);
			TFT_File_Picture(DemoWindow4, 0, 0, "../picture/lyrics.bmp", 1);			//清屏
			TFT_SetColor(DemoWindow4, COLOR_BLACK);
			TFT_SetTextPos(DemoWindow4, 90, (90+0));							//设置字符在窗口打印位置
			set_tft_print_utf8();    
			TFT_Print(DemoWindow4, "鏈壘鍒版瓕璇?);
			set_tft_print_ansi();	
		}
		else
		{
			fseek(fptr, 0, SEEK_END);
			size = ftell(fptr);
			rewind(fptr);
			lycris_buf = (char *)malloc(size+1);
			bzero(lycris_buf, size+1);
			if(lycris_buf == NULL)
			{

				printf("malloc lycris_buf error\n");
			}
			else
			{

				fread(lycris_buf, size, 1, fptr);
			}
			fclose(fptr);
		}
	}
	return lycris_buf;
}

/*******************************************************
*功能：     增加一个歌词信息节点至链表结尾
*参数：		链表的表头：head
			歌词时间：time
			歌词：lrc
*返回值：	链表的表头：head
********************************************************/
static LRC *add_lrc_to_link(LRC *head, uint time, char *lrc)
{
	LRC *nextpr,*temp;
	nextpr = head;

	if(nextpr==NULL)			//创建链表
	{
        head = (LRC *)malloc(sizeof(LRC));
        if(head == NULL)
        {
            printf("内存分配失败，创建链表失败\n");
            head = NULL;
        }
        else
        {
            head->time = time;
            strcpy(head->lrc, lrc);
            head->next = NULL;
        }
	}
	else	//增加一个链表元素至链表结尾
	{
		while(nextpr)
		{
 			temp = nextpr;
			nextpr = nextpr->next;
		}
		nextpr = (LRC *)malloc(sizeof(LRC));
		if(nextpr == NULL)
		{
			printf("内存分配失败，增加链表元素失败\n");
		}
		else
		{
			nextpr->time = time;
			strcpy(nextpr->lrc, lrc);
			nextpr->next = NULL;
			temp->next = nextpr;
		}
	}
	return head;
}

/*******************************************************
*功能：     遍历输出链表
*参数：		歌词句柄：lrc
*返回值：	无
********************************************************/
void print_lrc_link(LRC_PTR lrc)
{
	uint i = 0;
	for(i=0;i<lrc.lrc_arry_size;i++)
	{	
		//printf("第%d个节点：\n", i);
		//printf("鏃堕棿: %dms 姝岃瘝: %s\n", lrc.lrc_arry[i]->time, lrc.lrc_arry[i]->lrc);
		if(time_pos == (lrc.lrc_arry[i]->time/1000))
		{
			//printf("鏃堕棿: %d s 姝岃瘝: %s\n", time_pos, lrc.lrc_arry[i]->lrc);
			//sleep(1);
			if(i == 0)
			{
				TFT_File_Picture(DemoWindow4, 0, 0, "../picture/lyrics.bmp", 1);			//清屏贴图
				/***************画矩形********************/
				TFT_SetColor(DemoWindow4, COLOR_GREEN);
				TFT_Rectangle(DemoWindow4,35,50,336,140,PAINT_HOLLOW);
				
				TFT_SetColor(DemoWindow4, RGB(0xff,0x00,0xff));
				TFT_SetTextPos(DemoWindow4, 50, (90+0));			//设置字符在窗口打印位置
				set_tft_print_utf8();				
				TFT_Print(DemoWindow4, "姝屾洸: %s",lrc.lrc_arry[i]->lrc);
				set_tft_print_ansi();	
				
				TFT_SetColor(DemoWindow4, COLOR_BLACK);
				TFT_SetTextPos(DemoWindow4, 60, (90+30));			//设置字符在窗口打印位置
				set_tft_print_utf8();    
				TFT_Print(DemoWindow4, "%s",lrc.lrc_arry[i+1]->lrc);
				set_tft_print_ansi();	
			}
			else if (i >= 1 && i < (lrc.lrc_arry_size-1))
			{
				TFT_File_Picture(DemoWindow4, 0, 0, "../picture/lyrics.bmp", 1);			//清屏
				/***************画矩形********************/
				TFT_SetColor(DemoWindow4, COLOR_GREEN);
				
				TFT_Rectangle(DemoWindow4,35,50,336,140,PAINT_HOLLOW);
				TFT_SetColor(DemoWindow4, COLOR_BLACK);
				TFT_SetTextPos(DemoWindow4, 60, (90-30));			//设置字符在窗口打印位置
				set_tft_print_utf8();    
				TFT_Print(DemoWindow4, "%s",lrc.lrc_arry[i-1]->lrc);
				set_tft_print_ansi();	
				
				TFT_SetColor(DemoWindow4, RGB(0xff,0x00,0xff));
				TFT_SetTextPos(DemoWindow4, 50, (90+0));			//设置字符在窗口打印位置
				set_tft_print_utf8();    
				TFT_Print(DemoWindow4, "-->%s",lrc.lrc_arry[i]->lrc);
				set_tft_print_ansi();	
				
				TFT_SetColor(DemoWindow4, COLOR_BLACK);
				TFT_SetTextPos(DemoWindow4, 60, (90+30));			//设置字符在窗口打印位置
				set_tft_print_utf8();    
				TFT_Print(DemoWindow4, "%s",lrc.lrc_arry[i+1]->lrc);
				set_tft_print_ansi();	
			}
			else if(i == (lrc.lrc_arry_size-1))
			{
				TFT_File_Picture(DemoWindow4, 0, 0, "../picture/lyrics.bmp", 1);			//清屏
				/***************画矩形********************/
				TFT_SetColor(DemoWindow4, COLOR_GREEN);
				TFT_Rectangle(DemoWindow4,35,50,336,140,PAINT_HOLLOW);
				
				TFT_SetColor(DemoWindow4, COLOR_BLACK);
				TFT_SetTextPos(DemoWindow4, 60, (90-30));			//设置字符在窗口打印位置
				set_tft_print_utf8();    
				TFT_Print(DemoWindow4, "%s",lrc.lrc_arry[i-1]->lrc);
				set_tft_print_ansi();	
				
				TFT_SetColor(DemoWindow4, RGB(0xff,0x00,0xff));
				TFT_SetTextPos(DemoWindow4, 50, (90+0));			//设置字符在窗口打印位置
				set_tft_print_utf8();    
				TFT_Print(DemoWindow4, "-->%s",lrc.lrc_arry[i]->lrc);
				set_tft_print_ansi();	
				
				TFT_SetColor(DemoWindow4, COLOR_BLACK);
				TFT_SetTextPos(DemoWindow4, 60, (90+30));			//设置字符在窗口打印位置
				set_tft_print_utf8();    
				TFT_Print(DemoWindow4, "~~~~~~~~~~it's over!!!~~~~~~~~");
				set_tft_print_ansi();	
			}
		}
	}
}

/*******************************************************
*功能：     链表的元素按时间大小从小到大排序
*参数：		歌词句柄：lrc
*返回值：	无
********************************************************/
static void inorder_link(LRC_PTR *lrc)
{
	LRC *nextpr,*temp;
	uint j,k;

	nextpr = lrc->lrc_head;
	lrc->lrc_arry_size = 0;
	while(nextpr)
	{
		lrc->lrc_arry_size++;
		nextpr = nextpr->next;
	}
	lrc->lrc_arry = calloc(lrc->lrc_arry_size, sizeof(lrc->lrc_arry));
	nextpr = lrc->lrc_head;
	for(j=0;j<lrc->lrc_arry_size;j++)
	{
		*(lrc->lrc_arry+j) = nextpr;		//保存链表各元素地址
		nextpr = nextpr->next;
	}
	for(j=0;j<lrc->lrc_arry_size-1;j++)	//按照各链表元素的id从小到大排序，将各元素放入指针数组中
	{
		for(k=j+1;k<lrc->lrc_arry_size;k++)
		{
			if(lrc->lrc_arry[j]->time > lrc->lrc_arry[k]->time)
			{
				temp = *(lrc->lrc_arry+k);
				lrc->lrc_arry[k] = lrc->lrc_arry[j];
				lrc->lrc_arry[j]= temp;
			}
		}
	}
	lrc->lrc_head = *(lrc->lrc_arry);
	for(j=0;j<(lrc->lrc_arry_size-1);j++)	//按照指针数组中的各链表元素的地址的顺序重新给链表安排指针域
	{
		lrc->lrc_arry[j]->next = lrc->lrc_arry[j+1];
	}
	lrc->lrc_arry[lrc->lrc_arry_size-1]->next = NULL;
}

/*******************************************************
*功能：     判断是否是正确的时间标签
*参数：		时间标签：label
*返回值：	1：是；0：不是
********************************************************/
static char judge_time_label(char *label)
{
    //合法时间标签示例[00:07.41
    if(label)//label非空
    {
        if(strlen(label) == 9)
        {
            if((*(label+0)=='[') && (*(label+3)==':') && (*(label+6)=='.')
                && (*(label+1)>='0' && *(label+1)<='9')
                && (*(label+2)>='0' && *(label+2)<='9')
                && (*(label+4)>='0' && *(label+4)<='9')
                && (*(label+5)>='0' && *(label+5)<='9')
                && (*(label+7)>='0' && *(label+7)<='9')
                && (*(label+8)>='0' && *(label+8)<='9'))
            {
                //printf("label=%s\n", label);
                return 1;
            }
        }
    }
    return 0;
}

/*******************************************************
*功能：     计算时间标签
*参数：		时间标签：label
*返回值：	时间标签代表的时间(单位:ms)
********************************************************/
static uint calculate_time_label(char *label)
{
	uint mtime = 0;
    uint minute = 0;
	uint second = 0;
	uint msecond = 0;

	//printf("时间标签：%s\n", label);
    sscanf(label, "[%d:%d.%d", &minute, &second, &msecond);
    //printf("minute=%d\n,second=%d\n,msecond=%d\n", minute, second, msecond);
    mtime = minute*60000 + second*1000 + msecond*10;
    return mtime;
}

/*******************************************************
*功能：     处理歌词文件一行信息
*参数：		行地址：line
			链表的表头：head
*返回值：	链表的表头：head
********************************************************/
static LRC *dispose_line(char *line,LRC *head)
{
	uint i = 0;
	uint argc = 0;
	uint mtime = 0;
	char *lrc_text = NULL;
	char *argv[lrc_time_labels] = {NULL};

	//printf("line=%s\n", line);
	argv[argc] = strtok(line, "]");
    while((argv[++argc] = strtok(NULL,"]"))!=NULL)
    {
        if(argc>=lrc_time_labels)
        {
            printf("lrc: too many labels\n");
            break;
        }
    }
	/*
	printf("argc =%d\n", argc);
	for(i=0;i<argc;i++)
    {
		printf("argv[%d]=%s\n", i, argv[i]);
	}*/
    //取出歌词
    if(judge_time_label(argv[0]) != 0 && argv[argc-1] != NULL)
    {
       // printf("lyrics：%s\n", argv[argc-1]);
        lrc_text = argv[argc-1];
    }
	//printf("\n");
    for(i=0;i<argc;i++)
    {
		//printf("argc = %d\targv[%d]=%s\n", argc, i, argv[i]);
        if(judge_time_label(argv[i]))//判断时间标签
        {
			mtime = calculate_time_label(argv[i]);
            if(lrc_text)
            {
                head = add_lrc_to_link(head, mtime, lrc_text);
            }
        }
    }
	return head;
}

/*******************************************************
*功能：     将缓冲区内的'\n'换成'\0',并在每次替换时
			处理歌词文件行信息
*参数：		缓冲区首地址：lycris_buf
			歌词句柄：lrc
*返回值：	无
********************************************************/
static void get_dispose_line(char *lycris_buf, LRC_PTR *lrc)
{
	char *line_ptr = NULL;
	
	while((line_ptr = strtok(lycris_buf, "\n")) != NULL)
	{
		lycris_buf += strlen(lycris_buf)+1; 
		lrc->lrc_head = dispose_line(line_ptr, lrc->lrc_head);
	}
}

/*******************************************************
*功能：     处理歌词文件
*参数：		歌词文件名 ：name
			歌词句柄：lrc
*返回值：	歌词信息结构体(链表)
********************************************************/
LRC *dispose_lrc(char *name, LRC_PTR *lrc)
{
	char *lycris_buf = NULL;

	bzero(lrc, sizeof(LRC_PTR));	//init lrc struct
	lycris_buf = open_lrc(name);
	if(lycris_buf != NULL)
	{
		get_dispose_line(lycris_buf, lrc);
	 	free(lycris_buf);
        inorder_link(lrc);
	}
	return lrc->lrc_head;
}

/*******************************************************
*功能：     处理歌词文件
*参数：		歌词句柄：lrc
*返回值：	NULL
********************************************************/
void free_lrc_arry(LRC_PTR *lrc)
{
	int i;
	if(lrc->lrc_arry != NULL)
	{
		for(i=0; i<lrc->lrc_arry_size; i++)
		{
			free(lrc->lrc_arry[i]);
		}
		free(lrc->lrc_arry);
	}
	lrc->lrc_arry = NULL;
}

#if 1

/*******************************************************
*功能：     显示歌词程序
*参数：		无
*返回值：	无
********************************************************/
/*
int main(int argc, char **argv)
{
	LRC_PTR lrc;

    dispose_lrc("easy_love.lrc", &lrc);
	print_lrc_link(lrc);
    free_lrc_arry(&lrc);
	return 0;
}
*/
#endif
/***************************************************
	函数功能：进行实时的歌曲显示
	参数：		无
	返回值：	无
****************************************************/
void *display_lyrics(void *arg)
{
	LRC_PTR lrc;
	while(1)
	{
		//usleep(100*1000);
		char lrc_name[100] = "";
		char tmp[100] = "";
		bzero(tmp,sizeof(tmp));
		sscanf(song_name[play_now],"%[^.]",tmp);
		//printf("_____%s______\n",tmp);
		bzero(lrc_name,sizeof(lrc_name));
		sprintf(lrc_name,"../lyrics/%s.lrc",tmp);
		//printf("_____%s______\n",lrc_name);	
		dispose_lrc(lrc_name, &lrc);
		print_lrc_link(lrc);
	}
	free_lrc_arry(&lrc);
}