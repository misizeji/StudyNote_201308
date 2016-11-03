#include "uart.h"
#include "s5pv210.h"


#define IRQ_PUSH __asm__(	\
	"ldr sp, =0xD0034000		\n"\
	"subs lr, lr, #4				\n"\
	"stmfd sp!, {r0-r12, lr}		\n"\
	);
#define IRQ_POP __asm__(		\
	"ldmfd sp!, {r0-r12,pc}^	\n"\
	);

/*enable CPSR I-bit*/
#define IRQ_ENABLE __asm__ (\
	"mrs r0, cpsr				\n"\
	"bic  r0, r0, #0x80			\n"\
	"msr cpsr_c, r0			\n"\
	);

void interrupt_key(void)
{
	IRQ_PUSH
		
	static int irq_count = 1;

	/*清中断向量地址*/
	__REG(VIC0ADDRESS) = 0;
	
	/*清除挂标志*/
	__REG(EXT_INT_2_PEND)  = 0x01<<3|0x01<<4|0x01<<5|0x01<<6|0x01<<7;	
	
	printf("\n\t to IRQ: %d\n", irq_count++);

	/*设置成输入*/
	__REG(GPH2CON) &= ~(0xF<<(3*4)|0xF<<(4*4)|0xF<<(5*4)|0xF<<(6*4)|0xF<<(7*4));

	/*等待按键抬起*/
	while((__REG(GPH2DAT)&(0x08)) != (0x08));//key 2
	while((__REG(GPH2DAT)&(0x10)) != (0x10));//key 3
	while((__REG(GPH2DAT)&(0x20)) != (0x20));//key 4
	while((__REG(GPH2DAT)&(0x40)) != (0x40));//key 5
	while((__REG(GPH2DAT)&(0x80)) != (0x80));//key 6
	/*设置成中断*/
	__REG(GPH2CON) |= (0xF<<(3*4)|0xF<<(4*4)|0xF<<(5*4)|0xF<<(6*4)|0xF<<(7*4));
	
	IRQ_POP
}

void vic_init(void)
{
	//清中断向量地址
	__REG(VIC0ADDRESS) = 0;
	
	//绑定中断服务函数入口地址
	__REG(VIC0VECTADDR16) = (unsigned int)interrupt_key;
	
	//选择IRQ 中断(可以注释掉，默认IRQ)
	__REG(VIC0INTSELECT) &= ~(0x01<<16);
	
	//中断向量使能(共享16号中断源)
	__REG(VIC0INTENABLE) |= (0x01<<16);

	IRQ_ENABLE
}