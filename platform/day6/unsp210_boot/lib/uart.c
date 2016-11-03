
#include "uart.h"
#include <stdarg.h>//包含有va_list的定义
#include <stdlib.h>

#define _U	0x01	/* upper */
#define _L	0x02	/* lower */
#define _D	0x04	/* digit */
#define _C	0x08	/* cntrl */
#define _P	0x10	/* punct */
#define _S	0x20	/* white space (space/lf/tab) */
#define _X	0x40	/* hex digit */
#define _SP	0x80	/* hard space (0x20) */


static unsigned char _ctype[];

#define __ismask(x) (_ctype[(int)(unsigned char)(x)])
#define isxdigit(c)	((__ismask(c)&(_D|_X)) != 0)
#define isdigit(c)	((__ismask(c)&(_D)) != 0)
#define islower(c)	((__ismask(c)&(_L)) != 0)


static unsigned char _ctype[] = {
_C,_C,_C,_C,_C,_C,_C,_C,			/* 0-7 */
_C,_C|_S,_C|_S,_C|_S,_C|_S,_C|_S,_C,_C,		/* 8-15 */
_C,_C,_C,_C,_C,_C,_C,_C,			/* 16-23 */
_C,_C,_C,_C,_C,_C,_C,_C,			/* 24-31 */
_S|_SP,_P,_P,_P,_P,_P,_P,_P,			/* 32-39 */
_P,_P,_P,_P,_P,_P,_P,_P,			/* 40-47 */
_D,_D,_D,_D,_D,_D,_D,_D,			/* 48-55 */
_D,_D,_P,_P,_P,_P,_P,_P,			/* 56-63 */
_P,_U|_X,_U|_X,_U|_X,_U|_X,_U|_X,_U|_X,_U,	/* 64-71 */
_U,_U,_U,_U,_U,_U,_U,_U,			/* 72-79 */
_U,_U,_U,_U,_U,_U,_U,_U,			/* 80-87 */
_U,_U,_U,_P,_P,_P,_P,_P,			/* 88-95 */
_P,_L|_X,_L|_X,_L|_X,_L|_X,_L|_X,_L|_X,_L,	/* 96-103 */
_L,_L,_L,_L,_L,_L,_L,_L,			/* 104-111 */
_L,_L,_L,_L,_L,_L,_L,_L,			/* 112-119 */
_L,_L,_L,_P,_P,_P,_P,_C,			/* 120-127 */
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		/* 128-143 */
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		/* 144-159 */
_S|_SP,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,   /* 160-175 */
_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,_P,       /* 176-191 */
_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,_U,       /* 192-207 */
_U,_U,_U,_U,_U,_U,_U,_P,_U,_U,_U,_U,_U,_U,_U,_L,       /* 208-223 */
_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,_L,       /* 224-239 */
_L,_L,_L,_L,_L,_L,_L,_P,_L,_L,_L,_L,_L,_L,_L,_L};      /* 240-255 */


void _putc(char c)
{
	S5PC11X_UART *uart = (S5PC11X_UART *)(ELFIN_UART_CONSOLE_BASE);

	/* wait for room in the tx FIFO */
	while (!(uart->UTRSTAT & (0x01<<2)));

	uart->UTXH = c;

	/* If \n, also do \r */
	if (c == '\n')
		_putc('\r');
}

int serial_getc(void)
{
	S5PC11X_UART *const uart = (S5PC11X_UART *)(ELFIN_UART_CONSOLE_BASE);

	/* wait for character to arrive */
	while (!(uart->UTRSTAT & 0x1));

	return uart->URXH & 0xff;
}

void _puts(const char *s)
{
	while (*s) {
		_putc(*s++);
	}
}

void putnstr(const char *str, int n)
{
	if (str == NULL)
		return;

	while (n && *str != '\0') {
		_putc(*str);
		str++;
		n--;
	}
}

void putstr(const char *str)
{
	putnstr(str, strlen(str));
}


#if 1
#define ZEROPAD	1		/* pad with zero */
#define SIGN	2		/* unsigned/signed long */
#define PLUS	4		/* show plus */
#define SPACE	8		/* space if plus */
#define LEFT	16		/* left justified */
#define SPECIAL	32		/* 0x */
#define LARGE	64		/* use 'ABCDEF' instead of 'abcdef' */

/* We're not 64-bit, but... */
#define do_div(n,base)						\
({								\
	int __res;						\
	__res = ((unsigned long)n) % (unsigned int)base;	\
	n = ((unsigned long)n) / (unsigned int)base;		\
	__res;							\
})

size_t strnlen(const char * s, size_t count)
{
	const char *sc;

	for (sc = s; count-- && *sc != '\0'; ++sc)
		/* nothing */;
	return sc - s; 
}

static int skip_atoi(const char **s)
{
	int i=0;

	while (isdigit(**s))
		i = i*10 + *((*s)++) - '0';
	return i;
}

static char * number(char * buf, char * end, long long num, int base, int size, int precision, int type)
{
	char c,sign,tmp[66];
	const char *digits;
	const char small_digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";
	const char large_digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	int i;

	digits = (type & LARGE) ? large_digits : small_digits;
	if (type & LEFT)
		type &= ~ZEROPAD;
	if (base < 2 || base > 36)
		return 0;
	c = (type & ZEROPAD) ? '0' : ' ';
	sign = 0;
	if (type & SIGN) {
		if (num < 0) {
			sign = '-';
			num = -num;
			size--;
		} else if (type & PLUS) {
			sign = '+';
			size--;
		} else if (type & SPACE) {
			sign = ' ';
			size--;
		}
	}
	if (type & SPECIAL) {
		if (base == 16)
			size -= 2;
		else if (base == 8)
			size--;
	}
	i = 0;
	if (num == 0)
		tmp[i++]='0';
	else while (num != 0)
		tmp[i++] = digits[do_div(num,base)];
	if (i > precision)
		precision = i;
	size -= precision;
	if (!(type&(ZEROPAD+LEFT))) {
		while(size-->0) {
			if (buf <= end)
				*buf = ' ';
			++buf;
		}
	}
	if (sign) {
		if (buf <= end)
			*buf = sign;
		++buf;
	}
	if (type & SPECIAL) {
		if (base==8) {
			if (buf <= end)
				*buf = '0';
			++buf;
		} else if (base==16) {
			if (buf <= end)
				*buf = '0';
			++buf;
			if (buf <= end)
				*buf = digits[33];
			++buf;
		}
	}
	if (!(type & LEFT)) {
		while (size-- > 0) {
			if (buf <= end)
				*buf = c;
			++buf;
		}
	}
	while (i < precision--) {
		if (buf <= end)
			*buf = '0';
		++buf;
	}
	while (i-- > 0) {
		if (buf <= end)
			*buf = tmp[i];
		++buf;
	}
	while (size-- > 0) {
		if (buf <= end)
			*buf = ' ';
		++buf;
	}
	return buf;
}

int vsnprintf(char *buf, size_t size, const char *fmt, va_list args)
{
	int len;
	unsigned long long num;
	int i, base;
	char *str, *end, c;
	const char *s;

	int flags;		/* flags to number() */

	int field_width;	/* width of output field */
	int precision;		/* min. # of digits for integers; max
				   number of chars for from string */
	int qualifier;		/* 'h', 'l', or 'L' for integer fields */
				/* 'z' support added 23/7/1999 S.H.    */
				/* 'z' changed to 'Z' --davidm 1/25/99 */

	str = buf;
	end = buf + size - 1;

	if (end < buf - 1) {
		end = ((void *) -1);
		size = end - buf + 1;
	}

	for (; *fmt ; ++fmt) {
		if (*fmt != '%') {
			if (str <= end)
				*str = *fmt;
			++str;
			continue;
		}

		/* process flags */
		flags = 0;
		repeat:
			++fmt;		/* this also skips first '%' */
			switch (*fmt) {
				case '-': flags |= LEFT; goto repeat;
				case '+': flags |= PLUS; goto repeat;
				case ' ': flags |= SPACE; goto repeat;
				case '#': flags |= SPECIAL; goto repeat;
				case '0': flags |= ZEROPAD; goto repeat;
			}

		/* get field width */
		field_width = -1;
		if (isdigit(*fmt))
			field_width = skip_atoi(&fmt);
		else if (*fmt == '*') {
			++fmt;
			/* it's the next argument */
			field_width = va_arg(args, int);
			if (field_width < 0) {
				field_width = -field_width;
				flags |= LEFT;
			}
		}

		/* get the precision */
		precision = -1;
		if (*fmt == '.') {
			++fmt;	
			if (isdigit(*fmt))
				precision = skip_atoi(&fmt);
			else if (*fmt == '*') {
				++fmt;
				/* it's the next argument */
				precision = va_arg(args, int);
			}
			if (precision < 0)
				precision = 0;
		}

		/* get the conversion qualifier */
		qualifier = -1;
		if (*fmt == 'h' || *fmt == 'l' || *fmt == 'L' || *fmt =='Z') {
			qualifier = *fmt;
			++fmt;
			if (qualifier == 'l' && *fmt == 'l') {
				qualifier = 'L';
				++fmt;
			}
		}

		/* default base */
		base = 10;

		switch (*fmt) {
			case 'c':
				if (!(flags & LEFT)) {
					while (--field_width > 0) {
						if (str <= end)
							*str = ' ';
						++str;
					}
				}
				c = (unsigned int) va_arg(args, int);
				if (str <= end)
					*str = c;
				++str;
				while (--field_width > 0) {
					if (str <= end)
						*str = ' ';
					++str;
				}
				continue;

			case 's':
				s = va_arg(args, char *);
				if (!s)
					s = "<NULL>";

				len = strnlen(s, precision);

				if (!(flags & LEFT)) {
					while (len < field_width--) {
						if (str <= end)
							*str = ' ';
						++str;
					}
				}
				for (i = 0; i < len; ++i) {
					if (str <= end)
						*str = *s;
					++str; ++s;
				}
				while (len < field_width--) {
					if (str <= end)
						*str = ' ';
					++str;
				}
				continue;

			case 'p':
				if (field_width == -1) {
					field_width = 2*sizeof(void *);
					flags |= ZEROPAD;
				}
				str = number(str, end,
						(unsigned long) va_arg(args, void *),
						16, field_width, precision, flags);
				continue;


			case 'n':
				/* FIXME:
				* What does C99 say about the overflow case here? */
				if (qualifier == 'l') {
					long * ip = va_arg(args, long *);
					*ip = (str - buf);
				} else if (qualifier == 'Z') {
					size_t * ip = va_arg(args, size_t *);
					*ip = (str - buf);
				} else {
					int * ip = va_arg(args, int *);
					*ip = (str - buf);
				}
				continue;

			case '%':
				if (str <= end)
					*str = '%';
				++str;
				continue;

				/* integer number formats - set up the flags and "break" */
			case 'o':
				base = 8;
				break;

			case 'X':
				flags |= LARGE;
			case 'x':
				base = 16;
				break;

			case 'd':
			case 'i':
				flags |= SIGN;
			case 'u':
				break;

			default:
				if (str <= end)
					*str = '%';
				++str;
				if (*fmt) {
					if (str <= end)
						*str = *fmt;
					++str;
				} else {
					--fmt;
				}
				continue;
		}
		if (qualifier == 'L')
			num = va_arg(args, long long);
		else if (qualifier == 'l') {
			num = va_arg(args, unsigned long);
			if (flags & SIGN)
				num = (signed long) num;
		} else if (qualifier == 'Z') {
			num = va_arg(args, size_t);
		} else if (qualifier == 'h') {
			num = (unsigned short) va_arg(args, int);
			if (flags & SIGN)
				num = (signed short) num;
		} else {
			num = va_arg(args, unsigned int);
			if (flags & SIGN)
				num = (signed int) num;
		}
		str = number(str, end, num, base,
				field_width, precision, flags);
	}
	if (str <= end)
		*str = '\0';
	else if (size > 0)
		/* don't write out a null byte if the buf size is zero */
		*end = '\0';
	/* the trailing null byte doesn't count towards the total
	* ++str;
	*/
	return str-buf;
}



int getc_errno = 0;
char do_getc(vfuncp idler, unsigned long timeout, int *statp)
{
	unsigned char	c, rxstat;
	int	do_timeout = timeout != 0;
	S5PC11X_UART *const uart = (S5PC11X_UART *)(ELFIN_UART_CONSOLE_BASE);

	getc_errno = 0;	/* reste errno */

	while(!(uart->UTRSTAT & 0x1)) {

		if (do_timeout) {
			if (!timeout)
				break;
			timeout--;
		}

		if (idler)
			idler();
	}

	if (do_timeout && timeout == 0) {
		c = 0;
		rxstat = -1;
	} else {
		c = uart->URXH & 0xff;
		rxstat = (uart->UERSTAT & UART_ERR_MASK);
	}

	if (rxstat) {
		getc_errno = rxstat;
		/*printk("RXSTAT error. status = 0x%08lx", rxstat);*/
		if (statp)
			*statp = rxstat;
	}
	return (c);
}

char waitkey(unsigned long delay, int* error_p)
{
	return (do_getc((vfuncp)NULL, delay, error_p));
}


void getcmd(char *cmd_buf, unsigned int len)
{
	int curpos = 0;	/* current position - index into cmd_buf */
	char c;
	int cmd_echo = 1;

	/* Clear out the buffer */
	memset(cmd_buf, 0, MAX_CMDBUF_SIZE);

	for (;;) {
		c = serial_getc();
		switch (c) {
		case 0x08:
		case 0x06:
		case 0x07:
		case 0x7E:
		case 0x7F:	/* backspace or delete */
			/* we're not at the beginning of the line */
			if (curpos) {
				curpos--;
				_putc(0x08);	/* go backwards */
				_putc(' ');	/* overwrite the char */
				_putc(0x08);	/* go back again */
			}
			cmd_buf[curpos] = '\0';
			break;
		case '\r':
		case '\n':
		case '\0':
			_putc('\r');
			_putc('\n');
			goto end_cmd;
		case CTL_CH('x'):
			curpos = 0;
			break;

		default:
			if (curpos < MAX_CMDBUF_SIZE) {
				cmd_buf[curpos] = c;
				/* echo it back out to the screen */
				if (cmd_echo)
					_putc(c);
				curpos++;
			}
			break;
		}
	}
end_cmd:
	//putstr(cmd_buf);
	return;
}

void parseargs(char *argstr, int *argc_p, char **argv, char** resid)
{
	int argc = 0;
	char c;
	enum ParseState lastState = PS_WHITESPACE;

	/* tokenize the argstr */
	while ((c = *argstr) != 0) {
		enum ParseState newState;

		if (c == ';' && lastState != PS_STRING && lastState != PS_ESCAPE)
			break;

		if (lastState == PS_ESCAPE) {
			newState = stackedState;
		} else if (lastState == PS_STRING) {
			if (c == '"') {
				newState = PS_WHITESPACE;
				*argstr = 0;
			} else {
				newState = PS_STRING;
			}
		} else if ((c == ' ') || (c == '\t')) {
			/* whitespace character */
			*argstr = 0;
			newState = PS_WHITESPACE;
		} else if (c == '"') {
			newState = PS_STRING;
			*argstr++ = 0;
			argv[argc++] = argstr;
		} else if (c == '\\') {
			stackedState = lastState;
			newState = PS_ESCAPE;
		} else {
			/* token */
			if (lastState == PS_WHITESPACE) {
				argv[argc++] = argstr;
			}
			newState = PS_TOKEN;
		}

		lastState = newState;
		argstr++;
	}
	argv[argc] = NULL;
	if (argc_p != NULL)
		*argc_p = argc;

	if (*argstr == ';') {
		*argstr++ = '\0';
	}
	*resid = argstr;
}

static inline unsigned char __toupper(unsigned char c)
{
	if (islower(c))
		c -= 'a'-'A';
	return c;
}

unsigned long simple_strtoul(const char *cp,char **endp,unsigned int base)
{
	unsigned long result = 0,value;

	if (*cp == '0') {
		cp++;
		if ((*cp == 'x') && isxdigit(cp[1])) {
			base = 16;
			cp++;
		}
		if (!base) {
			base = 8;
		}
	}
	if (!base) {
		base = 10;
	}
	while (isxdigit(*cp) && (value = isdigit(*cp) ? *cp-'0' : (islower(*cp)
	    ? __toupper(*cp) : *cp)-'A'+10) < base) {
		result = result*base + value;
		cp++;
	}
	if (endp)
		*endp = (char *)cp;
	return result;
}

extern void my_boot(void);
void execcmd(int argc, const char **argv)
{
	unsigned int reg,val;
	if (strncmp("boot", argv[0], strlen(argv[0])) == 0)
	{
		my_boot();
		return;
	}
	else if (strncmp("reset", argv[0], strlen(argv[0])) == 0)
	{
		putstr("reset ......\r\n");
		SW_RST_REG = 0x01;//软复位
		return;
	}
	else if ((strncmp("md", argv[0], strlen(argv[0])) == 0) && (argc == 2))
	{
		
		reg = simple_strtoul(argv[1], NULL, 16);
		printf("\n reg = %#x\n",*((unsigned int *)reg));		
		return;
	}
	else if ((strncmp("mw", argv[0], strlen(argv[0])) == 0) && (argc == 3))
	{
		
		reg = simple_strtoul(argv[1], NULL, 16);
		val = simple_strtoul(argv[2], NULL, 16);
		*((unsigned int *)reg) = val;
		printf("\n reg:%#x = %#x\n",*((unsigned int *)reg),val);		
		return;
	}
	else if (strncmp("print", argv[0], strlen(argv[0])) == 0)
	{
		putstr("test\r\n");
		return;
	}
	else
	{
		putstr("command error!\r\n");
		return;
	}
}

void exec_string(char *buf)
{
	int argc;
	char *argv[128];
	char *resid;

	while (*buf) 
	{
		memset(argv, 0, sizeof(argv));
		parseargs(buf, &argc, argv, &resid);
		if (argc > 0)
			execcmd(argc, (const char **)argv);
		buf = resid;
	}
}

void serial_term(void)
{
	char cmd_buf[MAX_CMDBUF_SIZE];

	for (;;) {
		putstr("unsp210 # ");

		getcmd(cmd_buf, MAX_CMDBUF_SIZE);

		/* execute a user command */
		if (cmd_buf[0])
			exec_string(cmd_buf);
	}
}





















int printf(const char *fmt, ...)
{
	va_list args;
	int printed_len;
	static char printk_buf[1024];

	/* Emit the output into the temporary buffer */
	va_start(args, fmt);
	printed_len = vsnprintf(printk_buf, sizeof(printk_buf), fmt, args);
	va_end(args);

	 _puts(printk_buf);

	return printed_len;
}

/*
由于不能使用除法，所以加上
/usr/local/arm/4.3.2/lib/gcc/arm-none-linux-gnueabi/4.3.2/armv4t下面的库
主要是libgcc.a，再加上下面的raise函数就可以使用了
*/
int raise(int a)
{
	return 0;
}


#endif
