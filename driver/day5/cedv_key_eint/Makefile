# If KERNELRELEASE is defined, we've been invoked from the 
# kernel build system and can use its language. 
obj-m := cdev_key_eint.o 
#obj-m := m_test.o 
#m_test-objs := module_test.o

ifneq ($(KERNELRELEASE),)
#KERNELDIR ?= /lib/modules/$(shell uname -r)/build 
KERNELDIR = $(KERNELRELEASE)
else 
KERNELDIR = /home/edu/share/2.6.35.7_tools/unsp210_linux_2.6.35
endif


$(PWD) := $(shell pwd)

default: 
	@rm -rf *.ko
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules 
	@rm -rf *.o .t* .m* .*.cmd *.mod.c *.order *.symvers
clean:
	rm -rf *.ko *.o .t* .m* .*.cmd *.mod.c *.order *.symvers


