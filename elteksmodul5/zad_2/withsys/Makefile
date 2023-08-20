obj-m += mysys.o

all :
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD)

clear :
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD)
