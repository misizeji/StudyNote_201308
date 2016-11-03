#include "s5pv210.h"
#include "speed.h"


/*
* 单位: 延时1 ms
*/
void mdelay(int num)
{
	S5PC11X_TIMERS *const timers = S5PC11X_GetBase_TIMERS();

	//设置预分频为16
	timers->TCFG0 &= ~(0xff);
	timers->TCFG0 |= 16;			

	//选择1/4  分频低四位
	timers->TCFG1 &= ~(0x0f<<(1*4));
	timers->TCFG1 |= 0x02<<(1*4);

	//设置初值寄存器
	timers->ch[1].TCNTB = (get_PCLKP()/(17*4))*num;//get_PCLKP()输入时钟频率

	//手动装载
	timers->TCON &= ~(0x0f00);
	timers->TCON |= TIMER1_MANUP;

	//开启定时器0
	timers->TCON &= ~(0x0f00);
	timers->TCON |= TIMER1_ON;

	while(!(timers->ch[1].TCNTO));
	
	while(timers->ch[1].TCNTO);
}



