obj-m	:= mygpio.o

PWD	:= $(shell pwd)

default:
	$(MAKE)	-C $(KDIR)	M=$(PWD) modules

clean:
	rm -rf *.ko *.mod *.mod.* *.o modules.order
	rm -rf Module.symvers
