#include <linux/module.h>
#include <linux/init.h>
#include <linux/i2c.h>

static struct i2c_client *bma150_client;
static struct i2c_board_info bma150_info = {	
	I2C_BOARD_INFO("bma_150",0x38),
};

static int bma150_client_init(void)
{
	struct i2c_adapter *bma150_adapter;
	bma150_adapter = i2c_get_adapter(2);
	bma150_client = i2c_new_device(bma150_adapter,&bma150_info);
	i2c_put_adapter(bma150_adapter);
	return 0;
}

static void bma150_client_exit(void)
{
	i2c_unregister_device(bma150_client);
}


module_init(bma150_client_init);
module_exit(bma150_client_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("sunplusedu");
