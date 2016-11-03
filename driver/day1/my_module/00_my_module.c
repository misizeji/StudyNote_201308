#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>


/*module add*/
static int mymodule_init(void)
{
	printk("I am coming!!!\n");
	return 0;
}

/*module exit*/
static void mymodule_exit(void)
{
	printk("goodbye I am leaving!!!\n");
	return ;
}

/*define module add*/
module_init(mymodule_init);


/*define module exit*/
module_exit(mymodule_exit);


/*define the module author*/
MODULE_AUTHOR("Mr.G");


/*define the module permission*/
MODULE_LICENSE("GPL");









