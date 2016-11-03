#include "config.h"


#define IRQ_PUSH __asm__(	\
	"ldr sp, =0xD0034000		\n"\
	"subs lr, lr, #4				\n"\
	"stmfd sp!,{r0-r12, lr}		\n"\
	);

#define IRQ_POP __asm__(		\
	"ldmfd sp!,{r0-r12,pc}^	\n"\
	);

//enable CPSR I-bit
#define IRQ_ENABLE __asm__ (\
	"mrs r0, cpsr				\n"\
	"bic  r0, r0, #0x80			\n"\
	"msr cpsr_c, r0			\n"\
	);

#ifdef FUNC_TIMER_DELAY
/*
* 单位: 延时1 s
*/
void delay(int num)
{	
	S5PC11X_TIMERS *const timers = S5PC11X_GetBase_TIMERS();
	
	//设置预分频为65
	timers->TCFG0 &= ~(0xff);
	timers->TCFG0 |= 65;			

	//选择1/4  分频
	timers->TCFG1 &= ~(0x0f);
	timers->TCFG1 |= 0x02;

	//设置初值寄存器
	timers->ch[0].TCNTB = 250000*num;

	//手动装载
	timers->TCON &= ~(0x0f);
	timers->TCON |= TIMER0_MANUP;

	//开启定时器0
	timers->TCON &= ~(0x0f);
	timers->TCON |= TIMER0_ON;
	
	//等待TCNTO 装载后再继续
	while(!(timers->ch[0].TCNTO));
	
	while(timers->ch[0].TCNTO);
}

/*
* 单位: 延时1 ms
*/
void mdelay(int num)
{
	S5PC11X_TIMERS *const timers = S5PC11X_GetBase_TIMERS();

	//设置预分频为16
	timers->TCFG0 &= ~(0xff);
	timers->TCFG0 |= 16;			

	//选择1/4  分频
	timers->TCFG1 &= ~(0x0f);
	timers->TCFG1 |= 0x02;

	//设置初值寄存器
	timers->ch[0].TCNTB = (get_PCLKP()/(17*4*1000))*num;

	//手动装载
	timers->TCON &= ~(0x0f);
	timers->TCON |= TIMER0_MANUP;

	//开启定时器0
	timers->TCON &= ~(0x0f);
	timers->TCON |= TIMER0_ON;
	
	//等待TCNTO 装载后再继续
	while(!(timers->ch[0].TCNTO));
	
	while(timers->ch[0].TCNTO);
}

#endif /*FUNC_TIMER_DELAY*/

#ifdef FUNC_TIMER_INT
/*
* 定时器的初始化
* 输入时钟源是pclk_PSYS = 66MHz
* 输出时钟源配置为:250KHz  (预分频值:65  ；分频值: 1/4)
* 配置定时器的手动载入初值并自动重新载入初值
* 开启定时器
*/
int timer_init(void)
{
	S5PC11X_TIMERS *const timers = S5PC11X_GetBase_TIMERS();

	//设置预分频为65
	timers->TCFG0 &= ~(0xff);
	timers->TCFG0 |= 65;		
	
	//选择1/4  分频
	timers->TCFG1 &= ~(0x0f<<(1*4));
	timers->TCFG1 |= (0x02<<(1*4));

	//设置初值寄存器
	timers->ch[1].TCNTB = 250000;
	
	//手动装载
	timers->TCON &= ~(0x0f<<8);
	timers->TCON |= TIMER1_MANUP;
	
	//启动定时器1，且手动装载
	timers->TCON &= ~(0x0f<<8);
	timers->TCON |= TIMER1_ATLOAD_ON|TIMER1_ON;

	//清除timer1 中断 挂起标志
	TINT_CSTAT = (0x01<<6);
	
	//timer1 中断使能
	TINT_CSTAT = (0x01<<1);
	
	return 0;
}

/*
* 定时器timer 1 停止计时
*/
void stop_timer(void)
{
	S5PC11X_TIMERS *const timers = S5PC11X_GetBase_TIMERS();
	timers->TCON &= ~(0x0f<<8);
}

/*
* 定时器timer 1 开始计时
*/
void start_timer(void)
{
	S5PC11X_TIMERS *const timers = S5PC11X_GetBase_TIMERS();

	timers->TCON &= ~(0x0f<<8);
	timers->TCON |= TIMER1_ATLOAD_ON|TIMER1_ON;
}

/*
* timer1 中断服务函数
*/
void interrupt_timer(void)
{
	//进入中断保护现场
	IRQ_PUSH

	//清中断向量地址
	__REG(VIC0ADDRESS) = 0x00;
	
	//清除timer1 中断 挂起标志
	TINT_CSTAT = (0x01<<6);

	//跑马灯
	led_marquee();

	//timer1 中断使能
	TINT_CSTAT = (0x01<<1);

	//退出中断恢复现场
	IRQ_POP
}

/*
* timer1 中断初始化
*/
void vic_init(void)
{
	//清中断向量地址
	__REG(VIC0ADDRESS)	= 0x00;	

	//绑定中断服务函数入口地址
	__REG(VIC0VECTADDR22) = (unsigned int)interrupt_timer;	

	//选择IRQ 中断	
	__REG(VIC0INTSELECT) &= 	~(0x01<<22);	
			
	//中断向量使能  	
	__REG(VIC0INTENABLE) |= 	(0x01<<22);	

	//使能IRQ  中断
	IRQ_ENABLE
}
#endif /*FUNC_TIMER_INT*/

#ifdef FUNC_PWM_OUT
/*
* 定时器timer 3停止计时
*/
void stop_timer(void)
{
	S5PC11X_TIMERS *const timers = S5PC11X_GetBase_TIMERS();
	timers->TCON &= ~(0x0f<<16);
}

/*
* 定时器timer 3开始计时
*/
void start_timer(void)
{
	S5PC11X_TIMERS *const timers = S5PC11X_GetBase_TIMERS();

	timers->TCON &= ~(0x0f<<16);
	timers->TCON |= TIMER3_ATLOAD_ON|TIMER3_ON;
}

/*
*pwm占空比增大
*/
void pwm_duty_add(void)
{
	S5PC11X_TIMERS *const timers = S5PC11X_GetBase_TIMERS();

	if(timers->ch[3].TCMPB < 900)
		timers->ch[3].TCMPB += 100;
}

/*
*pwm占空比减小
*/
void pwm_duty_sub(void)
{
	S5PC11X_TIMERS *const timers = S5PC11X_GetBase_TIMERS();

	if(timers->ch[3].TCMPB > 0)
		timers->ch[3].TCMPB -= 100;
}

/*
* 定时器3作为PWM输出
*/
void pwm3_out_init(void)
{
	S5PC11X_TIMERS *const timers = S5PC11X_GetBase_TIMERS();

	/*根据原理图可知背光灯是由pwm3控制,
	即GPD0[3]口,将GPG0[3]配置成PWM 输出*/
	__REG(GPD0CON) &= ~(0x0f<<(3*4));
	__REG(GPD0CON) |= (0x02<<(3*4));
	
	//设置预分频为65
	timers->TCFG0 &= ~(0xff<<8);
	timers->TCFG0 |= 65<<8;			

	//选择1/4  分频
	timers->TCFG1 &= ~(0x0f<<12);
	timers->TCFG1 |= (0x02<<12);

	/*设置PWM频率为1KHz = 1MHz / 1000*/
	timers->ch[3].TCNTB = 1000;

	/*设置初始占空比为50%*/
	timers->ch[3].TCMPB = 500;

	//手动装载
	timers->TCON &= ~(0x0f<<16);
	timers->TCON |= TIMER3_MANUP;

	//启动定时器3，且手动装载
	timers->TCON &= ~(0x0f<<16);
	timers->TCON |= TIMER3_ATLOAD_ON|TIMER3_ON;
}

#endif

