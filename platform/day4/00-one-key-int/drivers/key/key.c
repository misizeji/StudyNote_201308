#include "s5pv210.h"


void key_init(void)
{
	//将GPH3_0  设置成输出
	__REG(GPH3CON) &= ~(0x0F<<(0*4));
	__REG(GPH3CON) |= 0x01<<(0*4);
	//设置GPH3_0输出低电平
	__REG(GPH3DAT) &= ~(0x01<<0);
	
	//将GPH2_3  设置成中断功能
	__REG(GPH2CON) &= ~(0xF<<(3*4));
	__REG(GPH2CON) |= 0xF<<(3*4);
	
	//设置GPH2_3  上拉使能
	__REG(GPH2PUD) &= ~(0x03<<(3*2));
	__REG(GPH2PUD) |= 0x02<<(3*2);
	
	//设置GPH2_3  下降沿触发
	__REG(EXT_INT_2_CON) &= ~(0x07<<(3*4));
	__REG(EXT_INT_2_CON) |= 0x02<<(3*4);

	//清除挂起标志
	__REG(EXT_INT_2_PEND) = 0x01<<3;
	
	//屏蔽位禁止
	__REG(EXT_INT_2_MASK) &= ~(0x01<<3);
}

