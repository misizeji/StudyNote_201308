#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "s5pv210.h"
#include "uart.h"
#include "asm_test.h"
#include "led.h"
#include "key.h"
#include "timer.h"
#include "interrupt.h"
#include "nand.h"
#include "speed.h"


/*打开对应的宏即可测试对应的功能*/
#define FUNC_TEST_ASM			//内嵌汇编测试

//#define FUNC_NAND_FLASH		//nand flash读写	

//#define FUNC_LED_CONTROL		//指示灯控制

//#define FUNC_ONE_KEY_POLL		//单独按键扫描

//#define FUNC_ALL_KEY_POLL		//扫描矩阵按键

//#define FUNC_TIMER_DELAY		//定时器延时	

//#define FUNC_PWM_OUT			//PWM控制背光

//#define FUNC_ALL_KEY_INT		//中断矩阵按键

//#define FUNC_TIMER_INT			//定时器中断		


#endif /*_CONFIG_H_*/

