#include "s5pv210.h"


void key_init(void)
{
	//将GPH3_0设置成输出
	__REG(GPH3CON) &= ~(0x0f<<(0*4));
	__REG(GPH3CON) |= 0x01<<(0*4);
	//设置GPH3_0输出高电平
	__REG(GPH3DAT) |= 0x01<<0;

	//将GPH2_3设置成输入
	__REG(GPH2CON) &= ~(0x0f<<(3*4));
	//设置GPH2_3下拉使能
	__REG(GPH2PUD) &= ~(0x03<<(3*2));
	__REG(GPH2PUD) |= 0x01<<(3*2);
}

void key_deal(void)
{
	const int led_on = 0x0f; 
	unsigned int temp = __REG(GPH2DAT)&0xff;
	
	if((temp&0x08) == 0x08)			
		__REG(GPH0DAT) &= ~led_on;	
	else if((temp&0x08) == 0x00)			
		__REG(GPH0DAT) |= led_on;
}
