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
#include <mach/regs-gpio.h>  		/*S5PV210_GPH3_BASE*/

#include <linux/workqueue.h>
#include <linux/kfifo.h>
#include <linux/semaphore.h>


#define EINT_DEVICE_ID			1

#define DRIVER_NAME				"key1_eint"
#define err(msg) 				printk(KERN_ERR "%s: " msg "\n", DRIVER_NAME)
#define __debug(fmt, arg...)	printk(KERN_DEBUG fmt, ##arg)

#define GPH0_CONREG			(S5PV210_GPH0_BASE + 0x00)
#define GPH0_DATREG			(S5PV210_GPH0_BASE + 0x04)
#define GPH0_UPREG			(S5PV210_GPH0_BASE + 0x08)
#define GPH0_DRVREG			(S5PV210_GPH0_BASE + 0x0c)

#define GPH3CON					(unsigned long)(S5PV210_GPH3_BASE+ 0x00)
#define GPH3DAT					(unsigned long)(S5PV210_GPH3_BASE + 0x04)
#define GPH2UP					(unsigned long)(S5PV210_GPH2_BASE + 0x08)

static int major = 0;		/* Driver Major Number */
static int minor = 0;		/* Driver Minor Number */
struct class *key_class;
static struct device *key_device;

//static unsigned char key;
/*定义一个kfifo变量*/
static struct kfifo key_kfifo;

/*定义一个信号量*/
static struct semaphore my_sem;


/* 定义工作队列指针变量，并定义工作变量。
 * 在此例中我们采用自定义的方式(我们还可以
 * 使用内核提供的工作队列,请参考ppt)
 */
static struct workqueue_struct *key_workqueue;
static struct work_struct key_work;


/* 工作处理函数work_handler
 * 该函数完成点亮/熄灭led灯的功能
 */
static void work_handler(struct work_struct *work)
{
	unsigned long reg_data;

	reg_data = readl(GPH0_DATREG);
	reg_data ^= 0x01<<3;
	writel(reg_data, GPH0_DATREG);
	
}

irqreturn_t buttons_interrupt(int irq, void *dev_id)
{	
	unsigned long key = (unsigned int)dev_id;
	//printk("the dev_id is %d\n",dev_id);
	/*down_trylock操作*/
	if(down_trylock(&my_sem) == 0)
	{
		/*把按键键码放入kfifo*/
		kfifo_in(&key_kfifo,&key,sizeof(key));
		/*up操作*/
		up(&my_sem);
	}
	//__debug("in eint function...\n");
	/*将work添加到工作队列workqueue中*/
	queue_work(key_workqueue, &key_work);
	return IRQ_HANDLED;
}

/* the initialization of IO ports
 * @prepare for key device and led device
 */
static void led_io_port_init(void)
{
	unsigned long con_val, up_val;
	
	con_val = readl(GPH0_CONREG);	
	con_val &= ~(0x0f<<12);
	con_val |= (0x01<<12);	
	writel(con_val, GPH0_CONREG);

	up_val = readl(GPH0_UPREG);
	up_val &= ~(0X03<<6);
	up_val |= (0X02<<6);
	writel(up_val, GPH0_UPREG);
}

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

static ssize_t key_read(struct file *filp, char *buf, size_t count, loff_t *f_pos)
{
	int key_num;
	int cpy_len;
	int retval;
	/*down操作*/
	down(&my_sem);
	/*把键码读取出来返回给上层应用*/
	kfifo_out(&key_kfifo,&key_num, sizeof(key_num));
	/*up操作*/
	up(&my_sem);
	//key_num = key;		//读取键值
	//key = 0;				//清除键值
	//printk("the key_num is %d\n",key_num);
	cpy_len = min(sizeof(key_num), count);
	retval = copy_to_user(buf, &key_num, cpy_len);
	
	return (cpy_len - retval);
}

static struct file_operations key_fops = {
	.owner = THIS_MODULE,
	.read = key_read,
};

static int __init key_eint_init(void)
{
	int retval;
	
	key_io_port_init();
	led_io_port_init();
	/*初始化信号量*/
	sema_init(&my_sem,1);
	/*为kfifo开闭空间*/
	kfifo_alloc(&key_kfifo,512, GFP_KERNEL);
	
	/* 创建工作队列--workqueue*/
	key_workqueue = create_workqueue("key_workqueue");
	if(IS_ERR(key_workqueue)){
		err("ceate workqueue failed!");
		return PTR_ERR(key_workqueue);
	}
	/*初始化工作--work, 指定处理函数work_handler*/
	INIT_WORK(&key_work, work_handler);
	
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
	
	/* Driver register */
	major = register_chrdev(major, DRIVER_NAME, &key_fops);
	if(major < 0){
		err("register char device fail");
		retval = major;
		goto error_register;
	}
	key_class=class_create(THIS_MODULE,DRIVER_NAME);
	if(IS_ERR(key_class)){
		err("class create failed!");
		retval =  PTR_ERR(key_class);
		goto error_class;
	}
	key_device=device_create(key_class,NULL, MKDEV(major, minor), NULL,DRIVER_NAME);
	if(IS_ERR(key_device)){
		err("device create failed!");
		retval = PTR_ERR(key_device);
		goto error_device;
	}
	__debug("register myDriver OK! Major = %d\n", major);
	return 0;

error_device:
	class_destroy(key_class);
error_class:
	unregister_chrdev(major, DRIVER_NAME);
error_register:
	free_irq(IRQ_EINT(20), (void *)EINT_DEVICE_ID);
error:
	return retval;
}

static void __exit key_eint_exit(void)
{
	/*清空工作队列*/
	flush_workqueue(key_workqueue);
	/*释放创建的工作队列，资源回收*/
	destroy_workqueue(key_workqueue);

	/*释放kfifo空间内存*/
	kfifo_free(&key_kfifo);
	free_irq(IRQ_EINT(20), (void *)EINT_DEVICE_ID);

	unregister_chrdev(major, DRIVER_NAME);
	device_destroy(key_class,MKDEV(major, minor));
	class_destroy(key_class);

	return;
}

module_init(key_eint_init);
module_exit(key_eint_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("sunplsuedu");

