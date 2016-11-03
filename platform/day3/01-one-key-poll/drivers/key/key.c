#include "s5pv210.h"


void key_init(void)
{
	//��GPH3_0���ó����
	__REG(GPH3CON) &= ~(0x0f<<(0*4));
	__REG(GPH3CON) |= 0x01<<(0*4);
	//����GPH3_0����ߵ�ƽ
	__REG(GPH3DAT) |= 0x01<<0;

	//��GPH2_3���ó�����
	__REG(GPH2CON) &= ~(0x0f<<(3*4));
	//����GPH2_3����ʹ��
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
