#ifndef _KEY_H_
#define _KEY_H_


extern void key_init(void);
extern void op_led(int key_num);
extern void col_scan_init(void);
extern void row_scan_init(void);
extern int key_read_col(void);
extern int key_read_row(void);
extern void key_deal(int key_val);
extern void find_key(unsigned int pend_val);


#endif /*_KEY_H_*/

