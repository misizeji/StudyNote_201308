#include "s5pv210.h"


void key_init(void)
{
	//��GPH3_0  ���ó����
	__REG(GPH3CON) &= ~(0x0F<<(0*4));
	__REG(GPH3CON) |= 0x01<<(0*4);
	//����GPH3_0����͵�ƽ
	__REG(GPH3DAT) &= ~(0x01<<0);
	
	//��GPH2_3  ���ó��жϹ���
	__REG(GPH2CON) &= ~(0xF<<(3*4));
	__REG(GPH2CON) |= 0xF<<(3*4);
	
	//����GPH2_3  ����ʹ��
	__REG(GPH2PUD) &= ~(0x03<<(3*2));
	__REG(GPH2PUD) |= 0x02<<(3*2);
	
	//����GPH2_3  �½��ش���
	__REG(EXT_INT_2_CON) &= ~(0x07<<(3*4));
	__REG(EXT_INT_2_CON) |= 0x02<<(3*4);

	//��������־
	__REG(EXT_INT_2_PEND) = 0x01<<3;
	
	//����λ��ֹ
	__REG(EXT_INT_2_MASK) &= ~(0x01<<3);
}

