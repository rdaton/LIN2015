obj-m +=  modtimer.o 
modtimer-objs += cbuffer.o probar.o
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

