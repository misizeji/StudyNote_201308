DRIVERS-y :=
DRIVERS-y += arch/s5pv210.o
DRIVERS-y += init/init.o
DRIVERS-y += lib/uart.o
DRIVERS-y += drivers/mtd/nand_read.o

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
#CLIBS = -L$(ARM_GCC_LIBS) -lc
LINKFLAGS = -Tarch/unspb.lds -Bstatic

export CC LD OBJCOPY OBJDUMP DIR_INCLUDE TOPDIR

unspb.bin :subdirs $(objs)
	$(LD) -v $(LINKFLAGS) -o unspb_elf $(objs) $(CLIBS)
	$(OBJCOPY) -O binary -S unspb_elf $@
	$(OBJDUMP) -D -m arm  unspb_elf > unspb.dis
	$(NM) -v -l unspb_elf > unspb.map
	cp unspb.bin down_loader -rf
	
clean: 
	find . \( -name '*.o' -o -name core -o -name ".*.flags" \) -type f -print \
	| grep -v lxdialog/ | xargs rm -f
	rm -f  unspb.dis unspb.bin unspb_elf
	rm -f  unspb.dis unspb.bin unspb.map down_loader/unspb.bin
	
include Rules.make
