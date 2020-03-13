obj-m := mychardev.o
KDIR := /lib/modules/5.3.0-40-generic/build
PWD := $(shell pwd)

default:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD)
