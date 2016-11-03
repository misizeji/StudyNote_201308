#include "uart.h"
#include "s5pv210.h"

/*
* 定时器timer 3停止计时
*/
void stop_timer(void)
{
	S5PC11X_TIMERS *const timers = S5PC11X_GetBase_TIMERS();
	timers->TCON &= ~(0x0f<<16);
}

/*
* 定时器timer 3开始计时
*/
void start_timer(void)
{
	S5PC11X_TIMERS *const timers = S5PC11X_GetBase_TIMERS();

	timers->TCON &= ~(0x0f<<16);
	timers->TCON |= TIMER3_ATLOAD_ON|TIMER3_ON;
}

/*
*pwm占空比增大
*/
void pwm_duty_add(void)
{
	S5PC11X_TIMERS *const timers = S5PC11X_GetBase_TIMERS();

	if(timers->ch[3].TCMPB < 900)
		timers->ch[3].TCMPB += 100;
}

/*
*pwm占空比减小
*/
void pwm_duty_sub(void)
{
	S5PC11X_TIMERS *const timers = S5PC11X_GetBase_TIMERS();

	if(timers->ch[3].TCMPB > 0)
		timers->ch[3].TCMPB -= 100;
}

/*
* 定时器3作为PWM输出
*/
void pwm3_out_init(void)
{
	S5PC11X_TIMERS *const timers = S5PC11X_GetBase_TIMERS();

	/*根据原理图可知背光灯是由pwm3控制,
	即GPD0[3]口,将GPG0[3]配置成PWM 输出*/
	__REG(GPD0CON) &= ~(0x0f<<(3*4));
	__REG(GPD0CON) |= (0x02<<(3*4));
	
	//设置预分频为65
	timers->TCFG0 &= ~(0xff<<8);
	timers->TCFG0 |= 65<<8;			

	//选择1/4  分频
	timers->TCFG1 &= ~(0x0f<<12);
	timers->TCFG1 |= (0x02<<12);

	/*设置PWM频率为1KHz = 1MHz / 1000*/
	timers->ch[3].TCNTB = 1000;

	/*设置初始占空比为50%*/
	timers->ch[3].TCMPB = 500;

	//手动装载
	timers->TCON &= ~(0x0f<<16);
	timers->TCON |= TIMER3_MANUP;

	//启动定时器3，且手动装载
	timers->TCON &= ~(0x0f<<16);
	timers->TCON |= TIMER3_ATLOAD_ON|TIMER3_ON;
}



