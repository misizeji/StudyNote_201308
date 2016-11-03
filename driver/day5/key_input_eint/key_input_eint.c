#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/device.h>
#include <asm/uaccess.h>
#include <linux/irq.h>
#include <linux/ioport.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/err.h>
#include <linux/platform_device.h> 
#include <linux/wait.h>
#include <linux/input.h>
#include <linux/gpio.h>
#include <mach/regs-gpio.h>
#include <linux/interrupt.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <mach/gpio.h>
#include <linux/spinlock.h>
#include <linux/sched.h>

#define EINT_DEVICE_ID			1

#define GPH3CON					(unsigned long)(S5PV210_GPH3_BASE+ 0x00)
#define GPH3DAT					(unsigned long)(S5PV210_GPH3_BASE + 0x04)
#define GPH2UP					(unsigned long)(S5PV210_GPH2_BASE + 0x08)


/*定义一个input_dev指针*/
static struct input_dev *input_key;

/* 中断服务函数
 * 在这里上报按键
 */
static irqreturn_t buttons_interrupt(int irq, void *dev)
{
	input_report_key(input_key,KEY_F4,1);
	input_report_key(input_key,KEY_F4,0);
	input_sync(input_key);
	return IRQ_HANDLED;
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

static int key_input_init(void)
{
	/*io初始化*/
	key_io_port_init();
	/*申请中断*/
	set_irq_type(IRQ_EINT(20),IRQ_TYPE_EDGE_FALLING);
	/*设置中断触发方式*/
	request_irq(IRQ_EINT(20), buttons_interrupt, IRQF_DISABLED, "KEY1", (void *)EINT_DEVICE_ID);
	/*初始化input_dev*/
	input_key = input_allocate_device();
	input_key->name = "heihei";
	/*设置input_dev*/
	input_key->evbit[0] = BIT_MASK(EV_SYN) | BIT_MASK(EV_ABS);
	set_bit(KEY_F4,input_key->keybit);
	/*注册input_dev*/
	input_register_device(input_key);
	return 0;

}

static void key_input_exit(void)
{
	/*释放中断*/
	free_irq(IRQ_EINT(20), (void *)EINT_DEVICE_ID);
	/*注销input_dev*/
	input_unregister_device(input_key);
	return;

}

module_init(key_input_init);
module_exit(key_input_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mr.G");


