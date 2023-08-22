obj-m += main.o mysys.o

all :
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD)
	
clear :
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD); rm Module.symvers modules.order main.o main.ko main.mod \
	main.mod.c  main.mod.o mysys.o mysys.ko mysys.mod mysys.mod.c  mysys.mod.o
