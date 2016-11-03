#include <linux/kernel.h>
#include <linux/init.h>	
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/device.h>
#include <linux/errno.h>		/* Error number */
#include <linux/types.h>
#include <linux/proc_fs.h>	
#include <linux/string.h>
#include <linux/slub_def.h>

#include <mach/regs-gpio.h>  	
#include <asm/io.h>


#define DRIVER_NAME		"led_gph0"
#define PROC_DIRECTORY  "led_gph0"
#define PROC_FILE		"led"

#define err(msg) 				printk(KERN_ERR "%s: " msg "\n", DRIVER_NAME)
#define __debug(fmt, arg...)	printk(KERN_DEBUG fmt, ##arg)

/*
 *GPH0相关寄存器虚拟地址，我们已知GPH0虚拟基地址为
 *S5PV210_GPH0_BASE（内核定义）
 */
#define GPH0_CONREG			(S5PV210_GPH0_BASE + 0x00)
#define GPH0_DATREG			(S5PV210_GPH0_BASE + 0x04)
#define GPH0_UPREG			(S5PV210_GPH0_BASE + 0x08)
#define GPH0_DRVREG			(S5PV210_GPH0_BASE + 0x0c)

static int major;		
static int minor;		
static struct class *led_class;
static struct device *led_dev;

/*定义一个proc文件的结构体指针*/

static struct proc_dir_entry *proc_entry;


static ssize_t led_gph0_read(struct file *filp, char *buf, size_t count, loff_t *f_pos)
{
	unsigned long dat_val;
	int cpy_size;
	int retval;

	__debug("led gph0 driver read called!\n");
	dat_val = readl(GPH0_DATREG);				//获取GPH0 DATA寄存器的值

	cpy_size = min(count, sizeof(dat_val));		//判断可拷贝数据的有效数目
	retval = copy_to_user(buf, &dat_val, cpy_size);		//拷贝数据到用户空间

	return (cpy_size-retval);					//返回实际传输的数据数目
}

static ssize_t led_gph0_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos)
{
	unsigned long io_data;
	unsigned long dat_val;
	int retval;
	int cpy_size;

	__debug("led gph0 driver write called!\n");

	cpy_size = min(count, sizeof(io_data));
	retval = copy_from_user(&io_data, buf, cpy_size);

	/*
	 *对寄存器进行写操作时，我们本着先读后写的原则，保证不相关的位
	 *不会受到影响，详细过程如下
	 */
	dat_val = readl(GPH0_DATREG);
	dat_val = (dat_val & ~0x0f) | (io_data & 0x0f);
	writel(dat_val, GPH0_DATREG);
	
	return (cpy_size-retval);
}

/* Driver Operation structure */
static struct file_operations led_gph0_fops = {
	owner:		THIS_MODULE,
	write:		led_gph0_write,
	read:		led_gph0_read,
};

static int io_port_init(void)
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
	
	return 0;
}

/*编写写的函数，用户执行echo重定向*/
int write_func(struct file *filp, const char *buffer, unsigned long count, void *data)
{
	char ary[100] = {0};
	int num = copy_from_user(ary, buffer, count);
	printk("%s---%d\n",ary,num);
	return num;
}

/* Module Init & Exit function */
static int __init s5pv210_led_gph0_init(void)
{
	int retval = 0;
	
	io_port_init();
	
	/* 注册字符设备驱动，返回成功分配的主设备号， 失败返回错误值 */
	major = register_chrdev(major, DRIVER_NAME, &led_gph0_fops);
	if(major < 0){
		err("register char device fail!\n");
		retval =  major;
		goto error;
	}
	
	/*创建设备类（设备模型相关知识，后期课程有介绍）*/
	led_class = class_create(THIS_MODULE,DRIVER_NAME);
	if(IS_ERR(led_class)){
		err("class create failed!");
		retval =  PTR_ERR(led_class);
		goto error_class;
	}
	
	/*创建设备文件，通知用户在“/dev/”目录下创件名字为DRIVER_NAME的设备文件*/
	led_dev = device_create(led_class,NULL, MKDEV(major, minor), NULL,DRIVER_NAME);
	if(IS_ERR(led_dev)){
		err("device create failed!");
		retval = PTR_ERR(led_dev);
		goto error_device;
	}
	
	/*创建文件，获得指向文件的指针*/
	proc_entry = create_proc_entry("led_gph0",0666, NULL);
	/*把实现好的写函数对结构体变量赋值*/
	proc_entry->write_proc = write_func;
	
	__debug("register led driver OK! major = %d\n", major);
	return 0;

error_device:
	class_destroy(led_class);
error_class:
	unregister_chrdev(major, DRIVER_NAME);	
error:
	return retval;

}

static void __exit s5pv210_led_gph0_exit(void)
{
	/* Driver unregister */
	unregister_chrdev(major, DRIVER_NAME);
	device_destroy(led_class,MKDEV(major, minor));
	class_destroy(led_class);
	/*删除proc目录文件*/
	remove_proc_entry("led_gph0", NULL);
	return;
}

module_init(s5pv210_led_gph0_init);
module_exit(s5pv210_led_gph0_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("sunplusedu");

