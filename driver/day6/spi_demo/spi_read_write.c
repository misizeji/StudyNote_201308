
#define CLK   S5PV210_GPIO	/*时钟信号*/
#define CS    S5PV210_GP	/*片选信号*/
#define MOSI  S5PV210_GP	/*数据输出*/
#define MISO  S5PV210_GP	/*数据输入*/

#define CLR_BIT(bit) gpio_set_value(bit,0)
#define SET_BIT(bit) gpio_set_value(bit,1)

/*spi初始化*/
static void spi_init(void)
{
	CLR_BIT(CLK);
	SET_BIT(CS);
	
	gpio_set_value(CLK,0);
	gpio_set_value(CS,1);
}


/*数据的发送*/
static void spi_send_byte(unsigned byte)
{
	int i;
	CLR_BIT(CS);

	for(i=0; i<8; i++)
	{
		if(byte & 0x80)
			SET_BIT(MOSI);/*准备数据 高电平*/
		else
			CLR_BIT(MOSI);/*准备数据 低电平*/
		udelay(50);
		SET_BIT(CLK);/*上升沿时钟*/
		udelay(50);/*接收者，自动接收数据，由硬件固化产生*/
		
		CLR_BIT(CLK);/*为下一次数据收发做准备*/
		udelay(50);
		byte = byte << 1;
	}
	SET_BIT(CS);
}

/*数据的接收*/
static unsigned char spi_recv_byte(void)
{
	int i;
	unsigned char byte = 0;
	CLR_BIT(CS);
	for(i=0; i<8; i++)
	{
		byte = byte << 1;
		udelay(50);/*让对方准备数据*/
		SET_BIT(CLK);
		byte |= gpio_get_value(MISO);
		udelay(50);
		CLR_BIT(CLK);
	}
	SET_BIT(CS);
	return byte;	
}


