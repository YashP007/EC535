# km/Makefile
obj-m := mytimer.o
KDIR := ../stock-linux-4.19.82-ti-rt-r33
PWD := $(shell pwd)

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
