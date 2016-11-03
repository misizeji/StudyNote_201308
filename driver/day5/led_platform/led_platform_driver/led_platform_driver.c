#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/device.h>
#include <asm/uaccess.h>
#include <linux/irq.h>
#include <linux/ioport.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/err.h>
#include <linux/platform_device.h>
#include <linux/wait.h>
#include <linux/semaphore.h> 	//struct semaphore
#include <asm/io.h> 			//__raw_writel
#include <linux/irq.h>	
#include <linux/interrupt.h>
#include <linux/spinlock.h>
#include <linux/sched.h>
#include <linux/slab.h> 		//kfree
#include <linux/delay.h>

#define DRIVER_NAME "led_gph0"
#define DEVICE_NAME "led_gph0"

#define BIT4(x) ((x)*4)
#define BIT1(x) ((x)*1)

#define GPH0_PHYSICAL_REGSIZE		16
#define GPH0_PHYSICAL_REGBASE		0xE0200C00
#define GPH0_CONREG			(gph0_base + 0x00)
#define GPH0_DATREG			(gph0_base + 0x04)
#define GPH0_UPREG			(gph0_base + 0x08)
#define GPH0_DRVREG			(gph0_base + 0x0c)

static struct semaphore readable;

static int key = 0;
static unsigned long *gph0base = NULL;

int MAJOR = 0;
int MINOR = 0;
static struct class *led_class;
static struct device *led_device;

static int led_open(struct inode *inode, struct file *filp)
{
	printk("led_open\n");
	return 0;
}


static ssize_t led_read(struct file *filp, char *buf, size_t count, loff_t *f_pos)
{
	int ret = -1;
	if (down_interruptible(&readable) == -EINTR) {
			return -1;
	}

	//ret = copy_to_user(buf, &key, count);
	
	return ret ? -1: count-ret;
	
}

static int led_release(struct inode *inode, struct file *filp)
{
	int i = 0;
	printk("led_release\n");
	i = !!i;
	return 0;
}

static struct file_operations led_gph0_fops = {
	.owner = THIS_MODULE,
	.open = led_open,
	.read = led_read,
	.release = led_release,
};


static void io_init(void)
{
	unsigned long con_val, up_val;
	/*
	 *引脚连接：GPH0 引脚1、2、3、4分别连接led1、2、3、4
	 *配置GPH0引脚为输出，上拉使能
	 */
	con_val = readl(GPH0_CONREG);
	con_val &= ~((0x0f<<0) | (0x0f<<4) | (0x0f<<8) | (0x0f<<12));
	con_val |= ((0x01<<0) | (0x01<<4) | (0x01<<8) | (0x01<<12));
	writel(con_val, GPH0_CONREG);

	up_val = readl(GPH0_UPREG);
	up_val &= ~((0X03<<0) | (0X03<<2) | (0X03<<4) | (0X03<<6));
	up_val |= ((0X02<<0) | (0X02<<2) | (0X02<<4) | (0X02<<6));
	writel(up_val, GPH0_UPREG);
	
	return ;
	
}

static int s5pv210_led_probe(struct platform_device *pdev)
{
	int retval = 0;
	
	/*@alloc region and ioremap
	 *@this part is used to alloc the resource of gph0 and remap the registers
	 */
	gph0_resource = request_mem_region(GPH0_PHYSICAL_REGBASE, 
				GPH0_PHYSICAL_REGSIZE, "led_gph0_reg");
	if(!gph0_resource){
		err("request mem region failed!");
		return -EBUSY;
	}
	/*把物理地址转换成虚拟地址*/
	gph0_base = ioremap(GPH0_PHYSICAL_REGBASE, GPH0_PHYSICAL_REGSIZE);
	if(!gph0_base){
		err("ioremap failed!");
		release_mem_region(GPH0_PHYSICAL_REGBASE, GPH0_PHYSICAL_REGSIZE);
		return -EIO;
	}

	io_init();
	
	/* Driver register */
	major = register_chrdev(major, DRIVER_NAME, &led_gph0_fops);
	if(major < 0){
		err("register char device fail!\n");
		retval =  major;
		goto error;
	}
	
	led_class = class_create(THIS_MODULE,DRIVER_NAME);
	if(IS_ERR(led_class)){
		err("class create failed!");
		retval =  PTR_ERR(led_class);
		goto error_class;
	}
	
	led_dev = device_create(led_class,NULL, MKDEV(major, minor), NULL,DRIVER_NAME);
	if(IS_ERR(led_dev)){
		err("device create failed!");
		retval = PTR_ERR(led_dev);
		goto error_device;
	}
	
	__debug("register led driver OK! major = %d\n", major);
	return 0;
	
	error_device:
		class_destroy(led_class);
	error_class:
		unregister_chrdev(major, DRIVER_NAME);	
	error:
		return retval;

}

static int s5pv210_led_remove(struct platform_device *dev)
{	
	iounmap(gph0_base);
	release_mem_region(GPH0_PHYSICAL_REGBASE, GPH0_PHYSICAL_REGSIZE);
	
	/* Driver unregister */
	unregister_chrdev(major, DRIVER_NAME);
	device_destroy(led_class,MKDEV(major, minor));
	class_destroy(led_class);

	return;
}

static struct platform_driver s5pv210_led_driver = {
		.probe = s5pv210_led_probe,
		.remove = s5pv210_led_remove,
		.driver = {
			.owner = THIS_MODULE,
			.name = "s5pv210-led",
		},
};

static int __init platform_driver_init(void)
{
	return platform_driver_register(&s5pv210_led_driver);
}


static void __exit platform_driver_exit(void)
{
	platform_driver_unregister(&s5pv210_led_driver);
	

}

module_init(platform_driver_init);
module_exit(platform_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("sunplusedu");

