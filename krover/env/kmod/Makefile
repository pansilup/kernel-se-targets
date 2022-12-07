CFLAGS_elf.o := -O0
obj-m += kmod.o

ld-objs += kmod.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

