#include <linux/kernel.h>
#include <linux/init.h>	
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/device.h>
#include <linux/errno.h>		/* Error number */
#include <linux/types.h>
#include <mach/regs-gpio.h>  	
#include <asm/io.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/kernel_stat.h>
#include <asm/setup.h>
#include <asm/system.h>
#include <asm/irq.h>
#include <asm/traps.h>
#include <asm/page.h>
#include <asm/cacheflush.h>
#include <asm/irq_regs.h>



#define DRIVER_NAME		"key_eint"
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
static struct class *eint_key_class;
static struct device *eint_key_dev;

int key_global = 0;

#define DEV_ID 827

irqreturn_t key_int_fun(int irq, void *dev_id)
{
	int id_data = 0;
	id_data = (int)dev_id;
	printk("%s %d\n",__func__, id_data);
	/*键码*/
	key_global = 3;
	return IRQ_HANDLED;
}

static ssize_t key_eint_read(struct file *filp, char *buf, size_t count, loff_t *f_pos)
{
	int cpy_size;
	int retval;
	/*键值返回给应用*/
	cpy_size = min(count, sizeof(key_global));
	retval = copy_to_user(buf, &key_global, cpy_size); /*返回值是失败的个数*/
	return (cpy_size - retval);
}


/* Driver Operation structure */
static struct file_operations eint_key_fops = {
	owner:		THIS_MODULE,
	read:		key_eint_read,
};

static int io_port_init(void)
{
	unsigned long con_val, dat_val, up_val;
	
	/*GPH3设置输出为低电平*/
	con_val = readl(GPH3_CONREG);
	con_val &=~ (0x0f<<0);
	con_val |=  (0x01<<0);
	writel(con_val, GPH3_CONREG);

	dat_val = readl(GPH3_DATREG);
	dat_val &=~ (0x01<<0);
	dat_val |=  (0x00<<0);
	writel(dat_val, GPH3_DATREG);
	
	/*GPH2设置为上拉使能*/
	con_val = readl(GPH2_CONREG);
	con_val &=~ ((0x0f<<12) | (0x0f<<16) | (0x0f<<20) | (0x0f<<24) | (0x0f<<28));
	con_val |=  ((0x00<<12) | (0x00<<16) | (0x00<<20) | (0x00<<24) | (0x0f<<28));
	writel(con_val, GPH2_CONREG);
	
	return 0;
}

/* Module Init & Exit function */
static int __init key_eint_init(void)
{
	int retval = 0;
	
	io_port_init();
	/*中断申请*/
	request_irq(IRQ_EINT(20), key_int_fun, IRQF_DISABLED, "heihei", (void *)DEV_ID);
	/*设置中断触发方式*/
	set_irq_type(IRQ_EINT(20), IRQ_TYPE_EDGE_FALLING);
	
	/* 注册字符设备驱动，返回成功分配的主设备号， 失败返回错误值 */
	major = register_chrdev(major, DRIVER_NAME, &eint_key_fops);
	if(major < 0){
		err("register char device fail!\n");
		retval =  major;
		goto error;
	}
	
	/*创建设备类（设备模型相关知识，后期课程有介绍）*/
	eint_key_class = class_create(THIS_MODULE,DRIVER_NAME);
	if(IS_ERR(eint_key_class)){
		err("class create failed!");
		retval =  PTR_ERR(eint_key_class);
		goto error_class;
	}
	
	/*创建设备文件，通知用户在“/dev/”目录下创件名字为DRIVER_NAME的设备文件*/
	eint_key_dev = device_create(eint_key_class,NULL, MKDEV(major, minor), NULL, DRIVER_NAME);
	if(IS_ERR(eint_key_dev)){
		err("device create failed!");
		retval = PTR_ERR(eint_key_dev);
		goto error_device;
	}
	__debug("register led driver OK! major = %d\n", major);
	return 0;

error_device:
	class_destroy(eint_key_class);
error_class:
	unregister_chrdev(major, DRIVER_NAME);	
error:
	return retval;

}

static void __exit key_eint_exit(void)
{
	/* Driver unregister */

	free_irq(IRQ_EINT(20), (void *)DEV_ID);
	unregister_chrdev(major, DRIVER_NAME);
	device_destroy(eint_key_class,MKDEV(major, minor));
	class_destroy(eint_key_class);

	return;
}

module_init(key_eint_init);
module_exit(key_eint_exit);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mr.G");

