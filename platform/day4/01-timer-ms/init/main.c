#include "uart.h"
#include "timer.h"
#include "led.h"

int main (void)
{	
	printf("\ns5pv210: test the 01-timer-ms-s ......\n");

	led_init();
	
	while(1)
	{
		led_marquee();
		mdelay(1000);
	
		//ctrl+CÍË³ö
		if(_getc_int() == 0x03)
			break;
	}
	return 0;
}


