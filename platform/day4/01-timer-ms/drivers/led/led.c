#include "s5pv210.h"

void soft_delay(vu_long count)
{
	vu_long i;

	for(;count>0;count--)
		for(i=100;i>0;i--);
}

void led_init(void)
{
	//将GPH0_0/1/2/3配置成输出模式
	__REG(GPH0CON) &= ~((0x0f<<(0*4))|(0x0f<<(1*4))|(0x0f<<(2*4))|(0x0f<<(3*4)));
	__REG(GPH0CON) |= ((0x01<<(0*4))|(0x01<<(1*4))|(0x01<<(2*4))|(0x01<<(3*4)));
}

void led_test(void)
{
#if 1
	//流水灯
	static int val = 1;

	__REG(GPH0DAT) &= ~0x0f;
	__REG(GPH0DAT) = val;
	
	val <<= 1;
	if(val == 16)
		val = 1;
	
	soft_delay(10000);
#else	 
	//同时亮灭
	static int led_on = 0x0f;	

	__REG(GPH0DAT) |= led_on;
	soft_delay(20000);
	
	__REG(GPH0DAT) &= ~led_on;
	soft_delay(20000);	
#endif
}

void led_marquee(void)
{
	//跑马灯
	static int val = 1;

	__REG(GPH0DAT) &= ~0x0f;
	__REG(GPH0DAT) = val;
	
	val <<= 1;
	if(val == 16)
		val = 1;
}


