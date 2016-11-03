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
* ��λ: ��ʱ1 s
*/
void delay(int num)
{	
	S5PC11X_TIMERS *const timers = S5PC11X_GetBase_TIMERS();
	
	//����Ԥ��ƵΪ65
	timers->TCFG0 &= ~(0xff);
	timers->TCFG0 |= 65;			

	//ѡ��1/4  ��Ƶ
	timers->TCFG1 &= ~(0x0f);
	timers->TCFG1 |= 0x02;

	//���ó�ֵ�Ĵ���
	timers->ch[0].TCNTB = 250000*num;

	//�ֶ�װ��
	timers->TCON &= ~(0x0f);
	timers->TCON |= TIMER0_MANUP;

	//������ʱ��0
	timers->TCON &= ~(0x0f);
	timers->TCON |= TIMER0_ON;
	
	//�ȴ�TCNTO װ�غ��ټ���
	while(!(timers->ch[0].TCNTO));
	
	while(timers->ch[0].TCNTO);
}

/*
* ��λ: ��ʱ1 ms
*/
void mdelay(int num)
{
	S5PC11X_TIMERS *const timers = S5PC11X_GetBase_TIMERS();

	//����Ԥ��ƵΪ16
	timers->TCFG0 &= ~(0xff);
	timers->TCFG0 |= 16;			

	//ѡ��1/4  ��Ƶ
	timers->TCFG1 &= ~(0x0f);
	timers->TCFG1 |= 0x02;

	//���ó�ֵ�Ĵ���
	timers->ch[0].TCNTB = (get_PCLKP()/(17*4*1000))*num;

	//�ֶ�װ��
	timers->TCON &= ~(0x0f);
	timers->TCON |= TIMER0_MANUP;

	//������ʱ��0
	timers->TCON &= ~(0x0f);
	timers->TCON |= TIMER0_ON;
	
	//�ȴ�TCNTO װ�غ��ټ���
	while(!(timers->ch[0].TCNTO));
	
	while(timers->ch[0].TCNTO);
}

#endif /*FUNC_TIMER_DELAY*/

#ifdef FUNC_TIMER_INT
/*
* ��ʱ���ĳ�ʼ��
* ����ʱ��Դ��pclk_PSYS = 66MHz
* ���ʱ��Դ����Ϊ:250KHz  (Ԥ��Ƶֵ:65  ����Ƶֵ: 1/4)
* ���ö�ʱ�����ֶ������ֵ���Զ����������ֵ
* ������ʱ��
*/
int timer_init(void)
{
	S5PC11X_TIMERS *const timers = S5PC11X_GetBase_TIMERS();

	//����Ԥ��ƵΪ65
	timers->TCFG0 &= ~(0xff);
	timers->TCFG0 |= 65;		
	
	//ѡ��1/4  ��Ƶ
	timers->TCFG1 &= ~(0x0f<<(1*4));
	timers->TCFG1 |= (0x02<<(1*4));

	//���ó�ֵ�Ĵ���
	timers->ch[1].TCNTB = 250000;
	
	//�ֶ�װ��
	timers->TCON &= ~(0x0f<<8);
	timers->TCON |= TIMER1_MANUP;
	
	//������ʱ��1�����ֶ�װ��
	timers->TCON &= ~(0x0f<<8);
	timers->TCON |= TIMER1_ATLOAD_ON|TIMER1_ON;

	//���timer1 �ж� �����־
	TINT_CSTAT = (0x01<<6);
	
	//timer1 �ж�ʹ��
	TINT_CSTAT = (0x01<<1);
	
	return 0;
}

/*
* ��ʱ��timer 1 ֹͣ��ʱ
*/
void stop_timer(void)
{
	S5PC11X_TIMERS *const timers = S5PC11X_GetBase_TIMERS();
	timers->TCON &= ~(0x0f<<8);
}

/*
* ��ʱ��timer 1 ��ʼ��ʱ
*/
void start_timer(void)
{
	S5PC11X_TIMERS *const timers = S5PC11X_GetBase_TIMERS();

	timers->TCON &= ~(0x0f<<8);
	timers->TCON |= TIMER1_ATLOAD_ON|TIMER1_ON;
}

/*
* timer1 �жϷ�����
*/
void interrupt_timer(void)
{
	//�����жϱ����ֳ�
	IRQ_PUSH

	//���ж�������ַ
	__REG(VIC0ADDRESS) = 0x00;
	
	//���timer1 �ж� �����־
	TINT_CSTAT = (0x01<<6);

	//�����
	led_marquee();

	//timer1 �ж�ʹ��
	TINT_CSTAT = (0x01<<1);

	//�˳��жϻָ��ֳ�
	IRQ_POP
}

/*
* timer1 �жϳ�ʼ��
*/
void vic_init(void)
{
	//���ж�������ַ
	__REG(VIC0ADDRESS)	= 0x00;	

	//���жϷ�������ڵ�ַ
	__REG(VIC0VECTADDR22) = (unsigned int)interrupt_timer;	

	//ѡ��IRQ �ж�	
	__REG(VIC0INTSELECT) &= 	~(0x01<<22);	
			
	//�ж�����ʹ��  	
	__REG(VIC0INTENABLE) |= 	(0x01<<22);	

	//ʹ��IRQ  �ж�
	IRQ_ENABLE
}
#endif /*FUNC_TIMER_INT*/

#ifdef FUNC_PWM_OUT
/*
* ��ʱ��timer 3ֹͣ��ʱ
*/
void stop_timer(void)
{
	S5PC11X_TIMERS *const timers = S5PC11X_GetBase_TIMERS();
	timers->TCON &= ~(0x0f<<16);
}

/*
* ��ʱ��timer 3��ʼ��ʱ
*/
void start_timer(void)
{
	S5PC11X_TIMERS *const timers = S5PC11X_GetBase_TIMERS();

	timers->TCON &= ~(0x0f<<16);
	timers->TCON |= TIMER3_ATLOAD_ON|TIMER3_ON;
}

/*
*pwmռ�ձ�����
*/
void pwm_duty_add(void)
{
	S5PC11X_TIMERS *const timers = S5PC11X_GetBase_TIMERS();

	if(timers->ch[3].TCMPB < 900)
		timers->ch[3].TCMPB += 100;
}

/*
*pwmռ�ձȼ�С
*/
void pwm_duty_sub(void)
{
	S5PC11X_TIMERS *const timers = S5PC11X_GetBase_TIMERS();

	if(timers->ch[3].TCMPB > 0)
		timers->ch[3].TCMPB -= 100;
}

/*
* ��ʱ��3��ΪPWM���
*/
void pwm3_out_init(void)
{
	S5PC11X_TIMERS *const timers = S5PC11X_GetBase_TIMERS();

	/*����ԭ��ͼ��֪���������pwm3����,
	��GPD0[3]��,��GPG0[3]���ó�PWM ���*/
	__REG(GPD0CON) &= ~(0x0f<<(3*4));
	__REG(GPD0CON) |= (0x02<<(3*4));
	
	//����Ԥ��ƵΪ65
	timers->TCFG0 &= ~(0xff<<8);
	timers->TCFG0 |= 65<<8;			

	//ѡ��1/4  ��Ƶ
	timers->TCFG1 &= ~(0x0f<<12);
	timers->TCFG1 |= (0x02<<12);

	/*����PWMƵ��Ϊ1KHz = 1MHz / 1000*/
	timers->ch[3].TCNTB = 1000;

	/*���ó�ʼռ�ձ�Ϊ50%*/
	timers->ch[3].TCMPB = 500;

	//�ֶ�װ��
	timers->TCON &= ~(0x0f<<16);
	timers->TCON |= TIMER3_MANUP;

	//������ʱ��3�����ֶ�װ��
	timers->TCON &= ~(0x0f<<16);
	timers->TCON |= TIMER3_ATLOAD_ON|TIMER3_ON;
}

#endif

