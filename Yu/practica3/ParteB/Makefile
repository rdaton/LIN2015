obj-m +=  fifomod.o 
fifomod-objs += cbuffer.o fifomod.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

