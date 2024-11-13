
# 模块化调试
KDIR ?= /home/shino/code/android/RK3399J/sources/kernel

obj-m += tlv320aic32x4.o

module:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean

# 内置模块
# obj-y += ak5574.o
