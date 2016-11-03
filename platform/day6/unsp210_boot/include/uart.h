
#ifndef __UART_H__
#define __UART_H__

#include "s5pc110.h"
#include "types.h"

enum ParseState {
	PS_WHITESPACE,
	PS_TOKEN,
	PS_STRING,
	PS_ESCAPE
};
enum ParseState stackedState;

#define MAX_CMDBUF_SIZE		60
#define CTL_CH(c)		((c) - 'a' + 1)

typedef void (*vfuncp)(void);

extern void _puts(const char *s);
extern int printf(const char *fmt, ...);
char waitkey(unsigned long delay, int* error_p);
void serial_term(void);



#endif /*__UART_H__*/

