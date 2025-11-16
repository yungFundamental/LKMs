obj-m += log_ls.o
log_ls-objs := main.o trampoline/trampoline.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

