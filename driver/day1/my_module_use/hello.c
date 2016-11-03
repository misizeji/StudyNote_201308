
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

#define DEBUG	//open debug message

#ifdef DEBUG
#define debug(fmt, arg...)		printk(KERN_WARNING fmt, ##arg)
#else
#define debug(fmt, arg...)		printk(KERN_DEBUG fmt, ##arg)
#endif

extern int add_test(int a ,int b); 
extern int sub_test(int a,int b); 

static int __init hello_init(void)
{
	int a,b;
  
	a = add_test(10,20);
	b = sub_test(30,20);
	debug("the add test result is %d\n",a);
	debug("the sub test result is %d\n",b);
  
	return 0;
}

static void __exit hello_exit(void)
{
	debug(" Hello exit\n ");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_AUTHOR("sunplsuedu");
MODULE_LICENSE("GPL");
