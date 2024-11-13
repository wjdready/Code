
# [2023-11-20 12:23:17]

官方 [wiki](https://wiki.luckfox.com/zh/Luckfox-Pico/Luckfox-Pico-quick-start)

## 源码编译

```sh
# 
git clone https://gitee.com/LuckfoxTECH/luckfox-pico.git

./build.sh -h
```

构建脚本的框架类似 AOSP, 最主要的是 project 目录, 包含 build.sh, 工程目录下的 build.sh 只是 project/build.sh 的软链接

首先 `build.sh lunch` 选择板子, 然后 `build.sh [target]` 来选择要构建的目标

project/cfg-all-items-introduction.txt 给出了所有配置项

根据 build.sh 中

```sh
RK_TARGET_BOARD_ARRAY=( $(cd ${TARGET_PRODUCT_DIR}/; ls BoardConfig*.mk BoardConfig_*/BoardConfig*.mk | sort) )
```

可知每个板子的配置都在 cfg 下, 必须以 BoardConfig 为前缀


# [2024-11-11 10:00:10]

仔细看, RK 修改了 kernel 的 Makefile, kernel/arch/arm/Makefile 中添加

```makefile
...

# Default target when executing plain make
boot := arch/arm/boot
ifeq ($(CONFIG_XIP_KERNEL),y)
KBUILD_IMAGE := $(boot)/xipImage
else
KBUILD_IMAGE := $(boot)/zImage
endif

...

all:	$(notdir $(KBUILD_IMAGE))

...

# ---------------- RK 添加 -----------------
%.img:
ifeq ("$(CONFIG_MODULES)$(MAKE_MODULES)$(srctree)","yy$(objtree)")
	$(Q)$(MAKE) $*.dtb zImage Image.gz modules
else
	$(Q)$(MAKE) $*.dtb zImage Image.gz
endif
	$(Q)$(srctree)/scripts/mkimg --dtb $*.dtb

CLEAN_DIRS += out
CLEAN_FILES += boot.img kernel.img resource.img zboot.img
# -------------------------------------------

```

RK 添加了自己的一个目标，使得对内核执行 make 时，使用 `make rv1103g-luckfox-pico-mini-b.img` 时会首先执行默认目标 zImage，然后执行 `$(Q)$(srctree)/scripts/mkimg --dtb $*.dtb` .

而 kernel/scripts/mkimg 是 RK 自己写的一个用于生成 boot.img resource.img 等镜像文件的 shell 脚本. 如果什么目标都不给，就执行 kernel 的默认目标，也就是 zImage

`mkimg.sh` 中使用了 AOSP 的打包工具 `mkbootimg.py` 可见 RK 平台的镜像文件是兼容安卓的


## 下载镜像

构建成功后会在 output/image 中生成各个分区的镜像, 然后使用 SocToolKit 来进行下载

```sh
-rw-r--r-- 1 shino None 3.0M 11月 11 11:09 boot.img
-rw-r--r-- 1 shino None 251K 11月 11 11:09 download.bin
-rw-r--r-- 1 shino None 256K 11月 11 11:09 env.img
-rw-r--r-- 1 shino None 178K 11月 11 11:09 idblock.img
-rw-r--r-- 1 shino None  21M 11月 11 11:09 oem.img
-rw-r--r-- 1 shino None  69M 11月 11 11:09 rootfs.img
-rw-r--r-- 1 shino None 1.7K 11月 11 11:09 sd_update.txt
-rw-r--r-- 1 shino None 1.6K 11月 11 11:09 tftp_update.txt
-rw-r--r-- 1 shino None 256K 11月 11 11:09 uboot.img
-rw-r--r-- 1 shino None  96M 11月 11 11:09 update.img
-rw-r--r-- 1 shino None 1.9M 11月 11 11:09 userdata.img
```

首先上电时按住boot键进入烧录模式， SocToolKit 识别后选择搜索路径，选择生成的镜像路径，SocToolKit 会自动识别和分析 env.img 镜像的内容，自动填充各个镜像的分区和地址，从而避免繁琐的手动输入

download.bin 是一个预先下载到内存的 uboot，然后通过该 uboot 的 usb 功能和 pc 通信，来实现软件烧录

每个分区都可以单独烧录，env.img 本身也会被烧录进去，并放在 0x00 地址处

可以单独烧录 download.bin 来获取 flash 设备信息，只需在 SocToolKit 的高级功能中，选择 "下载 DownloadBin", 然后就可以读取了

#### SD 卡镜像

要生成 sd 卡镜像，首先需要在编译配置时选择启动介质为 sd 卡，从而生成适合 sd 卡的启动镜像

rv1103 上电后片内 ROM 程序会优先从 spi nand flash 加载程序，然后才是 sd 卡，因此要从 sd 卡加载镜像，首先应当确保 flash 内的程序被擦除干净

然后使用 SocToolKit 工具进行镜像写入, SD 卡镜像实际也是由多个镜像写入到不同的 sd 卡地址而成, 在 "SD卡工具" 选项中加载启动文件 (即所有镜像文件)，并选择创建 SD 卡

这一过程中 SocToolKit 会读取分析 env.img 镜像文件内容，并将各个文件写入到对应的分区

#### env.img 分析

env.img 是由 uboot 源码中的 u-boot/tools/mkenvimage.c 生成的，用于提供 uboot 启动时的默认环境, 其中包含了各个分区信息，以及内核参数

https://bootlin.com/blog/mkenvimage-uboot-binary-env-generator/

