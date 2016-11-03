#include "uart.h"
#include "led.h"

int main (void)
{
	printf("\ns5pv210: test the 00-gpio-led ......\n");
	
	led_init();
	
	while(1)
	{
		led_test();
		
		//ctrl+CÍË³ö
		if(_getc_int() == 0x03)
			break;
	}
	return 0;
}

