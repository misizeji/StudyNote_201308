#include "config.h"

int main (void)
{
#ifdef FUNC_TEST_ASM
	printf("\ns5pv210: test the FUNC_TEST_ASM ......\n");

	get_pc();	
	get_gcd(36,27);	
	get_add();	

	while(1)
	{	
		//ctrl+C  退出	
		if(_getc_int() == 0x03)	
			break;
	}
#endif /*FUNC_TEST_ASM*/

#ifdef FUNC_LED_CONTROL
	printf("\ns5pv210: test the FUNC_LED_CONTROL ......\n");
	led_init();
	
	while(1)
	{
		led_test();
		
		//ctrl+C退出
		if(_getc_int() == 0x03)
			break;
	}
#endif /*FUNC_LED_CONTROL*/

#ifdef FUNC_ONE_KEY_POLL
	const int led_on = 0x0f; 

	printf("\ns5pv210: test the FUNC_ONE_KEY_POLL ......\n");	
	
	led_init();
	key_init();

	/*
	*  轮询扫描key2对应引脚的状态
	*  如果该引脚的电平状态由1-0；则点亮led
	*/
	while(1)
	{
		unsigned int temp = __REG(GPH2DAT)&0xff;
		
		if((temp & 0x08) == 0x08)
			__REG(GPH0DAT) &= ~led_on;	
		else if((temp & 0x08) == 0x00)
			__REG(GPH0DAT) |= led_on;

		//ctrl+C退出
		if(_getc_int() == 0x03)
			break;
	}
#endif /*FUNC_ONE_KEY_POLL*/

#ifdef FUNC_ALL_KEY_POLL
	int row_val = 0;
	int col_val = 0;
	int judge_val = 0;
		
	printf("\ns5pv210: test the FUNC_ALL_KEY_POLL ......\n");
	
	led_init();
	
	/*
	* 循环扫描
	*/
	while(1)
	{
		row_scan_init();
		row_val = key_read_row();

		col_scan_init();
		col_val = key_read_col();

		judge_val = row_val + col_val;

		key_deal(judge_val);
		soft_delay(3000);

		//ctrl+C退出
		if(_getc_int() == 0x03)
			break;
	}
#endif /*FUNC_ALL_KEY_POLL*/

#ifdef FUNC_TIMER_DELAY
	printf("\ns5pv210: test the FUNC_TIMER_DELAY ......\n");

	led_init();

	while(1)
	{
		led_marquee();
		//mdelay(1000);
		delay(1);
		
		//ctrl+C退出
		if(_getc_int() == 0x03)
			break;
	}
#endif /*FUNC_TIMER_DELAY*/

#ifdef FUNC_PWM_OUT
	printf("\ns5pv210: test the FUNC_PWM_OUT ......\n");

	pwm3_out_init();

	while(1)
	{
		char ch = _getc_int();

		switch(ch)
		{
			/*ctrl+C 退出*/
			case 0x03:
				stop_timer();
				return 0;
			/*t 停止闪烁*/
			case 't':
				stop_timer();
				break;
			/*k 继续闪烁*/
			case 'k':
				start_timer();
				break;	
			/*DOWN占空比减小*/	
			case 0x42:
				pwm_duty_sub();
				break;	
			/*UP 占空比增加*/	
			case 0x41:
				pwm_duty_add();
				break;					
			default:
				break;
		} 
	}
#endif

#ifdef FUNC_TIMER_INT
	printf("\ns5pv210: test the FUNC_TIMER_INT ......\n");

	timer_init();
	vic_init();
	led_init();
	
	while(1)
	{	
		char ch = _getc_int();
		
		switch(ch)
		{
			/*ctrl+C 退出*/
			case 0x03:
				stop_timer();
				return 0;
			case 't':
				stop_timer();
				break;
			case 'k':
				start_timer();
				break;
			default:
				break;
		}
	}
#endif /*FUNC_TIMER_INT*/

#ifdef FUNC_ALL_KEY_INT
	printf("\ns5pv210: test the FUNC_ALL_KEY_INT ......\n");
	
	key_init();
	vic_init();
	led_init();

	while(1)
	{	
		//ctrl+C退出
		if(_getc_int() == 0x03)
			break;
	}
#endif /*FUNC_ALL_KEY_INT*/

#ifdef FUNC_NAND_FLASH

	#define TEST_FLASH_ADDR	0x9000000
	#define TEST_FLASH_LEN		2048

	unsigned int i;
	unsigned char str[TEST_FLASH_LEN];
	
	printf("\ns5pv210: test the FUNC_NAND_FLASH ......\n");

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
		if(_getc_int() == 0x03)//ctrl+C退出
			break;
	}
#endif /*FUNC_NAND_FLASH*/

	return 0;	
}


