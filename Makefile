obj-m := cdriver.o
KDIR := /lib/modules/5.0.0-23-generic/build
PWD := $(shell pwd)

default:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD)
