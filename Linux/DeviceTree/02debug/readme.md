---
mytags: myblog
title: 快速搭建 linux 源码调试环境
date: 2023-08-26 21:21:40
categories: [linux]
tags: [linux, debug, 源码]
---

本文主要介绍如何快速搭建 linux 源码调试环境
<!-- more -->


Makefile

```makefile
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
```

直接执行 make debug

然后配置 vscode 

.vscode/launch.json 

```json
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "(gdb) 启动",
            "type": "cppdbg",
            "request": "launch",
            "program": "${workspaceFolder}/linux-5.18.2/vmlinux",
            "args": [],
            "stopAtEntry": false,       // 在进入debug 后暂停程序
            "cwd": "${workspaceFolder}",
            "environment": [],
            "externalConsole": true,    // 调试对象启动控制台
            "MIMode": "gdb",
            "miDebuggerPath": "/usr/bin/gdb-multiarch",
            "miDebuggerServerAddress": "localhost:1234",
        }
    ]
}
```
#### 前提环境

对于 Ubuntu 需安装
```
sudo apt-get install gcc-aarch64-linux-gnu
sudo apt-get install libncurses5-dev  build-essential git bison flex libssl-dev gdb-multiarch
```

gdb-multiarch 适用于多平台的调试, 真不错


#### 参考文献

[1](https://zhuanlan.zhihu.com/p/510289859?utm_id=0)
