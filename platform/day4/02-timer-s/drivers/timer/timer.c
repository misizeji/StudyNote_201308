#include "s5pv210.h"
#include "speed.h"


/*
* ��λ: ��ʱ1 ms
*/
void mdelay(int num)
{
	S5PC11X_TIMERS *const timers = S5PC11X_GetBase_TIMERS();

	//����Ԥ��ƵΪ16
	timers->TCFG0 &= ~(0xff);
	timers->TCFG0 |= 16;			

	//ѡ��1/4  ��Ƶ����λ
	timers->TCFG1 &= ~(0x0f<<(1*4));
	timers->TCFG1 |= 0x02<<(1*4);

	//���ó�ֵ�Ĵ���
	timers->ch[1].TCNTB = (get_PCLKP()/(17*4))*num;//get_PCLKP()����ʱ��Ƶ��

	//�ֶ�װ��
	timers->TCON &= ~(0x0f00);
	timers->TCON |= TIMER1_MANUP;

	//������ʱ��0
	timers->TCON &= ~(0x0f00);
	timers->TCON |= TIMER1_ON;

	while(!(timers->ch[1].TCNTO));
	
	while(timers->ch[1].TCNTO);
}



