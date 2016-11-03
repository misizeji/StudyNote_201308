/*
 * Copyright  2011-2012  sunplusapp
 *
 * led device driver - using GPH0 I/O port
 */

/*
 * this program is free software; you can redistribute it and/or modify
 * it
 *
 * we can learn the gpio controlling through the program; in this program 
 * we use GPH0 to turn on or off the led 
 * 
 * Date and Edition:		2012-12-27  v1.4
 * Author:				dxh/zsd/Valor-Lion 
 */
 


#include <linux/kernel.h>
#include <linux/init.h>	
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/device.h>
#include <linux/errno.h>		/* Error number */
#include <linux/types.h>
#include <mach/regs-gpio.h>  	
#include <asm/io.h>


#define DRIVER_NAME		"key_driver"
#define err(msg) 				printk(KERN_ERR "%s: " msg "\n", DRIVER_NAME)
#define __debug(fmt, arg...)	printk(KERN_DEBUG fmt, ##arg)


/*
 *GPH2相关寄存器虚拟地址，我们已知GPH2虚拟基地址为
 *S5PV210_GPH2_BASE（内核定义）
 */
#define GPH2_CONREG			(S5PV210_GPH2_BASE + 0x00)
#define GPH2_DATREG			(S5PV210_GPH2_BASE + 0x04)
#define GPH2_UPREG			(S5PV210_GPH2_BASE + 0x08)
#define GPH2_DRVREG			(S5PV210_GPH2_BASE + 0x0c)


/*
 *GPH3相关寄存器虚拟地址，我们已知GPH3虚拟基地址为
 *S5PV210_GPH3_BASE（内核定义）
 */
#define GPH3_CONREG			(S5PV210_GPH3_BASE + 0x00)
#define GPH3_DATREG			(S5PV210_GPH3_BASE + 0x04)
#define GPH3_UPREG			(S5PV210_GPH3_BASE + 0x08)
#define GPH3_DRVREG			(S5PV210_GPH3_BASE + 0x0c)

static int major;		
static int minor;		
static struct class *key_class;
static struct device *key_dev;


static ssize_t key_gph2_read(struct file *filp, char *buf, size_t count, loff_t *f_pos)
{
	/*readl(GPH2DAT)判断电平*/
	unsigned long gph2_dat_val = 0,ret_key_val = 0;
	int cpy_size;
	int retval;
	__debug("key gph0 driver read called!\n");

	/*返回键码给上层应用*/
	gph2_dat_val = readl(GPH2_DATREG);/*屏蔽低位数据把高位拿出来*/
	gph2_dat_val &= ~(0x07);

	//printk("%.8lx\n",gph2_dat_val);
	
	if(gph2_dat_val == 0xf0)	/*keye2 0x11110000*/
	{
		//printk("%d\n",__LINE__);
		ret_key_val = 2;
	}
	else if(gph2_dat_val == 0xe8)	/*key3 0x11101000*/
	{
		//printk("%d\n",__LINE__);
		ret_key_val = 3;
	}
	else if(gph2_dat_val == 0xd8) /*key4 0x11011000*/
	{
		//printk("%d\n",__LINE__);
		ret_key_val = 4;
	}
	else if(gph2_dat_val == 0xb8) /*key5 0x10111000*/
	{
		//printk("%d\n",__LINE__);
		ret_key_val = 5;
	}
	else if(gph2_dat_val == 0x78) /*key6 0x01111000*/
	{
		//printk("%d\n",__LINE__);
		ret_key_val = 6;
	}

	cpy_size = min(count, sizeof(ret_key_val));		//判断可拷贝数据的有效数目
	retval = copy_to_user(buf, &ret_key_val, cpy_size);		//拷贝数据到用户空间
	printk("%d\n",cpy_size-retval);
	
	return (cpy_size-retval);					//返回实际传输的数据数目
	
/*	unsigned long dat_val;
	int cpy_size;
	int retval;

	__debug("led gph0 driver read called!\n");
	dat_val = readl(GPH0_DATREG);				//获取GPH0 DATA寄存器的值

	cpy_size = min(count, sizeof(dat_val));		//判断可拷贝数据的有效数目
	retval = copy_to_user(buf, &dat_val, cpy_size);		//拷贝数据到用户空间

	return (cpy_size-retval);					//返回实际传输的数据数目
*/

}
static ssize_t key_gph2_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos)
{

/*	unsigned long io_data;
	unsigned long dat_val;
	int retval;
	int cpy_size;

	__debug("led gph0 driver write called!\n");

	cpy_size = min(count, sizeof(io_data));
	retval = copy_from_user(&io_data, buf, cpy_size);
*/
	/*
	 *对寄存器进行写操作时，我们本着先读后写的原则，保证不相关的位
	 *不会受到影响，详细过程如下
	 */
/*	dat_val = readl(GPH0_DATREG);
	dat_val = (dat_val & ~0x0f) | (io_data & 0x0f);
	writel(dat_val, GPH0_DATREG);
	
	return (cpy_size-retval);
*/
	return 0;
}
/* Driver Operation structure */
static struct file_operations key_gph0_fops = {
	owner:		THIS_MODULE,
	read:		key_gph2_read,
};

static int io_port_init(void)
{
	/*ROW:GPH3 配置输出 低电平
	  COL:GPH2 配置输入 上拉使能*/
	unsigned long gph3_con_val, gph3_dat_val,gph2_con_val,gph2_up_val;
	gph3_con_val = readl(GPH3_CONREG);
	gph3_con_val &= ~(0x0f<<0);
	gph3_con_val |= (0x01<<0);
	writel(gph3_con_val,GPH3_CONREG);

	gph3_dat_val = readl(GPH3_DATREG);
	gph3_dat_val &= ~(0x01<<0);
	gph3_dat_val |= (0x00<<0);
	writel(gph3_dat_val,GPH3_DATREG);

	gph2_con_val = readl(GPH2_CONREG);
	gph2_con_val &= ~((0x0f<<12)|(0x0f<<16)|(0x0f<<20)|(0x0f<<24)|(0x0f<<28));
	gph2_con_val |= ((0x00<<12)|(0x00<<16)|(0x00<<20)|(0x00<<24)|(0x00<<28));
	writel(gph2_con_val,GPH2_CONREG);

	gph2_up_val = readl(GPH2_UPREG);
	gph2_up_val &= ~((0X03<<6)|(0X03<<8)|(0X03<<10)|(0X03<<12)|(0x03<<14));
	gph2_up_val |= ((0X02<<6)|(0X02<<8)|(0X02<<10)|(0X02<<12)|(0x02<<14));
	writel(gph2_up_val ,GPH2_UPREG);
	
	/*
	 *引脚连接：GPH0 引脚1、2、3、4分别连接led1、2、3、4
	 *配置GPH0引脚为输出，上拉使能
	 */
/*	con_val = readl(GPH0_CONREG);
	con_val &= ~((0x0f<<0) | (0x0f<<4) | (0x0f<<8) | (0x0f<<12));
	con_val |= ((0x01<<0) | (0x01<<4) | (0x01<<8) | (0x01<<12));
	writel(con_val, GPH0_CONREG);

	up_val = readl(GPH0_UPREG);
	up_val &= ~((0X03<<0) | (0X03<<2) | (0X03<<4) | (0X03<<6));
	up_val |= ((0X02<<0) | (0X02<<2) | (0X02<<4) | (0X02<<6));
	writel(up_val, GPH0_UPREG);
*/	
	return 0;

}

/* Module Init & Exit function */
static int __init s5pv210_key_gph2_init(void)
{
	int retval = 0;
	io_port_init();
	
	/* 注册字符设备驱动，返回成功分配的主设备号， 失败返回错误值 */
	major = register_chrdev(major, DRIVER_NAME, &key_gph0_fops);
	if(major < 0){
		err("register char device fail!\n");
		retval =  major;
		goto error;
	}
	
	/*创建设备类（设备模型相关知识，后期课程有介绍）*/
	key_class = class_create(THIS_MODULE,DRIVER_NAME);
	if(IS_ERR(key_class)){
		err("class create failed!");
		retval =  PTR_ERR(key_class);
		goto error_class;
	}
	
	/*创建设备文件，通知用户在“/dev/”目录下创件名字为DRIVER_NAME的设备文件*/
	key_dev = device_create(key_class,NULL, MKDEV(major, minor), NULL,DRIVER_NAME);
	if(IS_ERR(key_dev)){
		err("device create failed!");
		retval = PTR_ERR(key_dev);
		goto error_device;
	}
	
	__debug("register led driver OK! major = %d\n", major);
	return 0;

error_device:
	class_destroy(key_class);
error_class:
	unregister_chrdev(major, DRIVER_NAME);	
error:
	return retval;

}

static void __exit s5pv210_key_gph2_exit(void)
{
	/* Driver unregister */
	unregister_chrdev(major, DRIVER_NAME);
	device_destroy(key_class,MKDEV(major, minor));
	class_destroy(key_class);

	return;
}

module_init(s5pv210_key_gph2_init);
module_exit(s5pv210_key_gph2_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mr.G");


