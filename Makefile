obj-m := led_driver.o
KDIR := /lib/modules/4.14.79-v7+/build
PWD := $(shell pwd)

default:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD)

