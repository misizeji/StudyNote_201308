#include <linux/device.h>
#include <linux/interrupt.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/irq.h>
#include <asm/uaccess.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <mach/gpio.h>
#include <mach/regs-gpio.h>  /*S5PV210_GPH3_BASE*/

#include <linux/cdev.h>
#include <linux/errno.h>



#define EINT_DEVICE_ID			1

#define DRIVER_NAME				"key1_eint"
#define err(msg) 				printk(KERN_ERR "%s: " msg "\n", DRIVER_NAME)
#define __debug(fmt, arg...)	printk(KERN_WARNING fmt, ##arg)

#define GPH3CON					(unsigned long)(S5PV210_GPH3_BASE+ 0x00)
#define GPH3DAT					(unsigned long)(S5PV210_GPH3_BASE + 0x04)
#define GPH2UP					(unsigned long)(S5PV210_GPH2_BASE + 0x08)

struct class *key_class;
static struct device *key_device;

static unsigned char key;
/*定义一个dev_t类型变量 设备号类型*/
/*定义cdev结构体*/
static struct cdev cdev;
static dev_t dev_nr;
static int minors;

irqreturn_t buttons_interrupt(int irq, void *dev_id)
{
	key = (unsigned int)dev_id;
	__debug("in eint function...\n");
	return IRQ_HANDLED;
}


static ssize_t key_read(struct file *filp, char *buf, size_t count, loff_t *f_pos)
{
	int key_num;
	int cpy_len;
	int retval;

	key_num = key;		//读取键值
	key = 0;			//清除键值
	
	cpy_len = min(sizeof(key_num), count);
	retval = copy_to_user(buf, &key_num, cpy_len);
	
	return (cpy_len - retval);
}

/* Driver Operation structure */
static struct file_operations key_fops = {
	.owner = THIS_MODULE,
	.read = key_read,
};

static void key_io_port_init(void)
{
	unsigned long reg_val;
	
	reg_val = readl(GPH3CON);
	reg_val &= ~((0x0f<<0) | (0x0f<<4));
	reg_val |= ((0x01<<0) | (0x01<<4));
	writel(reg_val, GPH3CON);

	reg_val = readl(GPH3DAT);
	reg_val &= ~((0x01<<0) | (0x01<<1));
	writel(reg_val, GPH3DAT);

	reg_val = readl(GPH2UP);
	reg_val &= ~(0x03<<8);
	reg_val |= 0x02<<8;
	writel(reg_val, GPH2UP);
}
static int __init key_eint_init(void)
{
	int retval;
	int ret;

	key_io_port_init();

	/*设置中断的触发类型，我们也可以在request_irq函数中直接设置中断触发类*/
	retval = set_irq_type(IRQ_EINT(20),IRQ_TYPE_EDGE_FALLING);
	if(retval){
		err("IRQ_EINT20 set irq type failed");
		goto error;
	}
	
	retval = request_irq(IRQ_EINT(20), buttons_interrupt, IRQF_DISABLED, 
			"KEY1", (void *)EINT_DEVICE_ID);
	if(retval){
		err("request eint20 failed");
		goto error;
	}

	minors = 1;
	/* Driver register */
	/*申请主从设备号*/
	ret = alloc_chrdev_region(&dev_nr,0,minors,DRIVER_NAME);
	if(ret){
		__debug("alloc chrdev region failed!\n");
		return ret;
	}
	__debug("the major requested is %d\n",MAJOR(dev_nr));
	
	/*初始化cdev*/
	cdev_init(&cdev, &key_fops);
	/*注册cdev*/
	if(cdev_add(&cdev, dev_nr, minors))
		goto error_register;
	
	key_class=class_create(THIS_MODULE,DRIVER_NAME);
	if(IS_ERR(key_class)){
		err("class create failed!");
		retval =  PTR_ERR(key_class);
		goto error_class;
	}
	key_device=device_create(key_class,NULL, dev_nr, NULL,DRIVER_NAME);
	if(IS_ERR(key_device)){
		err("device create failed!");
		retval = PTR_ERR(key_device);
		goto error_device;
	}
	//__debug("register myDriver OK! Major = %d\n", major);
	return 0;

	
error_device:
	class_destroy(key_class);
error_class:
	cdev_del(&cdev);
error_register:
	unregister_chrdev_region(dev_nr, minors);
	free_irq(IRQ_EINT(20), (void *)EINT_DEVICE_ID);
error:
	return retval;
}

static void __exit key_eint_exit(void)
{
	//__debug("in key_eint_exit\n");
	
	free_irq(IRQ_EINT(20), (void *)EINT_DEVICE_ID);
	/*注销cdev*/
	cdev_del(&cdev);
	/*注销主从设备号*/
	unregister_chrdev_region(dev_nr, minors);
	
	device_destroy(key_class,dev_nr);
	class_destroy(key_class);

	return;
}

module_init(key_eint_init);
module_exit(key_eint_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("sunplsuedu");

