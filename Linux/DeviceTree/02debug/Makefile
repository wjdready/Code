
ARCH := arm64 
CROSS_COMPILE := aarch64-linux-gnu-

KDIR = linux-5.18.2
KSRC_URL = https://mirrors.tuna.tsinghua.edu.cn/kernel/v5.x/$(KDIR).tar.xz

KIMG = $(KDIR)/arch/arm64/boot/Image
DTB = mydtb/my.dto
DTS = $(DTB:.dto=.dts)

all: $(KDIR) $(KDIR)/.config
	make -C $(KDIR) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) Image -j8

$(KIMG): 
	make all

$(KDIR):
	@echo "Download $(KDIR)"
	@wget -c $(KSRC_URL) && tar -xf $(KDIR).tar.xz

$(KDIR)/.config:
	make defconfig

# 默认配置并开启调试功能
defconfig: 
	make -C $(KDIR) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) defconfig
	$(KDIR)/scripts/config --file $(KDIR)/.config 	\
		--set-val CONFIG_DEBUG_INFO y						\
		--enable CONFIG_DEBUG_INFO_DWARF_TOOLCHAIN_DEFAULT 	\
		--disable CONFIG_DEBUG_INFO_NONE					\
		--disable CONFIG_DEBUG_INFO_SPLIT					\
		--disable CONFIG_DEBUG_INFO_BTF						\
		--disable CONFIG_DEBUG_INFO_REDUCED					\
		--disable CONFIG_DEBUG_INFO_COMPRESSED				\
		--disable CONFIG_GDB_SCRIPTS						\
		--disable CONFIG_DEBUG_EFI
	make -C $(KDIR) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) oldconfig

menuconfig:
	make -C $(KDIR) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) menuconfig 

$(DTS):
	mkdir -p $(dir $@)
	qemu-system-aarch64 -M  virt,dumpdtb=$(DTB) -nographic
	dtc -I dtb -O dts $(DTB) > $@

dtb: $(DTS)
	dtc -I dts -O dtb $< -o $(DTB)

$(DTB): 
	make dtb

qemu: $(KIMG) $(DTB)
	qemu-system-aarch64 -M  virt -cpu cortex-a53 -smp 2 -m 2048 -kernel $(KIMG) -dtb $(DTB) -nographic

debug: $(KIMG) $(DTB)
	qemu-system-aarch64 -M  virt -cpu cortex-a53 -smp 2 -m 2048 -kernel $(KIMG) -dtb $(DTB) -nographic -S -s

PHONY += dtb all defconfig qemu debug menuconfig
.PHONY: $(PHONY)
