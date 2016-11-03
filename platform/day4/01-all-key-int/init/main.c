#include "uart.h"
#include "interrupt.h"
#include "key.h"
#include "led.h"

int main (void)
{	
	printf("\ns5pv210: test the 00-one-key-int ......\n");
	
	key_init();
	vic_init();	
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

