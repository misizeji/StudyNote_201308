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
#if 0
	int i = 0;
	int r2 = 0;
	int r3 = 100;

	for(i=0;i<10;i++)
		r2 = r2 + r3;
	
	printf("\ns5pv210: C language the result of add is %d ......\n", r2);
#else
	int c_i = 0;
	int c_r2 = 0;
	int c_r3 = 100;
	
	__asm__ __volatile__(
	"mov r1, %[r1_val] 	\n"
	"mov r2, %[r2_val] 	\n"
	"mov r3, %[r3_val] 	\n"
	"LOOP: 				\n"
	"cmp r1, #10 		\n"
	"addne r2, r2, r3 	\n"
	"addne r1, r1, #1	\n"
	"bne LOOP 			\n"
	"mov %[r2_ret], r2	\n"
	:[r2_ret] "=r" (c_r2)
	:[r1_val] "r" (c_i),[r2_val] "r" (c_r2),[r3_val] "r" (c_r3)
	:"r1","r2","r3"
	);
	  
	printf("\ns5pv210: Assembly language the result of add is %d ......\n", c_r2);
#endif
}
