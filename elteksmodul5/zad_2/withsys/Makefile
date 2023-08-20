obj-m += mysys.o

all :
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD)

clear :
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD); rm Module.symvers modules.order mysys.o mysys.ko mysys.mod \
	mysys.mod.c  mysys.mod.o
