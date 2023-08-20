obj-m += proc.o

all :
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD)

clear :
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD); rm Module.symvers modules.order proc.o proc.ko proc.mod \
	proc.mod.c  proc.mod.o
