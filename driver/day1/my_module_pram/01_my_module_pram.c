#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>


static int num_test = 827;/*8月27号*/

/*module add*/
static int mymodule_init(void)
{
	printk("I am coming!!!\n");
	printk("num_test = %d\n",num_test);
	return 0;
}

/*module exit*/
static void mymodule_exit(void)
{
	printk("goodbye I am leaving!!!\n");
	return ;
}

/*num_test to recv the parmters*/
module_param(num_test, int, S_IRUGO);

/*define module add*/
module_init(mymodule_init);


/*define module exit*/
module_exit(mymodule_exit);


/*define the module author*/
MODULE_AUTHOR("Mr.G");


/*define the module permission*/
MODULE_LICENSE("GPL");



