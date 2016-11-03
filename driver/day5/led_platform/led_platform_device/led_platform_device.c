#include <linux/kernel.h>		
#include <linux/init.h>			
#include <linux/fs.h>			
#include <asm/uaccess.h>		
#include <linux/device.h> 		
#include <linux/errno.h>
#include <linux/platform_device.h>
	
#include <asm/io.h>
#include <linux/ioport.h>


#define DRIVER_NAME		"led_gph0"
#define err(msg) 					printk(KERN_ERR "%s: " msg "\n", DRIVER_NAME)
#define __debug(fmt, arg...)		printk(KERN_DEBUG fmt, ##arg)


#define GPH0_PHYSICAL_REGSIZE		16
#define GPH0_PHYSICAL_REGBASE		0xE0200C00
#define GPH0_CONREG			(gph0_base + 0x00)
#define GPH0_DATREG			(gph0_base + 0x04)
#define GPH0_UPREG			(gph0_base + 0x08)
#define GPH0_DRVREG			(gph0_base + 0x0c)


static struct resource led_gph0_resource[] = {
		[0] = {
			.start = GPH0_PHYSICAL_REGBASE,
			.end = GPH0_PHYSICAL_REGBASE + GPH0_PHYSICAL_REGSIZE,
			.flags = IORESOURCE_MEM,
		}
};


static void platform_led_release(struct device *dev) 
{
	printk("%s\n", __func__);
	return ;
}

static struct platform_device s5pv210_device_led = {
		.name = DRIVER_NAME,
		.id = -1,
		.num_resources = ARRAY_SIZE(led_gph0_resource),
		.resource = led_gph0_resource,
		.dev = {
			.release = platform_led_release,
		}
};

static int  __init platform_dev_init(void)
{
	int ret = 0;
	ret = platform_device_register(&s5pv210_device_led);
	if (ret)
		printk("platform_device_register failed\n");

	return ret;

}

static void  __exit platform_dev_exit(void)
{
	printk("before platform device unregister\n");
	platform_device_unregister(&s5pv210_device_led);
	printk("platform device unregister\n");
	 return ;
}


module_init(platform_dev_init);
module_exit(platform_dev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("sunplusedu");

