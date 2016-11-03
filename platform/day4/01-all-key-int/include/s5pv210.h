#ifndef __S5PV210_H__
#define __S5PV210_H__


typedef unsigned char			uchar;
typedef unsigned long 			ulong;
typedef unsigned int 			uint;
typedef volatile unsigned long	vu_long;
typedef volatile unsigned short vu_short;
typedef volatile unsigned char	vu_char;

typedef vu_char		S5PC11X_REG8;
typedef vu_short	S5PC11X_REG16;
typedef vu_long		S5PC11X_REG32;

#define UData(Data)	((unsigned long) (Data))

#define __REG(x)	(*(vu_long *)(x))
#define __REGl(x)	(*(vu_long *)(x))
#define __REGw(x)	(*(vu_short *)(x))
#define __REGb(x)	(*(vu_char *)(x))
#define __REG2(x,y)	(*(vu_long *)((x) + (y)))

/*GPIO*/
#define ELFIN_GPIO_BASE		0xE0200000

#define GPA0CON				0xE0200000	 
#define GPA0DAT				0xE0200004		 
#define GPA0PUD				0xE0200008		 
#define GPA0DRV				0xE020000C
#define GPA0CONPDN			0xE0200010	 
#define GPA0PUDPDN			0xE0200014

#define GPA1CON				0xE0200020
#define GPA1DAT				0xE0200024	 
#define GPA1PUD				0xE0200028	 
#define GPA1DRV				0xE020002C	 
#define GPA1CONPDN			0xE0200030 	 
#define GPA1PUDPDN			0xE0200034

#define GPD0CON				0xE02000A0
#define GPD0DAT				0xE02000A4	 
#define GPD0PUD				0xE02000A8	 
#define GPD0DRV				0xE02000AC
#define GPD0CONPDN			0xE02000B0	 
#define GPD0PUDPDN			0xE02000B4	

#define GPD1CON				0xE02000C0
#define GPD1DAT				0xE02000C4 
#define GPD1PUD				0xE02000C8 
#define GPD1DRV				0xE02000CC
#define GPD1CONPDN			0xE02000D0	 
#define GPD1PUDPDN			0xE02000D4	

#define GPH0CON				0xE0200C00
#define GPH0DAT				0xE0200C04 	 
#define GPH0PUD				0xE0200C08 	 
#define GPH0DRV				0xE0200C0C

#define GPH1CON				0xE0200C20
#define GPH1DAT				0xE0200C24	 
#define GPH1PUD				0xE0200C28	 
#define GPH1DRV				0xE0200C2C

#define GPH2CON				0xE0200C40
#define GPH2DAT				0xE0200C44	 
#define GPH2PUD				0xE0200C48	 
#define GPH2DRV				0xE0200C4C

#define GPH3CON				0xE0200C60
#define GPH3DAT				0xE0200C64 
#define GPH3PUD				0xE0200C68 
#define GPH3DRV				0xE0200C6C

/*CLOCK*/
#define APLL_LOCK_REG		__REG(0xE0100000)
#define MPLL_LOCK_REG		__REG(0xE0100008)
#define EPLL_LOCK_REG		__REG(0xE0100010)
#define VPLL_LOCK_REG		__REG(0xE0100020)

#define APLL_CON0_REG		__REG(0xE0100100)
#define APLL_CON1_REG		__REG(0xE0100104)
#define MPLL_CON_REG		__REG(0xE0100108)
#define EPLL_CON0_REG		__REG(0xE0100110)
#define EPLL_CON1_REG		__REG(0xE0100114)
#define VPLL_CON_REG		__REG(0xE0100120)

#define CLK_SRC0_REG		__REG(0xE0100200)
#define CLK_SRC1_REG		__REG(0xE0100204)
#define CLK_SRC2_REG		__REG(0xE0100208)
#define CLK_SRC3_REG		__REG(0xE010020C)
#define CLK_SRC4_REG		__REG(0xE0100210)
#define CLK_SRC5_REG		__REG(0xE0100214)
#define CLK_SRC6_REG		__REG(0xE0100218)

#define CLK_SRC_MASK0_REG	__REG(0xE0100280)
#define CLK_SRC_MASK1_REG	__REG(0xE0100284)

#define CLK_DIV0_REG		__REG(0xE0100300)
#define CLK_DIV1_REG		__REG(0xE0100304)
#define CLK_DIV2_REG		__REG(0xE0100308)
#define CLK_DIV3_REG		__REG(0xE010030C)
#define CLK_DIV4_REG		__REG(0xE0100310)
#define CLK_DIV5_REG		__REG(0xE0100314)
#define CLK_DIV6_REG		__REG(0xE0100318)
#define CLK_DIV7_REG		__REG(0xE010031C)

#define CLK_OUT_REG			__REG(0xE0100500)

#define CLK_GATE_IP0_REG	__REG(0xE0100460)
#define CLK_GATE_IP1_REG	__REG(0xE0100464)
#define CLK_GATE_IP2_REG	__REG(0xE0100468)
#define CLK_GATE_IP3_REG	__REG(0xE010046C)
#define CLK_GATE_IP4_REG	__REG(0xE0100470)
#define CLK_GATE_BLOCK_REG	__REG(0xE0100480)

#define CLK_DIV_STAT0_REG	__REG(0xE0101000)
#define CLK_DIV_STAT1_REG	__REG(0xE0101004)
#define CLK_MUX_STAT0_REG	__REG(0xE0101100)
#define CLK_MUX_STAT1_REG	__REG(0xE0101104)

/*EXTERN INTERRUPT*/
#define EXT_INT_0_CON		0xE0200E00
#define EXT_INT_1_CON		0xE0200E04
#define EXT_INT_2_CON		0xE0200E08
#define EXT_INT_3_CON		0xE0200E0C

#define EXT_INT_0_MASK		0xE0200F00
#define EXT_INT_1_MASK		0xE0200F04
#define EXT_INT_2_MASK		0xE0200F08
#define EXT_INT_3_MASK		0xE0200F0C

#define EXT_INT_0_PEND		0xE0200F40
#define EXT_INT_1_PEND		0xE0200F44
#define EXT_INT_2_PEND		0xE0200F48
#define EXT_INT_3_PEND		0xE0200F4C

/*VIC*/
#define VIC0IRQSTATUS 		0xF2000000	
#define VIC0FIQSTATUS  		0xF2000004	
#define VIC0RAWINTR			0xF2000008	
#define VIC0INTSELECT 		0xF200000C	
#define VIC0INTENABLE  		0xF2000010	
#define VIC0INTENCLEAR		0xF2000014	
#define VIC0SOFTINT  		0xF2000018
#define VIC0SOFTINTCLEAR	0xF200001C

#define VIC0ADDRESS			0xF2000F00	

#define VIC0VECTADDR0		0xF2000100
#define VIC0VECTADDR1		0xF2000104
#define VIC0VECTADDR2		0xF2000108
#define VIC0VECTADDR3		0xF200010C
#define VIC0VECTADDR4		0xF2000110
#define VIC0VECTADDR5		0xF2000114
#define VIC0VECTADDR6		0xF2000118
#define VIC0VECTADDR7		0xF200011C
#define VIC0VECTADDR8		0xF2000120
#define VIC0VECTADDR9		0xF2000124
#define VIC0VECTADDR10		0xF2000128
#define VIC0VECTADDR11		0xF200012C
#define VIC0VECTADDR12		0xF2000130
#define VIC0VECTADDR13		0xF2000134
#define VIC0VECTADDR14		0xF2000138
#define VIC0VECTADDR15		0xF200013C
#define VIC0VECTADDR16		0xF2000140
#define VIC0VECTADDR17		0xF2000144
#define VIC0VECTADDR18		0xF2000148
#define VIC0VECTADDR19		0xF200014C
#define VIC0VECTADDR20		0xF2000150
#define VIC0VECTADDR21		0xF2000154
#define VIC0VECTADDR22		0xF2000158
#define VIC0VECTADDR23		0xF200015C
#define VIC0VECTADDR24		0xF2000160
#define VIC0VECTADDR25		0xF2000164
#define VIC0VECTADDR26		0xF2000168
#define VIC0VECTADDR27		0xF200016C
#define VIC0VECTADDR28		0xF2000170
#define VIC0VECTADDR29		0xF2000174
#define VIC0VECTADDR30		0xF2000178
#define VIC0VECTADDR31		0xF200017C

/*UART*/
#define UART_CONSOLE 		0XE2900000

typedef struct {
	S5PC11X_REG32	ULCON;
	S5PC11X_REG32	UCON;
	S5PC11X_REG32	UFCON;
	S5PC11X_REG32	UMCON;
	S5PC11X_REG32	UTRSTAT;
	S5PC11X_REG32	UERSTAT;
	S5PC11X_REG32	UFSTAT;
	S5PC11X_REG32	UMSTAT;
	S5PC11X_REG8	UTXH;
	S5PC11X_REG8	res1[3];
	S5PC11X_REG8	URXH;
	S5PC11X_REG8	res2[3];
	S5PC11X_REG32	UBRDIV;
	S5PC11X_REG32	UDIVSLOT;
}S5PC11X_UART;

/*
 * PWM timer
 */
#define ELFIN_TIMER_BASE	0xE2500000

#define TCFG0_REG			__REG(0xE2500000)
#define TCFG1_REG			__REG(0xE2500004)
#define TCON_REG			__REG(0xE2500008)
#define TCNTB0_REG			__REG(0xE250000c)
#define TCMPB0_REG			__REG(0xE2500010)
#define TCNTO0_REG			__REG(0xE2500014)
#define TCNTB1_REG			__REG(0xE2500018)
#define TCMPB1_REG			__REG(0xE250001c)
#define TCNTO1_REG			__REG(0xE2500020)
#define TCNTB2_REG			__REG(0xE2500024)
#define TCMPB2_REG			__REG(0xE2500028)
#define TCNTO2_REG			__REG(0xE250002c)
#define TCNTB3_REG			__REG(0xE2500030)
#define TCMPB3_REG			__REG(0xE2500034)
#define TCNTO3_REG			__REG(0xE2500038)
#define TCNTB4_REG			__REG(0xE250003c)
#define TCNTO4_REG			__REG(0xE2500040)
#define TINT_CSTAT			__REG(0xE2500044)

typedef struct {
	S5PC11X_REG32	TCNTB;		// Timer Count Buffer Register at page804
	S5PC11X_REG32	TCMPB;		// Timer Compare Buffer Register at page804
	S5PC11X_REG32	TCNTO;		// Timer 4 Count Observation Register at page804
}  S5PC11X_TIMER;

typedef struct {
	S5PC11X_REG32	TCFG0;		//Timer Configuration Register 0 at page804 
	S5PC11X_REG32	TCFG1;		//Timer Configuration Register 1 at page804 
	S5PC11X_REG32	TCON;		//Timer Control Register at page804
	S5PC11X_TIMER	ch[4];
	S5PC11X_REG32	TCNTB4;		//Timer 4 Count Buffer Register at page804
	S5PC11X_REG32	TCNTO4;		//Timer 4 Count Observation Register at page804
}  S5PC11X_TIMERS;

static inline S5PC11X_TIMERS * S5PC11X_GetBase_TIMERS(void)
{
	return (S5PC11X_TIMERS *)ELFIN_TIMER_BASE;
}

/* auto reload on/off for Timer 4 */
#define TCON_4_AUTO			(1 << 22)       
#define TIMER4_ATLOAD_ON	(TCON_4_AUTO*1)

/* manual Update TCNTB4,TCMPB4 */
#define TCON_4_MAN			(1 << 21)       
#define TIMER4_MANUP		(TCON_4_MAN*1)

/* 0: Stop, 1: start Timer 4 */
#define TCON_4_ONOFF		(1 << 20)       
#define TIMER4_ON			(TCON_4_ONOFF*1)
#define TIMER4_OFF			(TCON_4_ONOFF*0)

/* auto reload on/off for Timer 3 */
#define TCON_3_AUTO			(1 << 19)       
#define TIMER3_ATLOAD_ON	(TCON_3_AUTO*1)

/* 1: Inverter on for TOUT3 */
#define TCON_3_INVERT		(1 << 18)       
#define TIMER3_IVT_ON		(TCON_3_INVERT*1)

/* manual Update TCNTB3,TCMPB3 */
#define TCON_3_MAN			(1 << 17)       
#define TIMER3_MANUP		(TCON_3_MAN*1)

/* 0: Stop, 1: start Timer 3 */
#define TCON_3_ONOFF		(1 << 16)       
#define TIMER3_ON			(TCON_3_ONOFF*1)
#define TIMER3_OFF			(TCON_3_ONOFF*0)

/* auto reload on/off for Timer 2 */
#define TCON_2_AUTO			(1 << 15)       
#define TIMER2_ATLOAD_ON	(TCON_2_AUTO*1)

/* 1: Inverter on for TOUT2 */
#define TCON_2_INVERT		(1 << 14)       
#define TIMER2_IVT_ON		(TCON_2_INVERT*1)

/* manual Update TCNTB2,TCMPB2 */
#define TCON_2_MAN			(1 << 13)       
#define TIMER2_MANUP		(TCON_2_MAN*1)

/* 0: Stop, 1: start Timer 2 */
#define TCON_2_ONOFF		(1 << 12)       
#define TIMER2_ON			(TCON_2_ONOFF*1)
#define TIMER2_OFF			(TCON_2_ONOFF*0)

/* auto reload on/off for Timer 1 */
#define TCON_1_AUTO			(1 << 11)       
#define TIMER1_ATLOAD_ON	(TCON_1_AUTO*1)

/* 1: Inverter on for TOUT1 */
#define TCON_1_INVERT		(1 << 10)       
#define TIMER1_IVT_ON		(TCON_1_INVERT*1)

/* manual Update TCNTB1,TCMPB1 */
#define TCON_1_MAN			(1 << 9)       
#define TIMER1_MANUP		(TCON_1_MAN*1)

/* 0: Stop, 1: start Timer 1 */
#define TCON_1_ONOFF		(1 << 8)       
#define TIMER1_ON			(TCON_1_ONOFF*1)
#define TIMER1_OFF			(TCON_1_ONOFF*0)

/* auto reload on/off for Timer 0 */
#define TCON_0_AUTO			(1 << 3)       
#define TIMER0_ATLOAD_ON	(TCON_0_AUTO*1)

/* 1: Inverter on for TOUT0 */
#define TCON_0_INVERT		(1 << 2)       
#define TIMER0_IVT_ON		(TCON_0_INVERT*1)

/* manual Update TCNTB0,TCMPB0 */
#define TCON_0_MAN			(1 << 1)       
#define TIMER0_MANUP		(TCON_0_MAN*1)

/* 0: Stop, 1: start Timer 0 */
#define TCON_0_ONOFF		(1 << 0)       
#define TIMER0_ON			(TCON_0_ONOFF*1)
#define TIMER0_OFF			(TCON_0_ONOFF*0)

//NAND FLASH
#define MP01CON_REG			__REG(0xE02002E0)
#define MP01DAT_REG			__REG(0xE02002E4)		 
#define MP01PUD_REG			__REG(0xE02002E8)		 
#define MP01DRV_REG			__REG(0xE02002EC)
#define MP01CONPDN_REG		__REG(0xE02002F0)	 
#define MP01PUDPDN_REG		__REG(0xE02002F4)

#define MP02CON_REG         __REG(0xE0200300)
#define MP02DAT_REG         __REG(0xE0200304)          
#define MP02PUD_REG         __REG(0xE0200308)          
#define MP02DRV_REG         __REG(0xE020030C)
#define MP02CONPDN_REG      __REG(0xE0200310)       
#define MP02PUDPDN_REG      __REG(0xE0200314)

#define MP03CON_REG         __REG(0xE0200320)
#define MP03DAT_REG         __REG(0xE0200324)          
#define MP03PUD_REG         __REG(0xE0200328)          
#define MP03DRV_REG         __REG(0xE020032C)
#define MP03CONPDN_REG      __REG(0xE0200330)       
#define MP03PUDPDN_REG      __REG(0xE0200334)

#define NFCONF_REG			__REG(0xB0E00000)
#define NFCONT_REG			__REG(0xB0E00004)
#define NFCMD_REG			__REG(0xB0E00008)
#define NFADDR_REG          __REG(0xB0E0000C)
#define NFDATA_REG          __REG(0xB0E00010)
#define NFDATA8_REG         __REGb(0xB0E00010)
#define NFSTAT_REG          __REG(0xB0E00028)

//Standard NAND flash commands
#define NAND_CMD_READ0			0
#define NAND_CMD_READ1			1
#define NAND_CMD_RNDOUT			5
#define NAND_CMD_PAGEPROG		0x10
#define NAND_CMD_READOOB		0x50
#define NAND_CMD_ERASE1			0x60
#define NAND_CMD_STATUS			0x70
#define NAND_CMD_STATUS_MULTI	0x71
#define NAND_CMD_SEQIN			0x80
#define NAND_CMD_RNDIN			0x85
#define NAND_CMD_READID			0x90
#define NAND_CMD_ERASE2			0xd0
#define NAND_CMD_RESET			0xff
//Extended commands for large page devices 
#define NAND_CMD_READSTART	0x30
#define NAND_CMD_RNDOUTSTART	0xE0
#define NAND_CMD_CACHEDPROG	0x15

#define NAND_FLASH_PAGE_SIZE	(1<<11)
#define NAND_FLASH_BLOCK_SIZE	(1<<17)

#define NAND_DISABLE_CE()	(NFCONT_REG |= (1 << 1))
#define NAND_ENABLE_CE()	(NFCONT_REG &= ~(1 << 1))
#define NF_TRANSRnB()		do { while(!(NFSTAT_REG & (1 << 0))); } while(0)

#define NFCONF_VAL	(7<<12)|(7<<8)|(7<<4)|(0<<3)|(1<<2)|(1<<1)|(0<<0)       
#define NFCONT_VAL	(0<<18)|(0<<17)|(0<<16)|(0<<10)|(0<<9)|(0<<8)|(0<<7)|(0<<6)|(0x3<<1)|(1<<0)
#define MP03CON_VAL	(1<<29)|(1<<25)|(1<<21)|(1<<17)|(1<<13)|(1<<9)|(1<<5)|(1<<1)


#endif /*_S5PV210_H_*/