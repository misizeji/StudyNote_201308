
#include <s5pc110.h>
#include <smdkv210single.h>

void led_test(unsigned int val)
{
	__REG(ELFIN_GPIO_BASE+GPH0CON_OFFSET) = 0x1111;
	__REG(ELFIN_GPIO_BASE+GPH0PUD_OFFSET) = 0x5500;
	__REG(ELFIN_GPIO_BASE+GPH0DAT_OFFSET) = val;
}
void disable_watch_dog(void)
{
	WTCON_REG = 0;
}

void init_system_clock(void)
{
	int i = 0x10000;
	
	CLK_SRC0_REG = 0x0;
	APLL_LOCK_REG = APLL_LOCKTIME_VAL;
	/* Disable PLL */
	do{
		APLL_CON0_REG = 0x0;
		MPLL_CON_REG = 0x0;//�ر�PLL
		MPLL_CON_REG = 0x0;//�ڶ�����0Ҳ����Ҫ��û��ԭ��
		CLK_DIV0_REG &= ~(CLK_DIV0_MASK);
		CLK_DIV0_REG |= CLK_DIV0_VAL;//���÷�Ƶϵ��
		APLL_CON0_REG = APLL_VAL;//�������໷
		MPLL_CON_REG = MPLL_VAL;
		VPLL_CON_REG = VPLL_VAL;
		APLL_CON1_REG = AFC_ON;//�����Զ�Ƶ�ʿ���

		for(i = 0x10000;i>1;i--);
		MPLL_CON_REG |= (1<<29);//������
		for(i = 0x100;i>1;i--);
	}while((MPLL_CON_REG&(1<<29)) !=(1<<29));//���ж��Ƿ����

	/* H/W lock detect disable */
	MPLL_CON_REG &= ~(1<<29);
	
	CLK_SRC0_REG |= 0x10001111;
	//CLK_DIV6_REG &= ~(0x7<<12);
	
}
void mem_setup(void)
{
	unsigned int i = 0x10000;
/* DMC0 Drive Strength (Setting 2X) */
	MP10DRV_REG = 0x0000AAAA;
	MP11DRV_REG = 0x0000AAAA;
	MP12DRV_REG = 0x0000AAAA;
	MP13DRV_REG = 0x0000AAAA;
	MP14DRV_REG = 0x0000AAAA;
	MP15DRV_REG = 0x0000AAAA;
	MP16DRV_REG = 0x0000AAAA;
	MP17DRV_REG = 0x0000AAAA;
	MP18DRV_REG = 0x00002AAA;
	
	/* DMC1 Drive Strength (Setting 2X) */
	MP20DRV_REG = 0x0000AAAA;
	MP21DRV_REG = 0x0000AAAA;
	MP22DRV_REG = 0x0000AAAA;
	MP23DRV_REG = 0x0000AAAA;
	MP24DRV_REG = 0x0000AAAA;
	MP25DRV_REG = 0x0000AAAA;
	MP26DRV_REG = 0x0000AAAA;
	MP27DRV_REG = 0x0000AAAA;
	MP28DRV_REG = 0x00002AAA;

/* DMC0 initialization at single Type*/
	PHYCONTROL0_REG = 0x00101000;
	PHYCONTROL1_REG = 0x00000086;
	PHYCONTROL0_REG = 0x00101002;
	PHYCONTROL0_REG = 0x00101003;
	
	//while((PHYSTATUS_REG&0x07) !=0x07);
#if 0	
	led_test(0x01);
	asm("	ldr	r0, =0xF0000000\n"
		"find_lock_val:\n"
		"	ldr	r1, [r0, #0x40]\n"
		"	and	r2, r1, #0x7\n"
		"	cmp	r2, #0x7\n"
		"	bne	find_lock_val\n");
	led_test(0x08);
#endif
	for(i = 0x10000;i>1;i--);
	PHYCONTROL0_REG = (((PHYSTATUS_REG&0x3fc0)<<18)|(0x100000)|(0x1000)|(0x03));

/* setting DDR2 */
	__REG(APB_DMC_0_BASE+DMC_CONCONTROL) = 0x0FFF2010;		
	__REG(APB_DMC_0_BASE+DMC_MEMCONTROL) = 0x00212400;		
	__REG(APB_DMC_0_BASE+DMC_MEMCONFIG0) = DMC0_MEMCONFIG_0;		
	__REG(APB_DMC_0_BASE+DMC_MEMCONFIG1) = DMC0_MEMCONFIG_1;		
	__REG(APB_DMC_0_BASE+DMC_PRECHCONFIG) = 0xFF000000;		
	__REG(APB_DMC_0_BASE+DMC_TIMINGAREF) = DMC0_TIMINGA_REF;
	__REG(APB_DMC_0_BASE+DMC_TIMINGROW) = DMC0_TIMING_ROW;
	__REG(APB_DMC_0_BASE+DMC_TIMINGDATA) = DMC0_TIMING_DATA;
	__REG(APB_DMC_0_BASE+DMC_TIMINGPOWER) = DMC0_TIMING_PWR;
	
	__REG(APB_DMC_0_BASE+DMC_DIRECTCMD) = 0x07000000;
	__REG(APB_DMC_0_BASE+DMC_DIRECTCMD) = 0x01000000;
	__REG(APB_DMC_0_BASE+DMC_DIRECTCMD) = 0x00020000;
	__REG(APB_DMC_0_BASE+DMC_DIRECTCMD) = 0x00030000;
	__REG(APB_DMC_0_BASE+DMC_DIRECTCMD) = 0x00010400;
	__REG(APB_DMC_0_BASE+DMC_DIRECTCMD) = 0x00000542;
	__REG(APB_DMC_0_BASE+DMC_DIRECTCMD) = 0x01000000;
	__REG(APB_DMC_0_BASE+DMC_DIRECTCMD) = 0x05000000;
	__REG(APB_DMC_0_BASE+DMC_DIRECTCMD) = 0x05000000;
	__REG(APB_DMC_0_BASE+DMC_DIRECTCMD) = 0x00000442;
	__REG(APB_DMC_0_BASE+DMC_DIRECTCMD) = 0x00010780;
	__REG(APB_DMC_0_BASE+DMC_DIRECTCMD) = 0x00010400;
	__REG(APB_DMC_0_BASE+DMC_DIRECTCMD) = 0x07100000;
	__REG(APB_DMC_0_BASE+DMC_DIRECTCMD) = 0x01100000;
	__REG(APB_DMC_0_BASE+DMC_DIRECTCMD) = 0x00120000;
	__REG(APB_DMC_0_BASE+DMC_DIRECTCMD) = 0x00130000;
	__REG(APB_DMC_0_BASE+DMC_DIRECTCMD) = 0x00110400;
	__REG(APB_DMC_0_BASE+DMC_DIRECTCMD) = 0x00100542;
	__REG(APB_DMC_0_BASE+DMC_DIRECTCMD) = 0x01100000;
	__REG(APB_DMC_0_BASE+DMC_DIRECTCMD) = 0x05100000;
	__REG(APB_DMC_0_BASE+DMC_DIRECTCMD) = 0x05100000;
	__REG(APB_DMC_0_BASE+DMC_DIRECTCMD) = 0x00100442;
	__REG(APB_DMC_0_BASE+DMC_DIRECTCMD) = 0x00110780;
	__REG(APB_DMC_0_BASE+DMC_DIRECTCMD) = 0x00110400;
	__REG(APB_DMC_0_BASE+DMC_CONCONTROL) = 0x0FF02030;
	__REG(APB_DMC_0_BASE+DMC_PWRDNCONFIG) = 0xFFFF00FF;
	__REG(APB_DMC_0_BASE+DMC_MEMCONTROL) = 0x00202400;

/* DMC1 initialization */
	PHYCONTROL10_REG = 0x00101000;
	PHYCONTROL11_REG = 0x00000086;
	PHYCONTROL10_REG = 0x00101002;
	PHYCONTROL10_REG = 0x00101003;
#if 0	
	//while((PHYSTATUS1_REG&0x07) !=0x07);
	asm("	ldr	r0, =0xF1400000\n"
		"find_lock_val2:\n"
		"	ldr	r1, [r0, #0x40]\n"
		"	and	r2, r1, #0x7\n"
		"	cmp	r2, #0x7\n"
		"	bne	find_lock_val2\n");
#endif
	for(i = 0x10000;i>1;i--);
	PHYCONTROL10_REG = (((PHYSTATUS1_REG&0x3fc0)<<18)|(0x100000)|(0x1000)|(0x03));

	/* settinf fot DDR2 */
	__REG(APB_DMC_1_BASE+DMC_CONCONTROL) = 0x0FFF2010;		
	__REG(APB_DMC_1_BASE+DMC_MEMCONTROL) = DMC1_MEMCONTROL;		
	__REG(APB_DMC_1_BASE+DMC_MEMCONFIG0) = DMC1_MEMCONFIG_0;		
	__REG(APB_DMC_1_BASE+DMC_MEMCONFIG1) = DMC1_MEMCONFIG_1;		
	__REG(APB_DMC_1_BASE+DMC_PRECHCONFIG) = 0xFF000000;		
	__REG(APB_DMC_1_BASE+DMC_TIMINGAREF) = DMC0_TIMINGA_REF;
	__REG(APB_DMC_1_BASE+DMC_TIMINGROW) = DMC0_TIMING_ROW;
	__REG(APB_DMC_1_BASE+DMC_TIMINGDATA) = DMC0_TIMING_DATA;
	__REG(APB_DMC_1_BASE+DMC_TIMINGPOWER) = DMC0_TIMING_PWR;

	__REG(APB_DMC_1_BASE+DMC_DIRECTCMD) = 0x07000000;
	__REG(APB_DMC_1_BASE+DMC_DIRECTCMD) = 0x01000000;
	__REG(APB_DMC_1_BASE+DMC_DIRECTCMD) = 0x00020000;
	__REG(APB_DMC_1_BASE+DMC_DIRECTCMD) = 0x00030000;
	__REG(APB_DMC_1_BASE+DMC_DIRECTCMD) = 0x00010400;
	__REG(APB_DMC_1_BASE+DMC_DIRECTCMD) = 0x00000542;
	__REG(APB_DMC_1_BASE+DMC_DIRECTCMD) = 0x01000000;
	__REG(APB_DMC_1_BASE+DMC_DIRECTCMD) = 0x05000000;
	__REG(APB_DMC_1_BASE+DMC_DIRECTCMD) = 0x05000000;
	__REG(APB_DMC_1_BASE+DMC_DIRECTCMD) = 0x00000442;
	__REG(APB_DMC_1_BASE+DMC_DIRECTCMD) = 0x00010780;
	__REG(APB_DMC_1_BASE+DMC_DIRECTCMD) = 0x00010400;
	__REG(APB_DMC_1_BASE+DMC_DIRECTCMD) = 0x07100000;
	__REG(APB_DMC_1_BASE+DMC_DIRECTCMD) = 0x01100000;
	__REG(APB_DMC_1_BASE+DMC_DIRECTCMD) = 0x00120000;
	__REG(APB_DMC_1_BASE+DMC_DIRECTCMD) = 0x00130000;
	__REG(APB_DMC_1_BASE+DMC_DIRECTCMD) = 0x00110400;
	__REG(APB_DMC_1_BASE+DMC_DIRECTCMD) = 0x00100542;
	__REG(APB_DMC_1_BASE+DMC_DIRECTCMD) = 0x01100000;
	__REG(APB_DMC_1_BASE+DMC_DIRECTCMD) = 0x05100000;
	__REG(APB_DMC_1_BASE+DMC_DIRECTCMD) = 0x05100000;
	__REG(APB_DMC_1_BASE+DMC_DIRECTCMD) = 0x00100442;
	__REG(APB_DMC_1_BASE+DMC_DIRECTCMD) = 0x00110780;
	__REG(APB_DMC_1_BASE+DMC_DIRECTCMD) = 0x00110400;
	__REG(APB_DMC_1_BASE+DMC_CONCONTROL) = 0x0FF02030;
	__REG(APB_DMC_1_BASE+DMC_PWRDNCONFIG) = 0xFFFF00FF;
	__REG(APB_DMC_1_BASE+DMC_MEMCONTROL) = 0x00202400;

}
void inituart(void)
{
	unsigned int i;
	S5PC11X_UART *uart = (S5PC11X_UART *)(ELFIN_UART_CONSOLE_BASE);//uart0

	GPA0_CON = 0x2222;//����GPIO

	uart->UFCON = 0x0;//�ر�FIFO
	uart->UMCON = 0x0;//�ر�AFC
	uart->ULCON = 0x3;//�Ǻ���ģʽ,����żУ��,data=8bit
	uart->UCON = 0x5;//ʹ��PCLK,�رո����ж�״̬,polling
	uart->UBRDIV = UART_UBRDIV_VAL;//DIV_VAL = (PCLK / (bps x 16))-1(PCLK=66M)
	uart->UDIVSLOT = UART_UDIVSLOT_VAL;//(num of 1's in UDIVSLOTn)/16 = 0.x(UDIVSLOTn=16*0.x)
	uart->UTXH= 'A';
}
void nand_init(void)
{
	MP01CON_REG &= ~((0xf<<8)); 
	MP01CON_REG |= (0x3<<8); 
	MP01PUD_REG &= ~((0x3<<4)); 
	MP03CON_REG &= ~(0xFFFFFF);
	MP03CON_REG = 0x22222222;
	MP03PUD_REG &= ~0x3fff;
	
	NFCONF_REG &=  ~0x777F;
	NFCONF_REG |= NFCONF_VAL;
	NFCONT_REG &= ~0x707C7;
	NFCONT_REG |= NFCONT_VAL;
}
