#include <linux/kernel.h>	
#include <linux/init.h>	
#include <linux/fs.h>		
#include <asm/uaccess.h>		
#include <linux/device.h> 		
#include <linux/errno.h>		
#include <linux/types.h>

#include <linux/delay.h>
#include <plat/gpio-cfg.h>
#include <mach/gpio.h>


#define DRIVER_NAME				"bma_150"
#define err(msg) 					printk(KERN_ERR "%s: " msg "\n", DRIVER_NAME)
#define __debug(fmt, arg...)		printk(KERN_DEBUG fmt, ##arg)

#define SCL S5PV210_GPD1(5)
#define SDA S5PV210_GPD1(4)

#define chip_addr  0x38
#define reg_addr   0x02

#define SET_BIT(bit)  gpio_set_value(bit,1)
#define CLR_BIT(bit)  gpio_set_value(bit,0)


typedef struct _position{	
	signed short x;	
	signed short y;	
	signed short z;				
}position_t;

//typedef struct _position position_t;

static int major;		
static int minor;		
static struct class *bma_150_class;
static struct device *bma_150_dev;

static void i2c_start(void)
{
	s3c_gpio_cfgpin(SDA,S3C_GPIO_OUTPUT);
	SET_BIT(SDA);
	udelay(50);
	SET_BIT(SCL);
	udelay(50);
	CLR_BIT(SDA);
	udelay(50);
	CLR_BIT(SCL);
	udelay(50);
}

static void i2c_stop(void)
{
	s3c_gpio_cfgpin(SDA,S3C_GPIO_OUTPUT);
	CLR_BIT(SDA);
	udelay(50);
	SET_BIT(SCL);
	udelay(50);
	SET_BIT(SDA);
	udelay(50);
	SET_BIT(SCL);
	udelay(50);
}

static void i2c_send_byte(unsigned char data)
{
	int i;
	s3c_gpio_cfgpin(SDA, S3C_GPIO_OUTPUT);
	CLR_BIT(SCL);
	for (i=0;i<8;i++)
	{
		if(data & 0x80)
		{
			SET_BIT(SDA);
		}
		else
		{
			CLR_BIT(SDA);
		}
		udelay(50);
		SET_BIT(SCL);
		udelay(50);		/*the other side would read data*/
		CLR_BIT(SCL);
		udelay(50);
		data = data << 1;
	}
	
}

static unsigned char  i2c_recv_byte(void)
{
	unsigned char data = 0;
	int i;
	s3c_gpio_cfgpin(SDA, S3C_GPIO_INPUT);
	CLR_BIT(SCL);
	for (i=0;i<8;i++)
	{
		data = data << 1;
		udelay(50);
		data |= gpio_get_value(SDA);
		udelay(50);
		SET_BIT(SCL);
		udelay(50);		/*the other side prepare data*/
		CLR_BIT(SCL);
		udelay(50);
		
	}
	return data;
}

static int i2c_recv_ack(void)
{
	int ack = 0;
	s3c_gpio_cfgpin(SDA, S3C_GPIO_INPUT);
	CLR_BIT(SCL);
	udelay(50);
	SET_BIT(SCL);
	udelay(50);
	ack = gpio_get_value(SDA);
	udelay(50);
	CLR_BIT(SCL);
	udelay(50);
	return ack;
}

static int i2c_send_ack(void)
{
	s3c_gpio_cfgpin(SDA, S3C_GPIO_OUTPUT);
	CLR_BIT(SCL);
	udelay(50);
	gpio_set_value(SDA,0);
	udelay(50);
	SET_BIT(SCL);
	udelay(50);	
	CLR_BIT(SCL);
	udelay(50);
	return 0;
}

static int i2c_send_nack(void)
{
	s3c_gpio_cfgpin(SDA, S3C_GPIO_OUTPUT);
	CLR_BIT(SCL);
	udelay(50);
	gpio_set_value(SDA,1);
	udelay(50);
	SET_BIT(SCL);
	udelay(50);
	CLR_BIT(SCL);
	udelay(50);
	return 0;
}

static void  i2c_read_bytes(unsigned char *buf, int count)
{
	int i;
	/*第一次通讯:写*/
	i2c_start();
	i2c_send_byte(chip_addr << 1 | 0);	/*发送从设备地址*/
	i2c_recv_ack();
	i2c_send_byte(reg_addr);
	i2c_recv_ack();
	i2c_stop();
	/*第二次通讯:读*/
	i2c_start();
	i2c_send_byte(chip_addr << 1 | 1);
	i2c_recv_ack();
	
	for(i=0;i<count-1;i++)
	{
		buf[i] = i2c_recv_byte();
		i2c_send_ack();
	}
	buf[count - 1] = i2c_recv_byte();
	i2c_send_nack();
	i2c_stop();
	
}

static ssize_t bma_150_read(struct file *filp, char *buf, size_t count, loff_t *f_pos)
{
	unsigned char data[6];
	static position_t pos;
	int cpy_size;
	int retval;
	int i;
	signed short x,y,z;


	i2c_read_bytes(data,sizeof(data));

	x = (((signed short)(data[1])) << 2) | (((signed short)(data[0])) >> 6);
	y = (((signed short)(data[3])) << 2) | (((signed short)(data[2])) >> 6);
	z = (((signed short)(data[5])) << 2) | (((signed short)(data[4])) >> 6);

	pos.x = (signed short) (x << 6) >> 6;
	pos.y = (signed short) (y << 6) >> 6;
	pos.z = (signed short) (z << 6) >> 6;

	/*测试办法:打印data数组的数据*/
/*	for(i=0; i<6; i++)
	{
		printk("%d ",data[i]);
	}
	printk("\n");
*/
	cpy_size = min(count, sizeof(pos));
	retval = copy_to_user(buf, &pos, cpy_size);

	return (cpy_size-retval);
}

/* Driver Operation structure */
static struct file_operations led_gph0_fops = {
	owner:		THIS_MODULE,
	read:		bma_150_read,
};


static int io_port_init(void)
{
	s3c_gpio_cfgpin(SCL, S3C_GPIO_OUTPUT);
	return 0;
}

/* Module Init & Exit function */
static int __init bma_150_init(void)
{
	int retval = 0;

	io_port_init();
	
	/* Driver register */
	major = register_chrdev(major, DRIVER_NAME, &led_gph0_fops);
	if(major < 0){
		err("register char device fail!\n");
		retval =  major;
		goto error;
	}
	
	bma_150_class = class_create(THIS_MODULE,DRIVER_NAME);
	if(IS_ERR(bma_150_class)){
		err("class create failed!");
		retval =  PTR_ERR(bma_150_class);
		goto error_class;
	}
	
	bma_150_dev = device_create(bma_150_class,NULL, MKDEV(major, minor), NULL,DRIVER_NAME);
	if(IS_ERR(bma_150_dev)){
		err("device create failed!");
		retval = PTR_ERR(bma_150_dev);
		goto error_device;
	}
	
	__debug("register led driver OK! major = %d\n", major);
	return 0;

error_device:
	class_destroy(bma_150_class);
error_class:
	unregister_chrdev(major, DRIVER_NAME);	
error:
	return retval;


}

static void __exit bma_150_exit(void)
{
	unregister_chrdev(major, DRIVER_NAME);
	device_destroy(bma_150_class,MKDEV(major, minor));
	class_destroy(bma_150_class);

	return;
}

module_init(bma_150_init);
module_exit(bma_150_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("sunplusedu");

