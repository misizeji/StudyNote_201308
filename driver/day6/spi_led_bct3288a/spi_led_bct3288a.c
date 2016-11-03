#include <linux/kernel.h>		
#include <linux/init.h>			
#include <linux/fs.h>			
#include <asm/uaccess.h>		
#include <linux/device.h> 		
#include <linux/errno.h>		
#include <mach/gpio.h>
#include <asm/delay.h>


#include <asm/io.h>
#include <linux/ioport.h>
#include <plat/gpio-cfg.h>

#include "bct3288a.h"


#define DRIVER_NAME		"bct3288a"
#define err(msg) 					printk(KERN_ERR "%s: " msg "\n", DRIVER_NAME)
#define __debug(fmt, arg...)		printk(KERN_DEBUG fmt, ##arg)

#define CLK S5PV210_GPB(4)
#define CS S5PV210_GPB(5)

#define MOSI S5PV210_GPB(7)

#define CLR_BIT(bit) gpio_set_value(bit,0)
#define SET_BIT(bit) gpio_set_value(bit,1)

static int major;		
static int minor;		
static struct class  *bct3288a_class;
static struct device *bct3288a_dev;

static void bct3288a_write_byte(unsigned char addr,unsigned char data)
{
	int i;
	CLR_BIT(CS);
	udelay(50);
	for(i=0; i<8; i++)
	{
		if(addr & 0x80)
			SET_BIT(MOSI);/*准备数据 高电平*/
		else
			CLR_BIT(MOSI);/*准备数据 低电平*/
		udelay(50);
		SET_BIT(CLK);/*上升沿时钟*/
		udelay(50);/*接收者，自动接收数据，由硬件固化产生*/
		
		CLR_BIT(CLK);/*为下一次数据收发做准备*/
		udelay(50);
		addr = addr << 1;
	}

	for(i=0; i<8; i++)
	{
		if(data& 0x80)
			SET_BIT(MOSI);/*准备数据 高电平*/
		else
			CLR_BIT(MOSI);/*准备数据 低电平*/
		udelay(50);
		SET_BIT(CLK);/*上升沿时钟*/
		udelay(50);/*接收者，自动接收数据，由硬件固化产生*/
		
		CLR_BIT(CLK);/*为下一次数据收发做准备*/
		udelay(50);
		data = data << 1;
	}
	udelay(50);
	SET_BIT(CS);
}

static ssize_t bct3288a_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos)
{
	unsigned char buffer[2];/*内核的数据*/
	int retval;
	int cpy_size;

	__debug("led gph0 driver write called!\n");

	cpy_size = min(count, sizeof(buffer));
	retval = copy_from_user(buffer, buf, cpy_size);
	/*buffer[0] 是地址，buffer[1] 是数据*/
	bct3288a_write_byte(buffer[0],buffer[1]);	
	return (cpy_size-retval);
}
static int bct3288a_ioctl(struct inode *inode ,struct file *file,unsigned int cmd, unsigned long arg)
{
	unsigned char addr;
	unsigned char data;
	
	switch(cmd)
	{
		case BCT3288A_CLEAR:
			addr = 0xff;
			data = 0x00;
			bct3288a_write_byte(addr,data);
			break;
		case BCT3288A_CLOSE:
			addr = 0xff;
			data = 0x00;
			bct3288a_write_byte(addr,data);
			break;
	}
	return 0;
}

/* Driver Operation structure */
static struct file_operations bct3288a_fops = {
	owner:		THIS_MODULE,
	write:		bct3288a_write,
	ioctl:		bct3288a_ioctl,
};


static int io_port_init(void)
{
	/*CLK CS初始化*/
	s3c_gpio_cfgpin(CLK,S3C_GPIO_OUTPUT);
	s3c_gpio_cfgpin(CS,S3C_GPIO_OUTPUT);
	s3c_gpio_cfgpin(MOSI,S3C_GPIO_OUTPUT);
/*
	gpio_set_value(CLK,0);
	gpio_set_value(CS,1);
*/
	CLR_BIT(CLK);
	SET_BIT(CS);
	
	return 0;
}

/* Module Init & Exit function */
static int __init bct3288a_init(void)
{
	int retval = 0;
	
	io_port_init();
	
	/* Driver register */
	major = register_chrdev(major, DRIVER_NAME, &bct3288a_fops);
	if(major < 0){
		err("register char device fail!\n");
		retval =  major;
		goto error;
	}
	
	bct3288a_class = class_create(THIS_MODULE,DRIVER_NAME);
	if(IS_ERR(bct3288a_class)){
		err("class create failed!");
		retval =  PTR_ERR(bct3288a_class);
		goto error_class;
	}
	
	bct3288a_dev = device_create(bct3288a_class,NULL, MKDEV(major, minor), NULL,DRIVER_NAME);
	if(IS_ERR(bct3288a_dev)){
		err("device create failed!");
		retval = PTR_ERR(bct3288a_dev);
		goto error_device;
	}
	
	__debug("register led driver OK! major = %d\n", major);
	return 0;

error_device:
	class_destroy(bct3288a_class);
error_class:
	unregister_chrdev(major, DRIVER_NAME);	
error:
	return retval;

}

static void __exit bct3288a_exit(void)
{
	/* Driver unregister */
	unregister_chrdev(major, DRIVER_NAME);
	device_destroy(bct3288a_class,MKDEV(major, minor));
	class_destroy(bct3288a_class);

	return;
}

module_init(bct3288a_init);
module_exit(bct3288a_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("sunplusedu");

