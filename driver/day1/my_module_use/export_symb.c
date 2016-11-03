/*
 * Copyright  2011-2012  sunplusapp
 */
 
/*
 * this program is free software; you can redistribute it and/or modify
 * it
 * 
 * Date and Edition:	2012-12-27  v1.2
 * Author:				dxh/zsd/Valor-Lion/lv/dw
 */
 
#include <linux/module.h>    /*module_init()*/
#include <linux/kernel.h>	 /* printk() */
#include <linux/init.h>		 /* __init __exit */

#define DEBUG		//open debug message

#ifdef DEBUG
#define debug(fmt, arg...)		printk(KERN_WARNING fmt, ##arg)
#else
#define debug(fmt, arg...)		printk(KERN_DEBUG fmt, ##arg)
#endif

                         
int add_test(int a ,int b)                                
{                                
	return a + b;                             
} 
                               
int sub_test(int a,int b)                                
{                                
	return a - b;                             
}                            

EXPORT_SYMBOL(add_test);
EXPORT_SYMBOL(sub_test);

MODULE_AUTHOR("sunplsuedu");
MODULE_LICENSE("GPL");