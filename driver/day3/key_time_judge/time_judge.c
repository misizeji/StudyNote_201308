#include <linux/module.h>  
#include <linux/kernel.h>	
#include <linux/init.h>		
#include <linux/fs.h>
#include <asm/uaccess.h>	
#include <linux/device.h>  	
#include <linux/errno.h>	
#include <linux/device.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/irq.h>
#include <asm/uaccess.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <mach/gpio.h>
#include <mach/regs-gpio.h>  /*S5PV210_GPH3_BASE*/

#include <linux/timer.h>
#include <linux/jiffies.h>
#include <linux/kfifo.h>


#define DRIVER_NAME			"time_judge"
#define err(msg) 					printk(KERN_ERR "%s: " msg "\n", DRIVER_NAME)
#define __debug(fmt, arg...)		printk(KERN_DEBUG fmt, ##arg)

#define GPH2CON					(unsigned long)(S5PV210_GPH2_BASE+ 0x00)
#define GPH2DAT					(unsigned long)(S5PV210_GPH2_BASE + 0x04)
#define GPH2UP					(unsigned long)(S5PV210_GPH2_BASE + 0x08)
#define GPH2DRV					(unsigned long)(S5PV210_GPH2_BASE+ 0x0C)

#define GPH3CON					(unsigned long)(S5PV210_GPH3_BASE+ 0x00)
#define GPH3DAT					(unsigned long)(S5PV210_GPH3_BASE + 0x04)
#define GPH3UP					(unsigned long)(S5PV210_GPH3_BASE + 0x08)
#define GPH3DRV					(unsigned long)(S5PV210_GPH3_BASE+ 0x0C)


static int major = 0;		
static int minor = 0;		
struct class *timer_class;
static struct device *timer_device;

/*申请定时器、fifo、微线程*/
static struct timer_list timer;
static struct kfifo key_fifo;
static struct tasklet_struct  tasklet;

irqreturn_t buttons_interrupt(int irq, void *dev_id)
{	
	/*将tasklet添加到微线程的调度列表中并返回*/	
	tasklet_schedule(&tasklet);
	return IRQ_HANDLED;
}

static void tasklet_handler(unsigned long arg)
{
	unsigned long gph2_val;
	int retval;
	int key = -1; 
	gph2_val = readl(GPH2DAT);
	
	if((gph2_val & 0xf8) == 0)
	{
		udelay(1000);//消抖
		gph2_val = readl(GPH2DAT);
		if((gph2_val & 0xf8) == 0)
		{
			key = 3;
			retval = kfifo_in(&key_fifo, &key, sizeof(key));//根据返回值判断是否成功	
			if(retval != sizeof(key))
			{		
				__debug("warning: the kfifo is full\n");	
			}
			
			if(!timer_pending(&timer))
			{
				timer.expires = jiffies + 1*HZ;
				add_timer(&timer);
			}
		}
	}
	return;
	
}

static void timer_handler(unsigned long dat)
{
	static int sec = 0;
	unsigned long gph2_val;
	int retval;
	int key = -1; 
	
	gph2_val = readl(GPH2DAT);
	if((gph2_val & 0xf8) == 0)
	{
		udelay(1000);//消抖
		gph2_val = readl(GPH2DAT);
		//printk("gph2=%d\n",gph2_val);
		if((gph2_val & 0xf8) == 0)
		{
			key = 3;
			retval = kfifo_in(&key_fifo, &key, sizeof(key));//根据返回值判断是否成功	
			if(retval != sizeof(key))
			{		
				__debug("warning: the kfifo is full\n");	
			}	
			sec++;
			//printk("<kernel>: deal timer handler.... sec = %d\n", sec);		//打印测试信息
			/*更新定时器到时时间，并开启定时器(一次1s)*/
			mod_timer(&timer, jiffies+1*HZ);
		}
	}
	return;
}

static ssize_t key_read(struct file *filp, char *buf, size_t count, loff_t *f_pos)
{
	int keynum = 0;
	int cpy_len;
	int retval;

	retval = kfifo_out(&key_fifo, &keynum, sizeof(keynum)); //根据返回值判断是否成功
	if(retval == sizeof(keynum)){
		//printk("keynum=%d\n", keynum);
		cpy_len = min(sizeof(keynum), count);
		retval = copy_to_user(buf, &keynum, cpy_len);
		
		return (cpy_len - retval);
	}
	return 0;
}


static struct file_operations timer_test_fops = {
	.owner = THIS_MODULE,
	.read = key_read,
};

static void key_io_port_init(void)
{	
	unsigned long reg_val;
	/*GPH3 配置成输出 低电平
	  GPH2 配置成输入，上拉使能*/
	reg_val = readl(GPH3CON);	
	reg_val &= ~((0x0f<<(0*4)) | (0x0f<<(1*4)));	
	reg_val |= ((0x01<<(0*4)) | (0x01<<(1*4)));	
	writel(reg_val, GPH3CON);	
	
	reg_val = readl(GPH3DAT);	
	reg_val &= ~((0x01<<0) | (0x01<<1));
	reg_val |= ((0x00<<0) | (0x00<<1));
	writel(reg_val, GPH3DAT);	

	reg_val = readl(GPH2CON);	
	reg_val &= ~(0x0f<<(4*4));	
	reg_val |= (0x00<<(0*4));	
	writel(reg_val, GPH2CON);
	
	reg_val = readl(GPH2UP);	
	reg_val &= ~(0x03<<(2*4));	
	reg_val |= (0x02<<8);	
	writel(reg_val, GPH2UP);
}


static int __init timer_test_init(void)
{
	int retval;

	key_io_port_init();
	//__debug("in key_eint_init\n");
	init_timer(&timer);			//初始化定时器
	timer.data = 0L;				//设定定时器参数
	timer.function = timer_handler;	//设定定时器服务函数

	/*申请fifo空间*/
	if(kfifo_alloc(&key_fifo, 128, GFP_KERNEL) != 0)
	{		
		err("kfifo malloc failed!\n");			
		retval = -ENOMEM;		
		goto error;	
	}

	/* 初始化微线程，指定线程处理函数tasklet_handler，	 
	* 传递参数-- key变量的地址	 
	*/	
	tasklet_init(&tasklet, tasklet_handler, 0);

	/*设置申请中断*/
	retval = set_irq_type(IRQ_EINT(20),IRQ_TYPE_EDGE_FALLING);	
	if(retval)
	{		
		err("IRQ_EINT20 set irq type failed");		
		goto error;	
	}
	
	retval = request_irq(IRQ_EINT(20), buttons_interrupt, IRQF_DISABLED,"KEY", NULL);
	if(retval)
	{		
		err("request eint20 failed");	
		goto error;	
	}
	
	major = register_chrdev(major, DRIVER_NAME, &timer_test_fops);
	if(major < 0){
		err("register char device fail!");
		retval = major;
		goto error;
	}
	timer_class=class_create(THIS_MODULE,DRIVER_NAME);
	if(IS_ERR(timer_class)){
		err("class create failed!");
		retval =  PTR_ERR(timer_class);
		goto error_class;
	}
	timer_device=device_create(timer_class,NULL, MKDEV(major, minor), NULL,DRIVER_NAME);
	if(IS_ERR(timer_device)){
		err("device create failed!");
		retval = PTR_ERR(timer_device);
		goto error_device;
	}
	__debug("register timer test OK! Major = %d\n", major);
	return 0;

error_device:
	class_destroy(timer_class);
error_class:
	unregister_chrdev(major, DRIVER_NAME);
error:
	free_irq(IRQ_EINT(20), NULL);
	return retval;
}

static void __exit timer_test_exit(void)
{
	kfifo_free(&key_fifo);
	free_irq(IRQ_EINT(20), NULL);
	tasklet_kill(&tasklet);

	unregister_chrdev(major, DRIVER_NAME);
	device_destroy(timer_class,MKDEV(major, minor));
	class_destroy(timer_class);

	return;
}

module_init(timer_test_init);
module_exit(timer_test_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("sunplusedu");

