#include "uart.h"

//将ctoa=2传入汇编加一后传出给C
void get_pc(void)
{
	unsigned int atoc_pc;
	unsigned int atoc_val;
	unsigned int ctoa = 2;

	__asm__ __volatile__(
	"mov r3, %[a_val] 	\n"        
	"add r3, r3, #0x01 	\n"        
	"mov %[outval], r3 	\n"    
	"mov %[outpc], pc 	\n"        
	:[outpc] "=r" (atoc_pc),[outval] "=r" (atoc_val)
	:[a_val] "r" (ctoa)
	:"r3"
	);
   
	printf("\natoc_pc = 0x%x, atoc_val = 0x%x\n", atoc_pc, atoc_val);
}

//求两个数的最大公约数
void get_gcd(int a, int b)	/*Greatest common divisor*/
{
#if 0			/*36,27*/
	while(a != b){      
		if (a > b)
			a = a - b;
		else
	       	b = b - a;
	}
#else
	__asm__ __volatile__(
	"mov r0, %[a_val] 	\n"        
	"mov r1, %[b_val] 	\n"        
	"1: 				\n"        
	"cmp r0, r1 		\n"        
	"beq 3f 			\n"   
	"bge 2f 			\n"   
	"sub r1, r1, r0 	\n"   
	"b 1b 				\n"   
	"2: 				\n"    
	"sub r0, r0, r1 	\n"        
	"b 1b 				\n"        
	"3: 				\n"
	"mov %[out_a], r0 	\n"
	:[out_a] "=r" (a)
	:[a_val] "r" (a),[b_val] "r" (b)
	:"r0","r1"
	);
#endif
    	printf("\n两个数的最大公约数是: %d\n", a);
}

//课后练习:加法运算
void get_add(void)
{
	int i = 0;
	int r2 = 0;
	int r3 = 100;
#if 0
	for(i=0;i<10;i++)
		r2 = r2 + r3;
	
	printf("\ns5pv210: C language the result of add is %d ......\n", r2);
#else

	__asm__ __volatile__(
	"mov r2, %[a_val]	\n"
	"mov r3, %[b_val]	\n"
	"mov r4, %[i_val]	\n"
	"1:					\n"
	"cmp r4, #10		\n"
	"beq 3f				\n"
	"bne 2f				\n"
	"2:					\n"
	"add r2, r2, r3		\n"
	"add r4, r4, #1		\n"
	"b 1b				\n"
	"3:					\n"
	"mov %[out_a], r2	\n"
	"mov %[out_i], r4	\n"
	:[out_a] "=r" (r2), [out_i] "=r" (i)
	:[a_val] "r" (r2), [b_val] "r" (r3), [i_val] "r" (i)
	:"r2","r3","r4"
	);
	
	printf("\nr2 = %d; r3 = %d i = %d",r2,r3,i);
#endif
}
