#include <linux/module.h>   
#include <linux/kernel.h>	
#include <linux/init.h>		
#include <linux/fs.h>		
#include <asm/uaccess.h>	
#include <linux/device.h>  


/*主设备号*/
static int major;
static int minor;
static struct class *char_driver_class;
static struct device *char_driver_device;

/*实现open*/
static int char_driver_open(struct inode *inode,struct file *filp)
{
	printk("%s\n",__func__);	/*打印当前函数的函数名称*/
	return 0;
}

/*实现read*/ 
static ssize_t char_driver_read(struct file *filp, char __user *buf, size_t count ,loff_t *f_pos)
{
	unsigned char ary[20] = {"haha"};
	printk("%s\n",__func__);
	
	copy_to_user(buf, ary, sizeof(ary));
	return 0;
}

/*实现write*/
static ssize_t char_driver_write(struct file *filp,const char __user *buf,size_t count ,loff_t *f_pos)
{
	unsigned char ary[20] = {0};
	copy_from_user(ary, buf, count);
	printk("%s ary = %s\n",__func__,ary);
	return 0;
}

/*实现release*/ 

static int char_driver_release(struct inode *inode,struct file *filp)
{
	printk("%s\n",__func__);
	return 0;
}

/*内核传参给驱动返回值由内核接收*/

static struct file_operations char_driver_fops = {
	
	.owner   = THIS_MODULE,
	
	.open    = char_driver_open, 	/*用实现好的open函数赋值*/
	
	.read    = char_driver_read, 	/*用实现好的read函数赋值*/
	
	.write   = char_driver_write, 	/*用实现好的write函数赋值*/
	
	.release = char_driver_release, /*用实现好的release函数赋值*/

};



/*module add*/
static int char_driver_init(void)
{
	
	printk("I am coming!!!\n");
	/*注册驱动*/
	major = register_chrdev(major, "char_driver", &char_driver_fops);
	printk("%d\n",major);
	/*生成设备节点*/
	/*cat /proc/devices*/

	/*2.生成设备文件*/
		
	/*2.1生成设备类*/
	char_driver_class = class_create(THIS_MODULE, "char_driver");
	/*2.2生成设备节点*/
	char_driver_device = device_create(char_driver_class, NULL, MKDEV(major,minor),NULL,"char_driver");
	return 0;

}


/*module exit*/
static void char_driver_exit(void)

{
	unregister_chrdev(major, "char_driver");
	device_destroy(char_driver_class, MKDEV(major, minor));
	class_destroy(char_driver_class);
	printk("goodbye I am leaving!!!\n");

	return ;

}


/*define module add*/

module_init(char_driver_init);





/*define module exit*/


module_exit(char_driver_exit);


/*define themodule author*/

MODULE_AUTHOR("Mr.G");


/*define themodule permission*/

MODULE_LICENSE("GPL");

              

