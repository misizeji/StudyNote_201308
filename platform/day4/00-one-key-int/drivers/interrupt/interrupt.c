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

	/*���ж�������ַ*/
	__REG(VIC0ADDRESS) = 0;
	
	/*����ұ�־*/
	__REG(EXT_INT_2_PEND)  = 0x01<<3;	
	
	printf("\n\t to IRQ: %d\n", irq_count++);

	/*���ó�����*/
	__REG(GPH2CON) &= ~(0xF<<(3*4));
	/*�ȴ�����̧��*/
	while((__REG(GPH2DAT)&0x08) != 0x08);
	/*���ó��ж�*/
	__REG(GPH2CON) |= 0xF<<(3*4);
	
	IRQ_POP
}

void vic_init(void)
{
	//���ж�������ַ
	__REG(VIC0ADDRESS) = 0;
	
	//���жϷ�������ڵ�ַ
	__REG(VIC0VECTADDR16) = (unsigned int)interrupt_key;
	
	//ѡ��IRQ �ж�(����ע�͵���Ĭ��IRQ)
	__REG(VIC0INTSELECT) &= ~(0x01<<16);
	
	//�ж�����ʹ��(����16���ж�Դ)
	__REG(VIC0INTENABLE) |= (0x01<<16);

	IRQ_ENABLE
}