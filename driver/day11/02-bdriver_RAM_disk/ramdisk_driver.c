
/***************************************************************
  *Project Name:			si4702_fm  
  *File Name:				si4702_ctrl.c  
  *Description:				do not use the request queue of system
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


#define		VRD_COUNT				2
#define 	VRD_PARTITION			4
#define 	VRD_SIZE				10*1024*1024		//size is 10M
#define 	VRD_SECTOR_SIZE			512
#define		VRD_SECTOR_COUNT		(VRD_SIZE/VRD_SECTOR_SIZE)
#define 	KERNEL_SECTOR_SIZE		512			//内核认为的扇区大小固定为512字节

#define 	DRIVER_NAME		"sun_ramdisk"
static int MAJOR_NR = 0;

typedef struct ramdisk_dev{
	int major;
	char* vmem;
	unsigned long mem_size;
	unsigned long sector_size;
	unsigned long sector_count;
	struct gendisk* gdisk;
}ramdisk_dev_t;

static ramdisk_dev_t rdisk[VRD_COUNT];


//自定义数据处理接口，不使用请求队列
static int vrd_make_request(struct request_queue *q, struct bio *bio)
{
	unsigned long flag;
    struct bio_vec *bvec;
    unsigned long  i;
    
    char	*pBuffer; 
	ramdisk_dev_t *prdisk = bio->bi_bdev->bd_disk->private_data;	//得到randisk的首地址
	sector_t sector = bio->bi_sector;		//要传输的第一个扇区
	unsigned long offset;	//要传输扇区的偏移量
	unsigned long nbytes;
    
	//循环取得数据操作缓冲区信息(操作地址及访问数据大小)
    bio_for_each_segment(bvec, bio, i){
		offset = sector*KERNEL_SECTOR_SIZE;		//获取存储位置偏移量
		nbytes = bio_cur_bytes(bio);		//获取数据传输的字节数
		
		//首先判断操作的数据空间是否超出块设备容量
		if((offset + nbytes) > prdisk->mem_size){
			PRINTK("bio error!\n");
			return 0;  
		}
//		pBuffer = __bio_kmap_atomic(bio,i,KM_USER0);	//kmap映射出缓冲区实际地址
		pBuffer = bvec_kmap_irq(bvec,&flag);			//同上方法二
		//		pBuffer = kmap(bvec->bv_page) + bvec->bv_offset;	//同上方法三
		switch(bio_data_dir(bio)) {
		case READ:
			memcpy(pBuffer, prdisk->vmem+offset, nbytes); 
			break; 
		case WRITE:
			memcpy(prdisk->vmem+offset, pBuffer, nbytes); 
			break;
		}
		sector += nbytes>>9;
//		__bio_kunmap_atomic(bio, KM_USER0);		//释放映射的内存缓冲区
		bvec_kunmap_irq(bvec, &flag);			//同上方法二
//		kunmap(bvec->bv_page); 					//同上方法三
    }
	//通知处理结束
    bio_endio(bio, 0);
    return 0;			//必须返回零否则该bio会重新被提交
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
	return 0;
}
/*检测到设备变化后的响应处理函数*/
static int vrd_revalidate_disk(struct gendisk *gdisk)
{
	ramdisk_dev_t *prd;
	prd = gdisk->private_data;
	PRINTK("in revalidate disk, the device major is %d\n",prd->major);
	
	return 0;
}
/*获取磁盘分区信息，通过配置hd_geo参数，传递给内核*/
static int vrd_getgeo(struct block_device* bdev, struct hd_geometry* hd_geo)
{
	ramdisk_dev_t *prd;
	prd = bdev->bd_disk->private_data;
	PRINTK("in getgeo, the device major is %d\n",prd->major);
	//柱面数 即 没磁盘的磁道 数
	hd_geo->heads = 4;		//磁头，磁盘
	hd_geo->sectors = 16;	//每磁道扇区数
	hd_geo->start = 0;		//
	hd_geo->cylinders = prd->mem_size/hd_geo->heads/KERNEL_SECTOR_SIZE/hd_geo->sectors;//320
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
	int lp = 0;
	struct gendisk *pgdisk = NULL;
	MAJOR_NR = register_blkdev(MAJOR_NR, DRIVER_NAME);
	if(MAJOR_NR < 0){
		PRINTK("register block device fail!\n");
		return MAJOR_NR;
	}
	PRINTK("\nMAJOR_NR = %d\n",MAJOR_NR);
	
    for(lp = 0; lp < VRD_COUNT; lp++){
		rdisk[lp].major = MAJOR_NR;
		rdisk[lp].sector_size = VRD_SECTOR_SIZE;
		rdisk[lp].mem_size = VRD_SIZE;
		rdisk[lp].vmem = vmalloc(VRD_SIZE);
		rdisk[lp].sector_count = VRD_SECTOR_COUNT;
	
        rdisk[lp].gdisk = alloc_disk(VRD_PARTITION);
		pgdisk = rdisk[lp].gdisk;
		
		//设置主设备号
		pgdisk->major = rdisk[lp].major;
		pgdisk->first_minor = lp*VRD_PARTITION;
		
		//为块设备命名
		sprintf(pgdisk->disk_name,"ramdisk%c",'a'+lp); 
		
		//不使用内核默认的请求队列，请求函数
        pgdisk->queue = blk_alloc_queue(GFP_KERNEL);  //生成请求处理队列
        blk_queue_make_request(pgdisk->queue, &vrd_make_request);
		set_capacity(pgdisk, rdisk[lp].sector_count*rdisk[lp].sector_size/KERNEL_SECTOR_SIZE); //设置块设备的总扇区数
		
        //设置块设备的处理函数
        pgdisk->fops = &vrd_fops;
        
        pgdisk->private_data  = rdisk+lp; //保存自定义结构体本身
        add_disk(pgdisk);//注册块设备结构     
    }
    return 0;
    
}

static void __exit vrd_exit(void)
{
    int lp;

    for(lp = 0; lp < VRD_COUNT; lp++){
        del_gendisk(rdisk[lp].gdisk); //注销块设备结构，与add_disk()配对使用
        put_disk(rdisk[lp].gdisk); //清除gendisk结构体分配的内存，与alloc_disk()配对使用
		vfree(rdisk[lp].vmem);
    }
	unregister_blkdev(MAJOR_NR, DRIVER_NAME);
}

module_init(vrd_init);
module_exit(vrd_exit);

MODULE_LICENSE("GPL");
