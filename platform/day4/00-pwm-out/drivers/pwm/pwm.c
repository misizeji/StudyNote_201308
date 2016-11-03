#include "uart.h"
#include "s5pv210.h"

/*
* ��ʱ��timer 3ֹͣ��ʱ
*/
void stop_timer(void)
{
	S5PC11X_TIMERS *const timers = S5PC11X_GetBase_TIMERS();
	timers->TCON &= ~(0x0f<<16);
}

/*
* ��ʱ��timer 3��ʼ��ʱ
*/
void start_timer(void)
{
	S5PC11X_TIMERS *const timers = S5PC11X_GetBase_TIMERS();

	timers->TCON &= ~(0x0f<<16);
	timers->TCON |= TIMER3_ATLOAD_ON|TIMER3_ON;
}

/*
*pwmռ�ձ�����
*/
void pwm_duty_add(void)
{
	S5PC11X_TIMERS *const timers = S5PC11X_GetBase_TIMERS();

	if(timers->ch[3].TCMPB < 900)
		timers->ch[3].TCMPB += 100;
}

/*
*pwmռ�ձȼ�С
*/
void pwm_duty_sub(void)
{
	S5PC11X_TIMERS *const timers = S5PC11X_GetBase_TIMERS();

	if(timers->ch[3].TCMPB > 0)
		timers->ch[3].TCMPB -= 100;
}

/*
* ��ʱ��3��ΪPWM���
*/
void pwm3_out_init(void)
{
	S5PC11X_TIMERS *const timers = S5PC11X_GetBase_TIMERS();

	/*����ԭ��ͼ��֪���������pwm3����,
	��GPD0[3]��,��GPG0[3]���ó�PWM ���*/
	__REG(GPD0CON) &= ~(0x0f<<(3*4));
	__REG(GPD0CON) |= (0x02<<(3*4));
	
	//����Ԥ��ƵΪ65
	timers->TCFG0 &= ~(0xff<<8);
	timers->TCFG0 |= 65<<8;			

	//ѡ��1/4  ��Ƶ
	timers->TCFG1 &= ~(0x0f<<12);
	timers->TCFG1 |= (0x02<<12);

	/*����PWMƵ��Ϊ1KHz = 1MHz / 1000*/
	timers->ch[3].TCNTB = 1000;

	/*���ó�ʼռ�ձ�Ϊ50%*/
	timers->ch[3].TCMPB = 500;

	//�ֶ�װ��
	timers->TCON &= ~(0x0f<<16);
	timers->TCON |= TIMER3_MANUP;

	//������ʱ��3�����ֶ�װ��
	timers->TCON &= ~(0x0f<<16);
	timers->TCON |= TIMER3_ATLOAD_ON|TIMER3_ON;
}



