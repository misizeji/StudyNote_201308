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
*�жϷ�����
*/
void interrupt_key(void)
{
	IRQ_PUSH;

	unsigned int pend_val = __REG(EXT_INT_2_PEND);

	//����ж�������ַ
	__REG(VIC0ADDRESS) = 0;

	//����жϹ����־
	__REG(EXT_INT_2_PEND) = 0xF8;	
	
	//�жϾ��尴��
	find_key(pend_val);	

	IRQ_POP;
}


/*
*�жϳ�ʼ������
*/
void vic_init(void)
{
	//����ж�������ַ
	__REG(VIC0ADDRESS) = 0;

	//���жϷ�������ڵ�ַ		
	__REG(VIC0VECTADDR16) = (unsigned int)interrupt_key;

	//ѡ��IRQ �ж�(Int select)
	__REG(VIC0INTSELECT) &= ~(0x01<<16);	
	
	//�ж�����ʹ��(Int enable) 
	__REG(VIC0INTENABLE) |= (0x01<<16);

	IRQ_ENABLE
}

#endif /*FUNC_ALL_KEY_INT*/

