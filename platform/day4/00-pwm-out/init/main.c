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
			//ctrl+C �˳�
			case 0x03:
				stop_timer();
				return 0;
			//t ֹͣ��˸
			case 't':
				stop_timer();
				break;
			//k ������˸
			case 'k':
				start_timer();
				break;	
			//DOWNռ�ձȼ�С	
			case 0x42:
				pwm_duty_sub();
				break;	
			//UP ռ�ձ�����
			case 0x41:
				pwm_duty_add();
				break;				
			default:
				break;
		} 
	}
	return 0;
}


