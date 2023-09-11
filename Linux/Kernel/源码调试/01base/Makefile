
ARCH := arm64 
CROSS_COMPILE := aarch64-linux-gnu-

KDIR = ./linux-5.18.2
KSRC_URL = https://mirrors.tuna.tsinghua.edu.cn/kernel/v5.x/$(KDIR).tar.xz

KIMG = $(KDIR)/arch/arm64/boot/Image
DTB = $(KDIR)/arch/arm64/boot/dts/broadcom/bcm2837-rpi-3-b.dtb

all: $(KDIR)
	make -C $(KDIR) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) Image -j8

$(KDIR):
	@echo "Download $(KDIR)"
	@wget -c $(KSRC_URL) && tar -xf $(KDIR).tar.xz

defconfig:
	make -C $(KDIR) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) defconfig 

menuconfig:
	make -C $(KDIR) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) menuconfig 

qemu:
	qemu-system-aarch64 -M  virt -cpu cortex-a53 -smp 2 -m 2048 -kernel $(KIMG) -nographic

debug:
	qemu-system-aarch64 -M  virt -cpu cortex-a53 -smp 2 -m 2048 -kernel $(KIMG) -dtb qemu.dtb -nographic -S -s

# ,dumpdtb=qemu.dtb 

