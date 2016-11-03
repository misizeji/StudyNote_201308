
/***************************************************************
  *Project Name:			ramdisk_driver  
  *File Name:				ramdisk_driver.c  
  *Description:				use the request queue of system
  *
  *Date and Edition:		2012-11-02  v1.1
  *Author:					Valor Lion
  **************************************************************/

#include <linux/module.h>    /*module_init()*/
#include <linux/kernel.h>	/* printk() */
#include <linux/init.h>		/* __init __exit */
#include <linux/fs.h>		/* file_operation */
#include <asm/uaccess.h>	/* copy_to_user, copy_from_user */
#include <linux/device.h>  /*class ,class_create ,device_create 等*/
#include <linux/errno.h>	/* Error number */
#include <linux/delay.h>	/* mdelay ,ndelay*/
#include <asm/delay.h>     /* udelay */
#include <linux/spinlock.h>

#include <linux/blkdev.h>
#include <linux/hdreg.h>	/* geo */
#include <linux/blkpg.h>
#include <linux/vmalloc.h>

#define DEBUG			//open debug message
#ifdef DEBUG
#define PRINTK(fmt, arg...)		printk(KERN_WARNING fmt, ##arg)
#else
#define PRINTK(fmt, arg...)		printk(KERN_DEBUG fmt, ##arg)
#endif


#define	VRD_COUNT				2
#define 	VRD_PARTITION			4
#define 	VRD_SIZE				10*1024*1024		//size is 10M
#define 	VRD_SECTOR_SIZE			512
#define	VRD_SECTOR_COUNT		(VRD_SIZE/VRD_SECTOR_SIZE)
#define 	KERNEL_SECTOR_SIZE		512			//内核认为的扇区大小固定为512字节

#define 	DRIVER_NAME		"sun_ramdisk"

typedef struct ramdisk_dev{
	int major;
	char* vmem;
	unsigned long mem_size;
	unsigned long sector_size;
	unsigned long sector_count;
	spinlock_t splock;
	struct gendisk* gdisk;
}ramdisk_dev_t;

static ramdisk_dev_t rdisk;

/*请求处理函数，用来处理I/O调度层处理后的request*/
void vrd_request(struct request_queue *q)
{
	struct request *req;
	ramdisk_dev_t *prdisk;
	sector_t start;
	unsigned long offset;
	unsigned long size;
	
	req = blk_fetch_request(q);			//提取一个request请求
	while(req){
		start = blk_rq_pos(req);		//获取读写操作的起始扇区
		prdisk = req->rq_disk->private_data;	//获取用户私有数据
		
		offset = start*KERNEL_SECTOR_SIZE;		//通过起始扇区获取ramdisk的偏移地址
		size = blk_rq_cur_bytes(req);		//获取数据读写的大小
		
		switch(rq_data_dir(req)){		//计算数据请求的方向
			case READ:
				memcpy(req->buffer, prdisk->vmem+offset, size);
			break;
			case WRITE:
				memcpy(prdisk->vmem+offset, req->buffer, size);
			break;
		}
		
		if(!__blk_end_request_cur(req,0))		//通知内核请求处理结束
			req = blk_fetch_request(q);			//获取下一个request请求
	}
}

static int vrd_open(struct block_device *bdev, fmode_t mod)
{
	ramdisk_dev_t *prd;
	prd = bdev->bd_disk->private_data;
	PRINTK("in open, the device major is %d\n",prd->major);
	
    return 0;
}

static int vrd_release (struct gendisk *gdisk, fmode_t mod)
{
	ramdisk_dev_t *prd;
	prd = gdisk->private_data;
	PRINTK("in realse, the device major is %d\n",prd->major);
    return 0;
}

static int vrd_ioctl(struct block_device *bdev, fmode_t mod, unsigned int cmd,unsigned long arg)
{
	ramdisk_dev_t *prd;
	prd = bdev->bd_disk->private_data;
	PRINTK("in ioctl, the device major is %d\n", prd->major);
	
    return -ENOTTY;
}

/*检测设备的变化，例如用户打开光驱，更换了一张光盘*/
static int vrd_media_changed(struct gendisk *gdisk)
{
	ramdisk_dev_t *prd;
	prd = gdisk->private_data;
	PRINTK("in media changed, the device major is %d\n",prd->major);
	//added here
	
	return 0;
}
/*检测到设备变化后的响应处理函数*/
static int vrd_revalidate_disk(struct gendisk *gdisk)
{
	ramdisk_dev_t *prd;
	prd = gdisk->private_data;
	PRINTK("in revalidate disk, the device major is %d\n",prd->major);
	//added here
	
	return 0;
}

/*获取磁盘分区信息，通过配置hd_geo参数，传递给内核*/
static int vrd_getgeo(struct block_device* bdev, struct hd_geometry* hd_geo)
{
	ramdisk_dev_t *prd;
	prd = bdev->bd_disk->private_data;
	PRINTK("in getgeo, the device major is %d\n",prd->major);
	
	hd_geo->cylinders = (prd->mem_size)/KERNEL_SECTOR_SIZE >> 6;
	hd_geo->heads = 4;
	hd_geo->sectors = 16;
	hd_geo->start = 0;
	return 0;
}

static struct block_device_operations vrd_fops =
{
    .owner   = THIS_MODULE,    
    .open    = vrd_open,
    .release = vrd_release,  
    .ioctl   = vrd_ioctl,
	.media_changed = vrd_media_changed,
	.revalidate_disk = vrd_revalidate_disk,
	.getgeo = vrd_getgeo,
};

static int __init vrd_init(void)
{
	struct gendisk *pgdisk = NULL;
	
	rdisk.major= register_blkdev(0, DRIVER_NAME);
	if(rdisk.major < 0){
		PRINTK("register block device fail!\n");
		return rdisk.major;
	}
	PRINTK("\nmajor = %d\n",rdisk.major);
	
	rdisk.sector_size = VRD_SECTOR_SIZE;
	rdisk.mem_size = VRD_SIZE;
	rdisk.vmem = vmalloc(VRD_SIZE);
	rdisk.sector_count = VRD_SECTOR_COUNT;
	spin_lock_init(&rdisk.splock);			//init the spin lock

    	rdisk.gdisk = alloc_disk(VRD_PARTITION);
	pgdisk = rdisk.gdisk;
	
	//设置主设备号
	pgdisk->major = rdisk.major;
	pgdisk->first_minor = 0;
	
	//为块设备命名
	sprintf(pgdisk->disk_name,"ramdisk%c",'a'); 
	
	//初始化请求队列，指定请求处理函数
	pgdisk->queue = blk_init_queue(vrd_request,&rdisk.splock);

	set_capacity(pgdisk, rdisk.sector_count*rdisk.sector_size/KERNEL_SECTOR_SIZE); //设置块设备的总扇区数
	
    	//设置块设备的处理函数
    	pgdisk->fops = &vrd_fops;
    
    	pgdisk->private_data  = &rdisk; //保存自定义结构体本身
    	add_disk(pgdisk);//注册块设备结构    类似device_create

    return 0;
    
}

static void __exit vrd_exit(void)
{

        del_gendisk(rdisk.gdisk); //注销块设备结构，与add_disk()配对使用
        put_disk(rdisk.gdisk); //清除gendisk结构体分配的内存，与alloc_disk()配对使用
	vfree(rdisk.vmem);

	unregister_blkdev(rdisk.major, DRIVER_NAME);
}

module_init(vrd_init);
module_exit(vrd_exit);

MODULE_LICENSE("GPL");
