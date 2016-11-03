DRIVERS-y :=
DRIVERS-y += init/main_init.o
DRIVERS-y += drivers/uart/uart_driver.o
DRIVERS-y += drivers/timer/clockmanage.o
DRIVERS-y += drivers/led/led_driver.o


objs := $(DRIVERS-y)

TOPDIR	:= $(shell /bin/pwd)
CROSSTOOLDIR := /usr/local/arm/4.3.2
CROSS_COMPILE   = $(CROSSTOOLDIR)/bin/arm-linux-
CC          = $(CROSS_COMPILE)gcc
LD          = $(CROSS_COMPILE)ld
OBJCOPY     = $(CROSS_COMPILE)objcopy
OBJDUMP     = $(CROSS_COMPILE)objdump
NM		     = $(CROSS_COMPILE)nm

DIR_INCLUDE=-I$(TOPDIR)/include
ARM_GCC_LIBS	= $(CROSSTOOLDIR)/arm-none-linux-gnueabi/libc/armv4t/usr/lib
CLIBS = -L$(ARM_GCC_LIBS) -lc -L$(CROSSTOOLDIR)/lib/gcc/arm-none-linux-gnueabi/4.3.2/armv4t -lgcc
LINKFLAGS = -Tapp.lds -Bstatic

export CC LD OBJCOPY OBJDUMP DIR_INCLUDE TOPDIR

app.bin :subdirs $(objs)
	$(LD) -v $(LINKFLAGS) -o app_elf $(objs) $(CLIBS)
	$(OBJCOPY) -O binary -S app_elf $@
	$(OBJDUMP) -D -m arm  app_elf > app.dis
	$(NM) -v -l app_elf > app.map
	
clean: 
	find . \( -name '*.o' -o -name core -o -name ".*.flags" \) -type f -print \
	| grep -v lxdialog/ | xargs rm -f
	rm -f  app.dis app.bin app_elf app.map
		
include Rules.make
