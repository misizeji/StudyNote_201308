#include <linux/kernel.h>	
#include <linux/init.h>	
#include <linux/fs.h>		
#include <asm/uaccess.h>		
#include <linux/device.h> 		
#include <linux/errno.h>		
#include <linux/types.h>
#include <linux/i2c.h>
#include <linux/i2c-id.h>

#include <linux/delay.h>
#include <plat/gpio-cfg.h>
#include <mach/gpio.h>


#define DRIVER_NAME					"bma_150"
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

static struct i2c_client *bma150_client;
static const struct i2c_device_id bma150_table[] = {
	{ "bma_150", 0 },/*the same as the name in client*/
	{}
};

static void  i2c_read_bytes(unsigned char *buf, int count)
{
	//int i;
	unsigned char reg = reg_addr;
	struct i2c_msg bma150_msg[2];	/*读需要两个结构体*/ 
	bma150_msg[0].addr  = bma150_client->addr;	/*从设备地址*/
	bma150_msg[0].flags = !I2C_M_RD;	    /*读写标记位*/
	bma150_msg[0].buf   = &reg;			    /*寄存器地址*/
	bma150_msg[0].len   = 1;			    /*buf的大小*/

	bma150_msg[1].addr  = bma150_client->addr;	/*从设备地址*/
	bma150_msg[1].flags = I2C_M_RD;		    /*读写标志位*/
	bma150_msg[1].buf   = buf;			    /*读到的数据放在此处*/
	bma150_msg[1].len   = count;		    /*要读数据的大小*/

	//i2c_transfer(bma150_client->adapter, bma150_msg, 2);
	
	/*使用subsys函数去实现*/

	
}

static ssize_t bma_150_read(struct file *filp, char *buf, size_t count, loff_t *f_pos)
{
	unsigned char data[6] = "";
	static position_t pos;
	int cpy_size;
	int retval;
	short x,y,z;
//	int i;
//	unsigned char reg = reg_addr;

//	i2c_read_bytes(data,sizeof(data));
/*
	i2c_master_send(bma150_client, &reg, 1);
	udelay(50);
	i2c_master_recv(bma150_client, data, sizeof(data));
*/
	i2c_smbus_read_i2c_block_data(bma150_client, 0x02, count, data);
	
	x = (((short)(data[1])) << 2) | (((short)(data[0])) >> 6);
	y = (((short)(data[3])) << 2) | (((short)(data[2])) >> 6);
	z = (((short)(data[5])) << 2) | (((short)(data[4])) >> 6);

	pos.x = (short) (x << 6) >> 6;
	pos.y = (short) (y << 6) >> 6;
	pos.z = (short) (z << 6) >> 6;

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

int bma150_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	int retval = 0;
	//io_port_init();
	bma150_client = client;

	printk("%s\n",__func__);
	
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

int bma150_remove(struct i2c_client *client)
{
	unregister_chrdev(major, DRIVER_NAME);
	device_destroy(bma_150_class,MKDEV(major, minor));
	class_destroy(bma_150_class);
	return 0;
}

struct i2c_driver bma150_driver = {
	.probe	 = bma150_probe,
	.remove  = bma150_remove,
	.id_table = bma150_table,
	.driver = {
		.name  = "hehe",
		.owner = THIS_MODULE,
			},
};

/* Module Init & Exit function */
static int __init bma_150_init(void)
{
	return i2c_add_driver(&bma150_driver);
}

static void __exit bma_150_exit(void)
{
	i2c_del_driver(&bma150_driver);
}

module_init(bma_150_init);
module_exit(bma_150_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("sunplusedu");

