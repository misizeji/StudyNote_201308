/* drivers/input/touchscreen/goodix_touch.c
 *
 * Copyright (C) 2010 Goodix, Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/time.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/earlysuspend.h>
#include <linux/hrtimer.h>
#include <linux/i2c.h>
#include <linux/input.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <mach/gpio.h>
#include <plat/gpio-cfg.h>
#include <linux/irq.h>
#include "goodix_touch_v2.h"



#define debug_goodix(fmt, arg...)  printk(KERN_INFO fmt, ##arg);

/* other cpt touch can use this workqueue */
struct workqueue_struct *goodix_wq;

static const struct i2c_device_id goodix_ts_id[] = {
	{ GOODIX_I2C_NAME, 0 },
	{ }
};


int i2c_write_bytes(struct i2c_client *client,uint8_t *data,int len)
{
	/* just one i2c_msg OK. */
	struct i2c_msg goodix_msg[1];	
	goodix_msg[0].addr  = client->addr;		/*从设备地址*/
	goodix_msg[0].flags = !I2C_M_RD;	    /*读写标记位*/
	goodix_msg[0].buf   = data;			    /*寄存器地址*/
	goodix_msg[0].len   = len;			    /*buf的大小*/

	i2c_transfer(client->adapter, goodix_msg, 1);
	//printk("%s,%d\n",__func__,__LINE__);
	return 0;
}

int i2c_read_bytes(struct i2c_client *client, uint8_t *data, int len)
{
	/* u need two i2c_msg */
	struct i2c_msg goodix_msg[2];	/*读需要两个结构体*/ 
	goodix_msg[0].addr  = client->addr;		/*从设备地址*/
	goodix_msg[0].flags = !I2C_M_RD;	    /*读写标记位*/
	goodix_msg[0].buf   = &data[0];			    /*寄存器地址*/
	goodix_msg[0].len   = 1;			    /*buf的大小*/

	goodix_msg[1].addr  = client->addr;		/*从设备地址*/
	goodix_msg[1].flags = I2C_M_RD;		    /*读写标志位*/
	goodix_msg[1].buf   = &data[1];				/*读到的数据放在此处*/
	goodix_msg[1].len   = len-1;		    	/*要读数据的大小*/

	i2c_transfer(client->adapter, goodix_msg, 2);
	//printk("%s,%d\n",__func__,__LINE__);
	return 0;
}


static int goodix_read_panel(struct goodix_priv *goodix, uint8_t *data, int size)
{
	return i2c_read_bytes(goodix->cli, data, size);
}

static int goodix_init_panel(struct goodix_priv *goodix)
{

	uint8_t config_info[54]={0x30,	0x19,0x05,0x05,0x28,0x02,0x14,0x14,0x10,0x32,0xF8,0x14,0x00,0x1E,0x00,0xED,0xCB,
									0xA9,0x87,0x65,0x43,0x21,0x01,0x00,0x00,0x35,0x2E,0x4D,0xC0,0x20,0x01,0x01,0x83,
									0x50,0x3C,0x1E,0x28,0x00,0x33,0x2C,0x01,0xEC,0x00,0x3C,0x00,0x00,0x00,0x00,0x00,
									0x00,0x00,0x00,0x00,0x01};
	return i2c_write_bytes(goodix->cli, config_info, sizeof(config_info));

}

static void goodix_ts_work_func(struct work_struct *work)
{	
	uint8_t  point_data[35]={ 0 };
	int ret = 0;
	int pos[MAX_FINGER_NUM][2];
	unsigned int pressure[MAX_FINGER_NUM];	

	struct goodix_priv *goodix = container_of(work, struct goodix_priv, work);
	
	ret = goodix_read_panel(goodix, point_data, sizeof(point_data));
	ret = goodix_analysis_data(point_data, goodix, pos, pressure);

	if((800-pos[0][0]>0)&&(pos[0][1]>0))
	{
		input_report_abs(goodix->input_dev, ABS_X, (800-pos[0][0]));
		input_report_abs(goodix->input_dev, ABS_Y, pos[0][1]);	
    	input_sync(goodix->input_dev);
		//printk("%d %d\n",(800-pos[0][0]),pos[0][1]);
	}
	//printk("%s,%d %d\n",__func__,800-pos[0][0],pos[0][1]);
	return ;

}

static irqreturn_t goodix_ts_irq_handler(int irq, void *dev_id)
{
	struct goodix_priv *ts = dev_id;
	//printk(KERN_CRIT "interrupt for guitar arrived.\n");	
	queue_work(goodix_wq, &ts->work);
	//printk("%s,%d\n",__func__,__LINE__);
	return IRQ_HANDLED;
}

static int goodix_ts_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	int ret;
	struct goodix_priv *goodix = kzalloc(sizeof(struct goodix_priv), GFP_KERNEL);
	//printk("%s,%d\n",__func__,__LINE__);
	
	ret = gpio_request(SHUTDOWN_PORT, "GOODIX_TS SHUTDOWN PORT");
	if (ret < 0)  {
		dev_err(&client->dev, "Failed to request GPIO:%d, ERRNO:%d\n",(int)SHUTDOWN_PORT,ret);
		goto err_check_functionality_failed;
	}
	
	/* set gpio output & 0; goodix step into normal mode */
	gpio_direction_output(SHUTDOWN_PORT, 0); 


	ret = i2c_check_functionality(client->adapter, I2C_FUNC_I2C);
	if (ret == 0) {
		dev_err(&client->dev, "Soc no I2C function, ERRNO:%d\n", ret);
		goto err_check_functionality_failed;
	}

	msleep(16);//step into normal mode, need 16ms
	
	dev_info(&client->dev, "chip goodix work...\t i2c connecttion work...\t now we request irq and input_dev\n");	

	/*init goodix variable*/
	i2c_set_clientdata(client,goodix);
	goodix->cli = client; 
	goodix->input_dev = NULL;
	goodix->private_data = NULL;

	/* init work of workqueue*/
	INIT_WORK(&goodix->work, goodix_ts_work_func);
	
	/* input dev init and register */
	goodix->input_dev = input_allocate_device();
	
	goodix->input_dev->evbit[0] = BIT_MASK(EV_SYN) | BIT_MASK(EV_KEY) | BIT_MASK(EV_ABS) ;
	goodix->input_dev->keybit[BIT_WORD(BTN_TOUCH)] = BIT_MASK(BTN_TOUCH);
	goodix->input_dev->absbit[0] = BIT(ABS_X) | BIT(ABS_Y); // for android

	input_set_abs_params(goodix->input_dev, ABS_X, 0, SCREEN_MAX_HEIGHT, 0, 0);
	input_set_abs_params(goodix->input_dev, ABS_Y, 0, SCREEN_MAX_WIDTH, 0, 0);
	input_set_abs_params(goodix->input_dev, ABS_PRESSURE, 0 , 255, 0, 0);
	
	ret = input_register_device(goodix->input_dev);
	if (ret) {
		printk(KERN_ERR "Failed to register device\n");
		goto err_free_dev;
	}
	
	/* request_irq */
	ret = set_irq_type(INT_FUNC,IRQ_TYPE_EDGE_FALLING);	
	if(ret){		
		printk("IRQ_EINT4 set irq type failed");		
		goto err_free_dev;	
	}
	
	ret= request_irq(INT_FUNC, goodix_ts_irq_handler, 0, "touch_screen", (void *)goodix);
	if (ret){
		printk("request error\n");
		goto err_free_irq;
	}
	
	/* init hardware*/
	goodix_init_panel(goodix);
	//printk("%s,%d\n",__func__,__LINE__);

	return 0;
	
err_free_irq:
	free_irq(INT_FUNC, (void *)goodix);
err_free_dev:
	input_free_device(goodix->input_dev);/*这个函数用于错误处理*/	
err_check_functionality_failed:	
	return ret;

}

static int goodix_ts_remove(struct i2c_client *client)
{
	struct goodix_priv *goodix = i2c_get_clientdata(client);
//	printk("%s,%d\n",__func__,__LINE__);
	free_irq(INT_FUNC, (void *)goodix);
//	printk("%s,%d\n",__func__,__LINE__);
	gpio_free(SHUTDOWN_PORT);
//	printk("%s,%d\n",__func__,__LINE__);
    input_unregister_device(goodix->input_dev);
//	printk("%s,%d\n",__func__,__LINE__);
	input_free_device(goodix->input_dev);
	kfree(goodix);
//	printk("%s,%d\n",__func__,__LINE__);
	return 0;
}

static struct i2c_driver goodix_ts_driver = {
	.probe		= goodix_ts_probe,
	.remove		= goodix_ts_remove,
	.id_table	= goodix_ts_id,
	.driver = {
		.name	= GOODIX_I2C_NAME,
		.owner = THIS_MODULE,
	},
};

static int __devinit cpt_ts_init(void)
{
	int ret;
	printk(KERN_INFO "Touchscreen driver of guitar is installing...\n");
	goodix_wq = create_workqueue("goodix_wq");//success means not null
	if (!goodix_wq) {
		printk(KERN_ALERT "creat workqueue faiked\n");
		return -ENOMEM;
	}
	ret=i2c_add_driver(&goodix_ts_driver);
	return ret; 		
}

static void __devexit cpt_ts_exit(void)
{
	printk(KERN_INFO "Touchscreen driver of guitar is exiting...\n");
	i2c_del_driver(&goodix_ts_driver);
	if (goodix_wq)
		destroy_workqueue(goodix_wq);
	
	return ;
}

module_init(cpt_ts_init);
module_exit(cpt_ts_exit);

MODULE_DESCRIPTION("Goodix Touchscreen Driver");
MODULE_LICENSE("GPL");


