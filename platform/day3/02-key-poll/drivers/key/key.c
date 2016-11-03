#include "s5pv210.h"


void key_init(void)
{
	//��GPH3_0���ó����
	__REG(GPH3CON) &= ~(0x0f<<(0*4));
	__REG(GPH3CON) |= 0x01<<(0*4);
	//����GPH3_0����ߵ�ƽ
	__REG(GPH3DAT) |= 0x01<<0;

	//��GPH2_3 GPH2_4 GPH2_5 GPH2_6 GPH2_7 ���ó�����
	__REG(GPH2CON) &= ~((0x0f<<(3*4))|(0x0f<<(4*4))|(0x0f<<(5*4))|(0x0f<<(6*4))|(0x0f<<(7*4)));

	
	//����GPH2_3 GPH2_4 GPH2_5 GPH2_6 GPH2_7����ʹ��
	__REG(GPH2PUD) &= ~((0x03<<(3*2))|(0x03<<(4*2))|(0x03<<(5*2)|(0x03<<(6*2))|(0x03<<(7*2))));
	__REG(GPH2PUD) |= ((0x01<<(3*2))|(0x01<<(4*2))|(0x01<<(5*2))|(0x01<<(6*2))|(0x01<<(7*2)));
}

void key_deal(void)
{
	const int led_on = 0x0f; 
	unsigned int temp = __REG(GPH2DAT)&0xff;
	
	
	if((temp&0x08) == 0x08)//key 2
	{
		__REG(GPH0DAT) &= ~led_on;
		printf("you press key 2\n");
	}
	else if((temp&0x10) == 0x10)//key 3
	{
		__REG(GPH0DAT) &= ~led_on;
		printf("you press key 3\n");
		
	}
	else if((temp&0x20) == 0x20)//key 4
	{
		__REG(GPH0DAT) &= ~led_on;
		printf("you press key 4\n");
	}
	else if((temp&0x40) == 0x40)//key 5
	{
		__REG(GPH0DAT) &= ~led_on;
		printf("you press key 5\n");
	}
	else if((temp&0x80) == 0x80)//key 6
	{
		__REG(GPH0DAT) &= ~led_on;
		
		
		
		printf("you press key 6\n");
	}
	else
	{
		__REG(GPH0DAT) |= led_on;
	}
}
