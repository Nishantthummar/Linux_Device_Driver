obj-m := interrupt.o
KDIR := /lib/modules/5.3.0-42-generic/build
PWD := $(shell pwd)

default:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD)
