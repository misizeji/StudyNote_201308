
#include "TFT_API.h"

int main(void)
{
 	WIN_HANDLE DemoWindow1;

	char *fbname = "/dev/fb5";
	getFBP(fbname);

	TFT_Init();	
	
	DemoWindow1 = TFT_CreateWindowEx(0,0,128,160,COLOR_YELLOW);
	printf("after create!\n");
	sleep(1);
	TFT_SetColor(DemoWindow1, COLOR_RED);	//设置工作窗口1颜色
	TFT_PutPixel(DemoWindow1, 10, 10);			//在工作窗口1中画点
	TFT_Line(DemoWindow1, 15, 15, 100, 100);	//在工作窗口1中画线
	TFT_Circle(DemoWindow1, 30, 30, 25, 1);		//在工作窗口1中画圆
	
	TFT_Circle(DemoWindow1, 30, 80, 25, 0);		//在工作窗口1中画圆
	
	TFT_SetColor(DemoWindow1, COLOR_CYAN);		//设置工作窗口1颜色
	
	TFT_Rectangle(	DemoWindow1,50,	50,90,90,1);							


	TFT_SetColor(DemoWindow1, COLOR_RED);  
	
	TFT_SetTextPos(DemoWindow1, 50, 50);
	
	TFT_Print(DemoWindow1, "TFT Demo程序示例");
	
	while(1);
	TFT_DeleteWindow(DemoWindow1);
	return 0;
}

