一、引脚
	SHUTDOWN  //模式切换引脚
	INT  //中断管脚
	SCL  //i2c 时钟
	SDA  //i2c 数据
二、工作原理
	按下触摸屏 产生脉冲信号 可以使用中断（上升沿or下降沿）
三、寄存器
	调用函数读取坐标值
	
dev.c(注册client 三要素：地址 名字 adapter)
	1.底板的J22模块与电容屏连接(找到adapter序号)
	2.地址 技术手册有

driver.c
	凡是这个struct goodix_priv的变量都必须是局部变量

	/* init work of workqueue*/
	初始化工作（工作队列已经初始化）
	/* input dev init and register */
	注册input
	重点：填充input_dev（tp_input.c）
	/* request_irq */
	中断号是INT_FUNC(.h文件里面）
	/* init hardware*/
	本质就是对goodix电容屏芯片寄存器配置
	使用goodix_init_panel()函数初始化即可
	
	goodix_ts_work_func
		goodix_read_panel //读取寄存器的原始数据
			uint8_t  point_data[35]={ 0 };
			第一个字节表示寄存器的地址（也就是0）
			剩下的34字节才是真正存放的地方
		goodix_analysis_data //分析寄存器原始数据 得到真正的坐标信息
			得到坐标信息存放于pos（已经完成）
		


	
	