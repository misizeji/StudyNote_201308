#this is a makefile for the driver make 

obj-m:=00_my_module.o

ifneq ($(KERNELRELEASE),)
KERNELDIR = $(KERNELRELEASE)
else
KERNELDIR = /home/edu/share/2.6.35.7_tools/unsp210_linux_2.6.35
endif

$(PWD):=$(shell pwd)

default:
	@echo make ....
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
	@echo make over !!!
clean:
	rm -rf *.ko
