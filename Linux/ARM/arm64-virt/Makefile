
ARCH := arm64 
CROSS_COMPILE := aarch64-linux-gnu-

KDIR = linux-5.18.2
KSRC_URL = https://mirrors.tuna.tsinghua.edu.cn/kernel/v5.x/$(KDIR).tar.xz

KIMG = $(KDIR)/arch/arm64/boot/Image
DTB = mydtb/my.dto
DTS = $(DTB:.dto=.dts)
ROOTFS_IMG = rootfs.img

QEMU_BASE_COMMAND = qemu-system-aarch64 -M  virt -cpu cortex-a53 -smp 4 -m 2048M -kernel $(KIMG) -dtb $(DTB)	\
	-append "root=/dev/vda rw rootfstype=ext4 console=tty1 ignore_loglevel" \
	-drive if=none,file=rootfs.img,id=hd0 \
	-device virtio-blk-device,drive=hd0 \
	-device virtio-gpu-pci	\
	-device usb-ehci -device usb-kbd -device usb-mouse -usb \
	-net nic -net user,hostfwd=tcp::10021-:22 \
	-show-cursor

all: $(KDIR) $(KDIR)/.config
	make -C $(KDIR) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) Image -j8

$(KIMG): 
	make all

$(KDIR):
	@echo "Download $(KDIR)"
	@wget -c $(KSRC_URL) && tar -xf $(KDIR).tar.xz

$(KDIR)/.config:
	make defconfig

_rootfs: 
	wget -c https://cdimage.ubuntu.com/ubuntu-base/releases/20.04.5/release/ubuntu-base-20.04.5-base-arm64.tar.gz && \
		mkdir -p $@ && tar -xf ubuntu-base-20.04.5-base-arm64.tar.gz -C $@

$(ROOTFS_IMG):
	make rootfs

tmpp: _rootfs
	echo "asdasd" | sudo chroot _rootfs /bin/bash << EOF; \
		sed -i 's/ports.ubuntu.com/mirrors.ustc.edu.cn/g' /etc/apt/sources.list; \
		apt-get update; 			\
		apt-get install systemd vim net-tools dhcpcd5 kmod; \
	EOF

rootfs: _rootfs
	dd if=/dev/zero of=rootfs.img bs=1G count=8
	mkfs.ext4 $(ROOTFS_IMG)
	echo "asdasd" | sudo mount -o loop $(ROOTFS_IMG) /mnt
	echo "asdasd" | sudo cp -p _rootfs/* /mnt -rf
	echo "asdasd" | sudo umount /mnt

header: _rootfs
	make -C $(KDIR) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) modules -j8
	echo "asdasd" | sudo make -C $(KDIR) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) modules_install INSTALL_MOD_PATH=_rootfs/lib
	echo "asdasd" | sudo make -C $(KDIR) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) headers_install INSTALL_HDR_PATH=_rootfs/usr/include
	make -C $(KDIR) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) clean
	echo "asdasd" | sudo cp -r $(KDIR) _rootfs/usr/src

# 默认配置并开启调试功能, virt 的图像显示
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
		--disable CONFIG_DEBUG_EFI							\
		--set-val CONFIG_DRM y								\
		--set-val CONFIG_DRM_VIRTIO_GPU y					\
		--disable CONFIG_DRM_DP_AUX_CHARDEV					\
		--disable CONFIG_DRM_DEBUG_MM
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

qemu: $(KIMG) $(DTB) $(ROOTFS_IMG)
	$(QEMU_BASE_COMMAND)

debug: $(KIMG) $(DTB) $(ROOTFS_IMG)
	$(QEMU_BASE_COMMAND) -S -s

PHONY += dtb all defconfig qemu debug menuconfig rootfs
.PHONY: $(PHONY)
