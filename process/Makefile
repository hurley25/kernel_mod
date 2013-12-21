#!Makefile

obj-m = process.o
KERNEL_DIR = /usr/src/kernels/3.11.4-101.fc18.i686
PWD = $(shell pwd)

all:
	$(MAKE) -C $(KERNEL_DIR) SUBDIRS=$(PWD) modules

.PHONY:
clean:
	$(RM) *.o *.mod.* *.order *.symvers *.ko
