#include "config.h"


#ifdef FUNC_ONE_KEY_POLL

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

#endif /*FUNC_ONE_KEY_POLL*/

#ifdef FUNC_ALL_KEY_POLL

/*
*��������ɨ��ʼ��
*��5�����ó�����������
*��2�����ó�����ߵ�ƽ
*/
void col_scan_init(void)
{
	//����GPH3_0/1Ϊ���
	__REG(GPH3CON) &= ~(0x0f<<(0*4)|0x0f<<(1*4));
	__REG(GPH3CON) |= (0x01<<(0*4)|0x01<<(1*4));
	//����GPH3_0/1����ߵ�ƽ
	__REG(GPH3DAT) |= (0x01<<0|0x01<<1);

	//����GPH2_3/4/5/6/7Ϊ����
	__REG(GPH2CON) &= ~(0x0f<<(3*4)|0x0f<<(4*4)|0x0f<<(5*4)|0x0f<<(6*4)|0x0f<<(7*4));
	//����GPH2_3/4/5/6/7����ʹ��
	__REG(GPH2PUD) &= ~(0x03<<(3*2)|0x03<<(4*2)|0x03<<(5*2)|0x03<<(6*2)|0x03<<(7*2));
	__REG(GPH2PUD) |= 0x01<<(3*2)|0x01<<(4*2)|0x01<<(5*2)|0x01<<(6*2)|0x01<<(7*2);
}

/*
*����е�״̬
*/
int key_read_col(void)
{
	int num = 0;

	switch(__REG(GPH2DAT)&0xf8)
	{
		case 0x08:
			num = 0x08;
			break;
		case 0x10:
			num = 0x10;
			break;
		case 0x20:
			num = 0x20;
			break;
		case 0x40:
			num = 0x40;
			break;
		case 0x80:
			num = 0x80;
			break;
		default:
			break;
	}
	
	return num;	
}

/*
*��������ɨ��ʼ��
*��5�����ó�����ߵ�ƽ
*��2�����ó�����������
*/
void row_scan_init(void)
{
	//����GPH3_0/1Ϊ����
	__REG(GPH3CON) &= ~(0x0f<<(0*4)|0x0f<<(1*4));
	//����GPH3_0/1����ʹ��
	__REG(GPH3PUD) &= ~(0x03<<(0*2)|0x03<<(1*2));
	__REG(GPH3PUD) |= 0x01<<(0*2)|0x01<<(1*2);

	//����GPH2_3/4/5/6/7Ϊ���
	__REG(GPH2CON) &= ~(0x0f<<(3*4)|0x0f<<(4*4)|0x0f<<(5*4)|0x0f<<(6*4)|0x0f<<(7*4));
	__REG(GPH2CON) |= (0x01<<(3*4)|0x01<<(4*4)|0x01<<(5*4)|0x01<<(6*4)|0x01<<(7*4));
	//����GPH2_3/4/5/6/7����ߵ�ƽ
	__REG(GPH2DAT) |= (0x01<<3|0x01<<4|0x01<<5|0x01<<6|0x01<<7);
}

/*
*����е�״̬
*/
int key_read_row(void)
{
	int num = 0;
	
	switch(__REG(GPH3DAT)&0x03)
	{
		case 0x01:
			num = 0x01;
			break;
		case 0x02:
			num = 0x02;
			break;
		default:
			break;
	}
	
	return num;
}

/******************************************************
	�����������ݰ���������ͬ�ĵ�
	* 			row_val		col_val			judge_val
	*1	key8		0x02		+	 0x10		=	    0x12
	*2	key2		0x01		+	 0x08		=	    0x09
	*3	key4    	0x01		+	 0x20		=	    0x21
	*4	key5  	0x01		+	 0x40		=	    0x41
	*5	key6		0x01		+	 0x80		=	    0x81
	*6	key3		0x01		+	 0x10		=	    0x11
	*7	key9		0x02		+	 0x20		=	    0x22
	*8	key11	0x02		+	 0x80		=	    0x82
	*9	key10	0x02		+	 0x40		=	    0x42
********************************************************/
void key_deal(int key_val)
{
	switch(key_val)
	{
		case 0x12:
			__REG(GPH0DAT) = 0x08; 
			break;
		case 0x09:
			__REG(GPH0DAT) = 0x02;
			break;
		case 0x21:
			__REG(GPH0DAT) = 0x04;
			break;
		case 0x41:
			__REG(GPH0DAT) = 0x05;
			break;
		case 0x81:
			__REG(GPH0DAT) = 0x06;
			break;
		case 0x11:
			__REG(GPH0DAT) = 0x03;
			break;
		case 0x22:
			__REG(GPH0DAT) = 0x09;
			break;
		case 0x82:
			__REG(GPH0DAT) = 0x0B;
			break;
		case 0x42:
			__REG(GPH0DAT) = 0x0A;
			break;
		default:
			break;
	}
}


#endif /*FUNC_ALL_KEY_POLL*/

#ifdef FUNC_ALL_KEY_INT

/*
*����2��Ϊ�����������ߵ�ƽ
*/
static void key_row_init(void)
{
	//��GPH3_0/1���ó����
	__REG(GPH3CON) &= ~(0x0f<<(0*4)|0x0f<<(1*4));
	__REG(GPH3CON) |= (0x01<<(0*4)|0x01<<(1*4));	

	//����GPH3_0/1����ߵ�ƽ
	__REG(GPH3DAT) |= ((0x01<<0)|(0x01<<1));
}

/*
* ����5��Ϊ�жϣ������ش���������ʹ��
*/
static void key_col_init(void)
{
	//��5�����ó��ж�ģʽ
	__REG(GPH2CON) |= (0x0F<<(3*4)|0x0F<<(4*4)|0x0F<<(4*5)|0x0F<<(4*6)|0x0F<<(4*7));
	
	//����ʹ��
	__REG(GPH2PUD) &= ~(0x03<<(2*3)|0x03<<(2*4)|0x03<<(2*5)|0x03<<(2*6)|0x03<<(2*7));
	__REG(GPH2PUD) |= (0x01<<(2*3)|0x01<<(2*4)|0x01<<(2*5)|0x01<<(2*6)|0x01<<(2*7));

	//�����ش�����ʽ
	__REG(EXT_INT_2_CON) &= ~(0x07<<(3*4)|0x07<<(4*4)|0x07<<(4*5)|0x07<<(4*6)|0x07<<(4*7));
	__REG(EXT_INT_2_CON) |= (0x03<<(3*4)|0x03<<(4*4)|0x03<<(4*5)|0x03<<(4*6)|0x03<<(4*7));
	
	//��������־
	__REG(EXT_INT_2_PEND) = 0xF8;
	
	//����λ��ֹ
	__REG(EXT_INT_2_MASK) &= ~(0xF8);
}

/*
*������ʼ��
*/
void key_init(void)
{
	key_row_init();
	key_col_init();
}

/*
*ȷ�����尴��
*/
void find_key(unsigned int pend_val)
{
	unsigned int value = 0;

	//����2��Ϊ����
	__REG(GPH3CON)  &= ~(0x0f<<(0*4)|0x0f<<(1*4));
	//����2������ʹ��
	__REG(GPH3PUD) &= ~(0x03<<(0*2)|0x03<<(1*2));
	__REG(GPH3PUD) |= (0x01<<(0*2)|0x01<<(1*2));
	
	//����5��Ϊ���
	__REG(GPH2CON)  &= ~(0x0f<<(3*4)|0x0f<<(4*4)|0x0f<<(5*4)|0x0f<<(6*4)|0x0f<<(7*4));
	__REG(GPH2CON)  |= (0x01<<(3*4)|0x01<<(4*4)|0x01<<(5*4)|0x01<<(6*4)|0x01<<(7*4));
	//����5������ߵ�ƽ
	__REG(GPH2DAT) |= ((0x01<<3)|(0x01<<4)|(0x01<<5)|(0x01<<6)|(0x01<<7));

	//PUD:����ʹ��
	__REG(GPH2PUD) &= ~(0x03<<(3*2)|0x03<<(4*2)|0x03<<(5*2)|0x03<<(6*2)|0x03<<(7*2));
	__REG(GPH2PUD) |= (0x02<<(3*2)|0x02<<(4*2)|0x02<<(5*2)|0x02<<(6*2)|0x02<<(7*2));	

	value = __REG(GPH3DAT) & 0x03;

	/*********************************************
	*col		key			EXT_INT_2_PEND
	*3��		key2		0x08		
	*4��		key3,8		0x10
	*5��		key4,9		0x20
	*6��		key5,10		0x40
	*7��		key6,11		0x80
	*********************************************/
	switch(pend_val)
	{
		case 0x08:						//col_03
			if(value == 0x01)			//key2
				__REG(GPH0DAT)  = 0x02;
			break;
		case 0x10:						//col_04
			if(value == 0x01)			//key3
				__REG(GPH0DAT)  = 0x03;
			else if(value== 0x02)		//key8
				__REG(GPH0DAT)  = 0x08;
			break;
		case 0x20:						//col_05
			if(value == 0x01)			//key4
				__REG(GPH0DAT)  = 0x04;
			else if(value== 0x02)		//key9
				__REG(GPH0DAT)  = 0x09;
			break;
		case 0x40:						//col_06
			if(value == 0x01)			//key5
				__REG(GPH0DAT)  = 0x05;
			else if(value== 0x02)		//key10
				__REG(GPH0DAT)  = 0x0A;
			break;
		case 0x80:						//col_07
			if(value == 0x01)			//key6
				__REG(GPH0DAT)  = 0x06;
			else if(value== 0x02)		//key11
				__REG(GPH0DAT)  = 0x0B;
			break;
		default:
			break;
	}
	
	//���³�ʼ����������Ϊ�������������ɨ��
	key_init();
}

#endif /*FUNC_ALL_KEY_INT*/
