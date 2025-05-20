obj-m += rootkit.o

KDIR := /usr/src/linux-source-6.8.0

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean