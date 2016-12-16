obj-m +=  fifomod.o 
pcmod1-objs += cbuffer.o fifomod.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

