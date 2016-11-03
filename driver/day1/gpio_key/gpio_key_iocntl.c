#include <linux/kernel.h>
#include <linux/init.h>	
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/device.h>
#include <linux/errno.h>		/* Error number */
#include <linux/types.h>
#include <mach/regs-gpio.h>  	
#include <asm/io.h>

#include <mach/gpio.h>
#include "key_ioctl.h"


#define DRIVER_NAME		"key_ioctl"
#define err(msg) 				printk(KERN_ERR "%s: " msg "\n", DRIVER_NAME)
#define __debug(fmt, arg...)	printk(KERN_DEBUG fmt, ##arg)

/*
 *GPH0相关寄存器虚拟地址，我们已知GPH0虚拟基地址为
 *S5PV210_GPH0_BASE（内核定义）
 */
#define GPH2_CONREG			(S5PV210_GPH2_BASE + 0x00)
#define GPH2_DATREG			(S5PV210_GPH2_BASE + 0x04)
#define GPH2_UPREG			(S5PV210_GPH2_BASE + 0x08)
#define GPH2_DRVREG			(S5PV210_GPH2_BASE + 0x0c)

#define GPH3_CONREG			(S5PV210_GPH3_BASE + 0x00)
#define GPH3_DATREG			(S5PV210_GPH3_BASE + 0x04)
#define GPH3_UPREG			(S5PV210_GPH3_BASE + 0x08)
#define GPH3_DRVREG			(S5PV210_GPH3_BASE + 0x0c)


static int major;		
static int minor;		
static struct class *key_class;
static struct device *key_dev;

static ssize_t key_read(struct file *filp, char *buf, size_t count, loff_t *f_pos)
{

	unsigned long dat_val;
	int cpy_size;
	int retval;
	/*read读(读GPH2DAT)判断电平*/
	dat_val = readl(GPH2_DATREG);
	/*读GPH2键值返回给应用*/
	cpy_size = min(count, sizeof(dat_val));
	retval = copy_to_user(buf, &dat_val, cpy_size);

	return cpy_size - retval;
	/*
	__debug("led gph0 driver read called!\n");
	dat_val = readl(GPH2_DATREG);				//获取GPH0 DATA寄存器的值

	cpy_size = min(count, sizeof(dat_val));		//判断可拷贝数据的有效数目
	retval = copy_to_user(buf, &dat_val, cpy_size);		//拷贝数据到用户空间

	return (cpy_size-retval);					//返回实际传输的数据数目
	*/
}


int key_ioctl (struct inode *inode, struct file *filp,unsigned int cmd, unsigned long arg)
{
	/*cmd是命令*/

	/*arg是参数,表示某一个IO口*/

	switch(cmd)
	{
		//配置GPH3某个IO口为输出
		case GPH3CON_CTL:
			s3c_gpio_cfgpin(S5PV210_GPH3(arg), S3C_GPIO_OUTPUT);
			break;
		//输出低电平
		case GPH3DAT_CTL:
			gpio_set_value(S5PV210_GPH3(arg), 0);
			break;
	}
	return 0;


}



/* Driver Operation structure */
static struct file_operations key_fops = {
	owner:		THIS_MODULE,
	read:		key_read,
	ioctl:		key_ioctl,
};

static int io_port_init(void)
{
	unsigned long con_val, up_val;
	
	/*GPH3设置输出为低电平*/
	/*
	con_val = readl(GPH3_CONREG);
	con_val &=~ (0x0f<<0);
	con_val |=  (0x01<<0);
	writel(con_val, GPH3_CONREG);

	dat_val = readl(GPH3_DATREG);
	dat_val &=~ (0x01<<0);
	dat_val |=  (0x00<<0);
	writel(dat_val, GPH2_DATREG);
	*/
	
	/*GPH2设置输入为上拉使能*/
	con_val = readl(GPH2_CONREG);
	con_val &=~ ((0x0f<<(4*3)) | (0x0f<<(4*4)) | (0x0f<<(4*5)) | (0x0f<<(4*6)) | (0x0f<<(4*7)));
	con_val |=  ((0x00<<(4*3)) | (0x00<<(4*4)) | (0x00<<(4*5)) | (0x00<<(4*6)) | (0x0f<<(4*7)));
	writel(con_val, GPH2_CONREG);
	
	up_val = readl(GPH2_UPREG);
	up_val &=~ ((0x03<<(2*3)) | (0x03<<(2*4)) | (0x03<<(2*5)) | (0x03<<(2*6)) | (0x03<<(2*7)));
	up_val |=  ((0x02<<(2*3)) | (0x02<<(2*4)) | (0x02<<(2*5)) | (0x02<<(2*6)) | (0x02<<(2*7)));
	writel(up_val, GPH2_UPREG);
	

	/*
	 *引脚连接：GPH0 引脚1、2、3、4分别连接led1、2、3、4
	 *配置GPH0引脚为输出，上拉使能
	 */
	 /*
	con_val = readl(GPH0_CONREG);
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
static int __init key_init(void)
{
	int retval = 0;
	
	io_port_init();
	
	/* 注册字符设备驱动，返回成功分配的主设备号， 失败返回错误值 */
	major = register_chrdev(major, DRIVER_NAME, &key_fops);
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
	key_dev = device_create(key_class,NULL, MKDEV(major, minor), NULL, DRIVER_NAME);
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

static void __exit key_exit(void)
{
	/* Driver unregister */
	unregister_chrdev(major, DRIVER_NAME);
	device_destroy(key_class,MKDEV(major, minor));
	class_destroy(key_class);

	return;
}

module_init(key_init);
module_exit(key_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mr.G");

