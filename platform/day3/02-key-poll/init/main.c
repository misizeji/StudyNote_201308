#include "s5pv210.h"
#include "uart.h"
#include "led.h"
#include "key.h"

int main (void)
{
	printf("\ns5pv210: test the 01-one-key-poll ......\n");
	
	led_init();
	key_init();

	while(1)
	{
		key_deal();

		//ctrl+CÍË³ö
		if(_getc_int() == 0x03)
			break;
	}
	return 0;
}


