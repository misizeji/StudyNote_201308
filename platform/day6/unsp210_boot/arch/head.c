
#include <s5pc110.h>
#include <smdkv210single.h>
extern void disable_watch_dog (void);
extern void init_system_clock (void);
extern void inituart (void);
extern void mem_setup (void);
extern void nand_init (void);
extern int copy_uboot_to_ram (void);
extern void main (void);

register volatile unsigned int init_sp asm ("r13");//定义变量代表r13:堆栈指针
register volatile unsigned int jmp_pc asm ("r15");
 
unsigned int temp_val __attribute__((__section__(".boot_data"))) = 0x2000;//GNU C 的用法

void _start(void)
{
	init_sp = 0xd0036000;
	disable_watch_dog();
	init_system_clock();
	inituart();
	//asm("bl mem_ctrl_asm_init\n");
	mem_setup();
	init_sp = CFG_UBOOT_BASE-12;
	nand_init();
	copy_uboot_to_ram();

	jmp_pc = (unsigned int)main;
}



