#!Makefile

obj-m = sblkdev.o
KERNEL_DIR = /usr/src/kernels/$(shell uname -r)

PWD = $(shell pwd)

all:
	$(MAKE) -C $(KERNEL_DIR) SUBDIRS=$(PWD) modules

.PHONY:
clean:
	$(RM) *.o *.mod.* *.order *.symvers *.ko
