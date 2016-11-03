#include "uart.h"
#include "pwm.h"


int main (void)
{	
	printf("\ns5pv210: test the 02-pwm-out ......\n");

	pwm3_out_init();

	while(1)
	{
		char ch = _getc_int();

		switch(ch)
		{
			//ctrl+C 退出
			case 0x03:
				stop_timer();
				return 0;
			//t 停止闪烁
			case 't':
				stop_timer();
				break;
			//k 继续闪烁
			case 'k':
				start_timer();
				break;	
			//DOWN占空比减小	
			case 0x42:
				pwm_duty_sub();
				break;	
			//UP 占空比增加
			case 0x41:
				pwm_duty_add();
				break;				
			default:
				break;
		} 
	}
	return 0;
}


