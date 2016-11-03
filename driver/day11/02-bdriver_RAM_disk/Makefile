# If KERNELRELEASE is defined, we've been invoked from the 
# kernel build system and can use its language. 

ifneq ($(KERNELRELEASE),) 
obj-m := ramdisk_driver.o 
#module_test-objs := file1.o file2.o

else 
KERNELDIR = /home/edu/share/2.6.35.7_tools/unsp210_linux_2.6.35
PWD := $(shell pwd)

.PHONY: modules modules_install clean

modules: 
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules 

modules_install:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules_install 
	
clean:
	rm -rf *.o *.mod.c *.order *.symvers
	
endif






