#!Makefile

obj-m = process_print.o
KERNEL_DIR = /usr/src/kernels/3.9.11-200.fc18.i686.PAE
PWD = $(shell pwd)

all:
	$(MAKE) -C $(KERNEL_DIR) SUBDIRS=$(PWD) modules

.PHONY:
clean:
	$(RM) *.o *.mod.* *.order *.symvers *.ko
