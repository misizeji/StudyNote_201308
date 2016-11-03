#include "config.h"


#ifdef FUNC_ONE_KEY_POLL

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

#endif /*FUNC_ONE_KEY_POLL*/

#ifdef FUNC_ALL_KEY_POLL

/*
*按键的列扫初始化
*把5列配置成输入且下拉
*把2行配置成输出高电平
*/
void col_scan_init(void)
{
	//设置GPH3_0/1为输出
	__REG(GPH3CON) &= ~(0x0f<<(0*4)|0x0f<<(1*4));
	__REG(GPH3CON) |= (0x01<<(0*4)|0x01<<(1*4));
	//设置GPH3_0/1输出高电平
	__REG(GPH3DAT) |= (0x01<<0|0x01<<1);

	//设置GPH2_3/4/5/6/7为输入
	__REG(GPH2CON) &= ~(0x0f<<(3*4)|0x0f<<(4*4)|0x0f<<(5*4)|0x0f<<(6*4)|0x0f<<(7*4));
	//设置GPH2_3/4/5/6/7下拉使能
	__REG(GPH2PUD) &= ~(0x03<<(3*2)|0x03<<(4*2)|0x03<<(5*2)|0x03<<(6*2)|0x03<<(7*2));
	__REG(GPH2PUD) |= 0x01<<(3*2)|0x01<<(4*2)|0x01<<(5*2)|0x01<<(6*2)|0x01<<(7*2);
}

/*
*检查列的状态
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
*按键的行扫初始化
*把5列配置成输出高电平
*把2行配置成输入且下拉
*/
void row_scan_init(void)
{
	//设置GPH3_0/1为输入
	__REG(GPH3CON) &= ~(0x0f<<(0*4)|0x0f<<(1*4));
	//设置GPH3_0/1下拉使能
	__REG(GPH3PUD) &= ~(0x03<<(0*2)|0x03<<(1*2));
	__REG(GPH3PUD) |= 0x01<<(0*2)|0x01<<(1*2);

	//设置GPH2_3/4/5/6/7为输出
	__REG(GPH2CON) &= ~(0x0f<<(3*4)|0x0f<<(4*4)|0x0f<<(5*4)|0x0f<<(6*4)|0x0f<<(7*4));
	__REG(GPH2CON) |= (0x01<<(3*4)|0x01<<(4*4)|0x01<<(5*4)|0x01<<(6*4)|0x01<<(7*4));
	//设置GPH2_3/4/5/6/7输出高电平
	__REG(GPH2DAT) |= (0x01<<3|0x01<<4|0x01<<5|0x01<<6|0x01<<7);
}

/*
*检查行的状态
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
	按键处理，根据按键点亮不同的灯
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
*配置2行为输出，且输出高电平
*/
static void key_row_init(void)
{
	//将GPH3_0/1设置成输出
	__REG(GPH3CON) &= ~(0x0f<<(0*4)|0x0f<<(1*4));
	__REG(GPH3CON) |= (0x01<<(0*4)|0x01<<(1*4));	

	//设置GPH3_0/1输出高电平
	__REG(GPH3DAT) |= ((0x01<<0)|(0x01<<1));
}

/*
* 配置5列为中断，上升沿触发，下拉使能
*/
static void key_col_init(void)
{
	//将5列设置成中断模式
	__REG(GPH2CON) |= (0x0F<<(3*4)|0x0F<<(4*4)|0x0F<<(4*5)|0x0F<<(4*6)|0x0F<<(4*7));
	
	//下拉使能
	__REG(GPH2PUD) &= ~(0x03<<(2*3)|0x03<<(2*4)|0x03<<(2*5)|0x03<<(2*6)|0x03<<(2*7));
	__REG(GPH2PUD) |= (0x01<<(2*3)|0x01<<(2*4)|0x01<<(2*5)|0x01<<(2*6)|0x01<<(2*7));

	//上升沿触发方式
	__REG(EXT_INT_2_CON) &= ~(0x07<<(3*4)|0x07<<(4*4)|0x07<<(4*5)|0x07<<(4*6)|0x07<<(4*7));
	__REG(EXT_INT_2_CON) |= (0x03<<(3*4)|0x03<<(4*4)|0x03<<(4*5)|0x03<<(4*6)|0x03<<(4*7));
	
	//清除挂起标志
	__REG(EXT_INT_2_PEND) = 0xF8;
	
	//屏蔽位禁止
	__REG(EXT_INT_2_MASK) &= ~(0xF8);
}

/*
*按键初始化
*/
void key_init(void)
{
	key_row_init();
	key_col_init();
}

/*
*确定具体按键
*/
void find_key(unsigned int pend_val)
{
	unsigned int value = 0;

	//配置2行为输入
	__REG(GPH3CON)  &= ~(0x0f<<(0*4)|0x0f<<(1*4));
	//配置2行下拉使能
	__REG(GPH3PUD) &= ~(0x03<<(0*2)|0x03<<(1*2));
	__REG(GPH3PUD) |= (0x01<<(0*2)|0x01<<(1*2));
	
	//配置5列为输出
	__REG(GPH2CON)  &= ~(0x0f<<(3*4)|0x0f<<(4*4)|0x0f<<(5*4)|0x0f<<(6*4)|0x0f<<(7*4));
	__REG(GPH2CON)  |= (0x01<<(3*4)|0x01<<(4*4)|0x01<<(5*4)|0x01<<(6*4)|0x01<<(7*4));
	//配置5列输出高电平
	__REG(GPH2DAT) |= ((0x01<<3)|(0x01<<4)|(0x01<<5)|(0x01<<6)|(0x01<<7));

	//PUD:上拉使能
	__REG(GPH2PUD) &= ~(0x03<<(3*2)|0x03<<(4*2)|0x03<<(5*2)|0x03<<(6*2)|0x03<<(7*2));
	__REG(GPH2PUD) |= (0x02<<(3*2)|0x02<<(4*2)|0x02<<(5*2)|0x02<<(6*2)|0x02<<(7*2));	

	value = __REG(GPH3DAT) & 0x03;

	/*********************************************
	*col		key			EXT_INT_2_PEND
	*3列		key2		0x08		
	*4列		key3,8		0x10
	*5列		key4,9		0x20
	*6列		key5,10		0x40
	*7列		key6,11		0x80
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
	
	//重新初始化按键，因为上面进行了逆向扫描
	key_init();
}

#endif /*FUNC_ALL_KEY_INT*/
