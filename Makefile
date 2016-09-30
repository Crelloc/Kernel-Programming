ifeq ($(KERNELRELEASE),)

KERNELDIR ?= /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

.PHONY: build clean

build: 
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
	$(CC) testingphonychar.c -o test
	insmod phonychar.ko
	insmod quote.ko
	mknod /dev/quote c 60 0
	chmod 666 /dev/quote

clean:
	rmmod phonychar
	rmmod quote
	rm -rf test /dev/quote /dev/phonychar
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean

else

$(info Building with KERNELRELEASE = ${KERNELRELEASE})

obj-m += quote.o
obj-m += phonychar.o

endif

