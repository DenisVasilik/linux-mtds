ifneq ($(KERNELRELEASE),)
	obj-m += mtds.o

	mtds-objs := mtds_drv.o \
				 mtds_hal.o \
				 xspi.o \
				 xspi_options.o \

else
	KERNELDIR ?= /home/zed-dev/arty-z7-petalinux/build/linux/kernel/linux-digilent/
	PWD := $(shell pwd)
	CCFLAGS := -I$(PWD)/include

all:
	$(MAKE) -C $(KERNELDIR) -I$(PWD)/include M=$(PWD) modules

endif

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
