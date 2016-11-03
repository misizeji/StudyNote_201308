#ifndef _TFT_API_H_
#define _TFT_API_H_
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/types.h>
#include <fcntl.h>

typedef int WIN_HANDLE;
typedef short FONT;

typedef unsigned short COLOR;
typedef struct {
	unsigned short CharWidth;
	unsigned short CharHeight;
	unsigned char *FontBuf;
}STR_FONT;


//图形显示颜色 转换宏定义 RGB888--->RGB565
#define RGB24(r, g, b) 		( (((COLOR)r&0xf8)<<8) | (((COLOR)g&0xfc)<<3) | (((COLOR)b&0xf8)>>3) )

//特定颜色 RGB565
#define 	COLOR_WHITE		0xFFFF
#define		COLOR_BLACK		0x0000
#define		COLOR_RED			0xF800
#define		COLOR_GREEN		0x07E0
#define		COLOR_BLUE		0x001F
#define		COLOR_MAGENTA	0xF81F
#define		COLOR_CYAN		0x07FF
#define		COLOR_YELLOW	0xFFE0

//描述窗口的结构体
typedef struct {
	short TLx;													// 绘图区起始坐标
	short TLy;													// 绘图区起始坐标
	short BRx;													// 绘图区结束坐标
	short BRy;													// 绘图区结束坐标
	short Width;												// 绘图区宽度
	short Height;												// 绘图区高度
	short CurTextX;											// 文字绘制坐标
	short CurTextY;											// 文字绘制坐标
	unsigned short Transparency;				// 绘图透明度
	short Flag;													// 有效标志
	COLOR FGColor;											// 绘图前景色
	COLOR BGColor;											// 绘图背景色
	FONT AsciiFont;											// 当前使用的ASCII字库
	FONT ChineseFont;										// 当前使用的中文字库
}STR_WINDOW;



/*===========================================================*/
/*                      窗口操作类函数                       */
/*===========================================================*/

//=============================================================
//	TFT驱动程序初始化
//=============================================================
void TFT_Init(void);
//=============================================================
//	创建工作窗口，最对可创建15个窗口
//=============================================================
WIN_HANDLE TFT_CreateWindowEx(short TLx,		// 左上角起始坐标
							  short TLy,									// 左上角起始坐标
							  short width,								// 窗口宽度
							  short height,								// 窗口高度
							  COLOR BGColor);							// 窗口背景色
							  
WIN_HANDLE TFT_CreateWindow(short TLx,				// 左上角坐标
							 short TLy,
							 short BRx,											// 右下角坐标
							 short BRy,
							 COLOR BGColor);								// 背景颜色

//=============================================================
//	删除工作窗口（工作窗口区域清空）
//=============================================================
void TFT_DeleteWindow(WIN_HANDLE Handle);

//=============================================================
//	复制或移动工作窗口
//=============================================================
int TFT_MoveWindowEx(WIN_HANDLE Handle, short TLx, short TLy, int Cut);
//参数：	TLx,TLy		-	新位置左上角坐标
//				Cut				-	原窗口内容是否清除

//=============================================================
//	重设工作窗口（不改变屏幕图像）
//=============================================================
int TFT_ResetWindow(WIN_HANDLE Handle, short TLx, short TLy, short BRx, short BRy);

//=============================================================
//	移动工作窗口位置（不改变屏幕图像）
//=============================================================
int TFT_MoveWindowBorder(WIN_HANDLE Handle, short TLx, short TLy);


//=============================================================
//	工作窗口清屏
//=============================================================
void TFT_ClearWindow(WIN_HANDLE Handle);

//=============================================================
//	获取工作窗口宽度
//=============================================================
short TFT_GetWindowWidth(WIN_HANDLE Handle);

//=============================================================
//	获取工作窗口高度
//=============================================================
short TFT_GetWindowHeight(WIN_HANDLE Handle);

//=============================================================
//	设置新显示内容的透明度
//=============================================================
void TFT_SetTransparency(WIN_HANDLE Handle, unsigned short TransparencySet);

//=============================================================
//	获取当前设定的透明度
//=============================================================
unsigned short TFT_GetTransparency(WIN_HANDLE Handle);


//=============================================================
//	设置显示内容的前景色
//=============================================================
void TFT_SetColor(WIN_HANDLE Handle, unsigned short ColorSet);


//=============================================================
//	获取当前设定的前景色
//=============================================================
unsigned short TFT_GetColor(WIN_HANDLE Handle);

//=============================================================
//	设定显示背景色（对工作区、文本有效）
//=============================================================
void TFT_SetBGColor(WIN_HANDLE Handle, unsigned short ColorSet);

//=============================================================
//	获取当前设定的背景色
//=============================================================
unsigned short TFT_GetBGColor(WIN_HANDLE Handle);

//=============================================================
//	设置工作区中的文本显示位置
//=============================================================
void TFT_SetTextPos(WIN_HANDLE Handle, short x, short y);


//=============================================================
//	设置当前使用的中文字库
//=============================================================
void TFT_SetChineseFont(WIN_HANDLE Handle, FONT FontID);


//=============================================================
//	获取当前使用的中文字库信息
//=============================================================
FONT TFT_GetChineseFont(WIN_HANDLE Handle, STR_FONT *FontInfo);


//=============================================================
//	设置当前使用的ASCII字库
//=============================================================
void TFT_SetAsciiFont(WIN_HANDLE Handle, FONT FontID);


//=============================================================
//	获取当前使用的ASCII字库信息
//=============================================================
FONT TFT_GetAsciiFont(WIN_HANDLE Handle, STR_FONT *FontInfo);


/*===========================================================*/
/*                        绘图类函数                         */
/*===========================================================*/
//=============================================================
//	在工作区的指定位置画点
//=============================================================
void TFT_PutPixel(WIN_HANDLE Handle, short x, short y);

//=============================================================
//	获取工作区内指定坐标像素的颜色
//=============================================================
unsigned short TFT_PickColor(WIN_HANDLE Handle, short x, short y);


//=============================================================
//	在工作区中根据颜色表绘制行
//=============================================================
void TFT_PutPixelLine(WIN_HANDLE Handle, short x, short y, short len, COLOR *ColorTab);

//=============================================================
//	在工作区中画直线
//=============================================================
void TFT_Line(WIN_HANDLE Handle, short x1, short y1, short x2, short y2);


//=============================================================
//	在工作区中画圆
//=============================================================
void TFT_Circle(WIN_HANDLE Handle, unsigned x,unsigned y,unsigned r,unsigned Mode);
//				x - 圆心的x坐标(相对于工作区)
//				y - 圆心的y坐标(相对于工作区)
//				r - 圆的半径
//				Mode -	0(PAINT_HOLLOW)	空心圆  1(PAINT_SOLID)	实心圆


//=============================================================
//	在工作区中矩形
//=============================================================
void TFT_Rectangle(WIN_HANDLE Handle,		// 窗体指针
				   unsigned x1,									// 左上角坐标
				   unsigned y1,
				   unsigned x2,									// 右下角坐标
				   unsigned y2,
				   unsigned Mode);							// PAINT_HOLLOW: 空心
				   															// PAINT_SOLID : 实心
				   		
/*===========================================================*/
/*                        汉字及图像显示类                  */
/*===========================================================*/	   		
				   		
				   															
//=============================================================
//	显示一个字符或汉字
//=============================================================
void TFT_PutChar(WIN_HANDLE Handle, unsigned short CharCode);


//=============================================================
//	根据指定格式打印字符串
//=============================================================
void TFT_Print(WIN_HANDLE Handle, const char *format, ...);


//=============================================================
//	在工作区中显示图像
//=============================================================
void TFT_PutImageEx(WIN_HANDLE Handle, short x, short y, short width, short height, void *pImage);


//=============================================================
//	在工作区中显示图像
//=============================================================
void TFT_File_Picture(WIN_HANDLE Handle, short x, short y, unsigned char * Bmp_Name, int AutoScale);
//参数	x,y：					图片在窗口中的位置
//			Bmp_Name：		图片的名称（如不在当前路径下，需包含路径信息）
//			AutoScale:		1：图片自适应窗口大小		0:不适应
//			自适应窗口用于当图像大于窗口时，并且图像显示的起始不能在窗口之外





#endif
