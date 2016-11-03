#include "config.h"


#ifdef FUNC_NAND_FLASH


void _nand_init(void)
{
//设置第2pin为nand片选信号
	MP01CON_REG &= ~((0xf<<8)); 
	MP01CON_REG |= (0x3<<8); 
//设置第2pin为禁止上下拉	
	MP01PUD_REG &= ~((0x3<<4)); 
//配置nand控制引脚并禁止上下拉	
	MP03CON_REG &= ~(0xFFFFFF);
	MP03CON_REG = 0x22222222;
	MP03PUD_REG &= ~0x3fff;

//数据线MP06无须初始化，因为默认就是用于nand数据接口	

	NFCONF_REG &=  ~0x777F;
	NFCONF_REG |= NFCONF_VAL;
	NFCONT_REG &= ~0x707C7;
	NFCONT_REG |= NFCONT_VAL;
	NFCONF_REG |= ((0x70)|(0x7700));
	NFCONT_REG |= 0x03;
}

unsigned int _nand_read_id(void)
{
	vu_char id[4];
	unsigned int nand_id = 0;
	ulong i,code;

	NAND_ENABLE_CE();
	NFCMD_REG = NAND_CMD_READID;
	NFADDR_REG =  0x00;

	/* wait for a while */
	for (i=0; i<200; i++);
	code = NFDATA8_REG;
	id[0] = NFDATA8_REG;
	id[1] = NFDATA8_REG;
	id[2] = NFDATA8_REG;
	id[3] = NFDATA8_REG;

	nand_id = id[3];
	nand_id |= (id[2]<<8);
	nand_id |= (id[1]<<16);	
	nand_id |= (id[0]<<24);
	NAND_DISABLE_CE();

	//printf("\nnand code:%#x\n",(int)code);
	
	return nand_id;
	
}

static void nand_readpage (ulong col, ulong row, uchar* buf, int len)
{
	int i;

	NAND_ENABLE_CE();

	NFCMD_REG = NAND_CMD_READ0;

	/* Write Column Address */
	NFADDR_REG = (col) & 0xff;
	NFADDR_REG = (col >> 8) & 0xff;

	/* Write Row Address */
	NFADDR_REG = (row) & 0xff;
	NFADDR_REG = (row >> 8) & 0xff;
	NFADDR_REG = (row >> 16) & 0xff;

	NFCMD_REG = NAND_CMD_READSTART;

	NF_TRANSRnB();
	
	for (i = 0; i < len; i++) {
		buf[i] = NFDATA8_REG;
	}

	NAND_DISABLE_CE();
}

static void nand_writepage (ulong col, ulong row, uchar* buf, int len)
{
	int i;

	NAND_ENABLE_CE();

	NFCMD_REG = NAND_CMD_SEQIN;

	/* Write Column Address */
	NFADDR_REG = (col) & 0xff;
	NFADDR_REG = (col >> 8) & 0xff;

	/* Write Row Address */
	NFADDR_REG = (row) & 0xff;
	NFADDR_REG = (row >> 8) & 0xff;
	NFADDR_REG = (row >> 16) & 0xff;


	for (i = 0; i < len; i++) {
		NFDATA8_REG = buf[i];
	}

	NFCMD_REG = NAND_CMD_PAGEPROG;

	NF_TRANSRnB();
	
	NAND_DISABLE_CE();
}

static void _nand_eraseblock (ulong row)
{
	NAND_ENABLE_CE();

	NFCMD_REG = NAND_CMD_ERASE1;

	/* Write Row Address */
	NFADDR_REG = (row) & 0xff;
	NFADDR_REG = (row >> 8) & 0xff;
	NFADDR_REG = (row >> 16) & 0xff;

	NFCMD_REG = NAND_CMD_ERASE2;

	NF_TRANSRnB();

	NAND_DISABLE_CE();
}

unsigned int  _nand_erase(unsigned int srcaddr,unsigned int size)
{
	if(size > NAND_FLASH_BLOCK_SIZE)
		return -1;
		
	_nand_eraseblock(srcaddr/NAND_FLASH_PAGE_SIZE);

	return size;	
}

unsigned int  _nand_read(unsigned char *destaddr,unsigned int srcaddr,unsigned int size)
{
	if(size > NAND_FLASH_PAGE_SIZE)
		return -1;

	nand_readpage(srcaddr%NAND_FLASH_PAGE_SIZE, 
					srcaddr/2048, 
					destaddr, 
					size);

	return size;	
}

unsigned int  _nand_write(unsigned char *destaddr,unsigned int srcaddr,unsigned int size)
{
	if(size > NAND_FLASH_PAGE_SIZE)
		return -1;
	
	nand_writepage(srcaddr%NAND_FLASH_PAGE_SIZE, 
					srcaddr/NAND_FLASH_PAGE_SIZE, 
					destaddr, 
					size);

	return size;	
}

#endif

