#include "uart.h"
#include "asm_test.h"

int main (void)
{	
	printf("\ns5pv210: test the 00-test-asm  ......\n");

	get_pc();
	get_gcd(36,27);
	get_add();
	
	while(1)
	{	
		//ctrl+C  ÍË³ö	
		if(_getc_int() == 0x03)	
			break;
	}
	return 0;
}

