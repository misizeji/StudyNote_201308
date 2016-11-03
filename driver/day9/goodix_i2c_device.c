/*
 * Copyright (c) 2012 sunplusapp
 * this program is free software; you can redistribute it and/or modify it
 *	
 * this is a demo about how to create i2c_board_info dynamically; it just like 
 * i2c_register_device
 * 
 *
 * Date and Edition:		2012-11-18  v1.0
 * Date and Edition:		2013-01-05  v1.1
 * Author:				<feng.lv@sunplusapp.com>
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/i2c.h>
#include <linux/err.h>
#include <linux/slab.h>
#include <linux/irq.h>
#include "goodix_touch_v2.h"


static struct i2c_board_info dev_test_info = {	
	/*设置type 与addr*/
	I2C_BOARD_INFO("Goodix-TS",0x55),
};

static struct i2c_client *dev_test_client;

static int dev_test_dev_init(void)
{
	struct i2c_adapter *i2c_adap;
	printk("the device is initing ...\n");
	/*获取adapter并注册*/
	i2c_adap = i2c_get_adapter(0);
	dev_test_client = i2c_new_device(i2c_adap,&dev_test_info);
	i2c_put_adapter(i2c_adap);
	
	return 0;
}

static void dev_test_dev_exit(void)
{
	printk("the device is exited!!!\n");
	i2c_unregister_device(dev_test_client);
	return;
}

module_init(dev_test_dev_init);
module_exit(dev_test_dev_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("sunplusedu");


