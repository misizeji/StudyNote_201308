#include "config.h"


#ifdef FUNC_ALL_KEY_INT

#define IRQ_PUSH __asm__(	\
	"ldr sp, =0xD0034000		\n"\
	"subs lr, lr, #4				\n"\
	"stmfd sp!, {r0-r12, lr}		\n"\
	);
#define IRQ_POP __asm__(		\
	"ldmfd sp!, {r0-r12,pc}^	\n"\
	);

//enable CPSR I-bit
#define IRQ_ENABLE __asm__ (\
	"mrs r0, cpsr				\n"\
	"bic  r0, r0, #0x80			\n"\
	"msr cpsr_c, r0			\n"\
	);


/*
*中断服务函数
*/
void interrupt_key(void)
{
	IRQ_PUSH;

	unsigned int pend_val = __REG(EXT_INT_2_PEND);

	//清除中断向量地址
	__REG(VIC0ADDRESS) = 0;

	//清除中断挂起标志
	__REG(EXT_INT_2_PEND) = 0xF8;	
	
	//判断具体按键
	find_key(pend_val);	

	IRQ_POP;
}


/*
*中断初始化函数
*/
void vic_init(void)
{
	//清除中断向量地址
	__REG(VIC0ADDRESS) = 0;

	//绑定中断服务函数入口地址		
	__REG(VIC0VECTADDR16) = (unsigned int)interrupt_key;

	//选择IRQ 中断(Int select)
	__REG(VIC0INTSELECT) &= ~(0x01<<16);	
	
	//中断向量使能(Int enable) 
	__REG(VIC0INTENABLE) |= (0x01<<16);

	IRQ_ENABLE
}

#endif /*FUNC_ALL_KEY_INT*/

