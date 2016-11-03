#include "uart.h"
#include "nand.h"


#define TEST_FLASH_ADDR	0x9000000
#define TEST_FLASH_LEN		2048

int main (void)
{
	unsigned int i;
	unsigned char str[TEST_FLASH_LEN];
	
	printf("\ns5pv210: test 00-nand-flash ......\n");

	printf("\nnand ID:%#x\n\n",_nand_read_id());
	
	_nand_erase(TEST_FLASH_ADDR, TEST_FLASH_LEN);
	_nand_read(str, TEST_FLASH_ADDR, TEST_FLASH_LEN);
	for(i=0; i<TEST_FLASH_LEN; i++)
		printf("%#x ", str[i]);

	printf("\n\nprint over after erase\n\n");
		
	for(i=0; i<TEST_FLASH_LEN; i++)
		str[i] = 0x5a;
	_nand_write(str, TEST_FLASH_ADDR, TEST_FLASH_LEN);
	
	_nand_read(str, TEST_FLASH_ADDR, TEST_FLASH_LEN);
	for(i=0; i<TEST_FLASH_LEN; i++)
		printf("%#x ", str[i]);
	
	printf("\n\nprint over after write, press Ctrl+C end the test !\n");
	
	while(1)
	{
		//ctrl+CÍË³ö
		if(_getc_int() == 0x03)
			break;
	}
	return 0;
}


