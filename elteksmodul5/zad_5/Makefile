obj-m += kernmod.o

all :
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD)
	gcc usermod.c -o usermod

clear :
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD); rm -f .*; rm usermod; rm Module.symvers modules.order kernmod.o \
	kernmod.ko kernmod.mod kernmod.mod.c  kernmod.mod.o
	
