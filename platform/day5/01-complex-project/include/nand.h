#ifndef _NAND_H_
#define _NAND_H_


extern unsigned int _nand_read_id(void);
extern unsigned int  _nand_erase(unsigned int srcaddr,unsigned int size);
extern unsigned int  _nand_read(unsigned char *destaddr,unsigned int srcaddr,unsigned int size);
extern unsigned int  _nand_write(unsigned char *destaddr,unsigned int srcaddr,unsigned int size);


#endif /* _NAND_H_ */
