

### [2023-02-20 22:38:43] 起始

首先当然是从看[官方文档](https://mangopi.org/)开始啦. 

进入后需要选择 T113-S3 的型号，这里直接给出 [T113-S3 型号软硬件相关文档全在这里](https://mangopi.org/mqr).

发现在官方文档里面有一篇[T113-S3入门资料汇总（避坑指南）](https://bbs.aw-ol.com/topic/2495) 是个人写的, 可以作为参考...

还包括和芯片的数据手册 [t113-s3_datasheet_v1.6.pdf](https://mangopi.org/_media/t113-s3_datasheet_v1.6.pdf) 和用户手册 [t113-s3_user_manual_v1.3_.pdf](https://mangopi.org/_media/t113-s3_user_manual_v1.3_.pdf)

现在把官方源码[Tina-Linux](https://github.com/mangopi-sbc/Tina-Linux) 克隆下来, 根据教程试图一步一步来编译和构建...

包含的子模块依赖中 Linux 内核源码下载要等好长时间, 需要耐心等待...

不过有百度云的源码[一键下载包](https://pan.baidu.com/s/1wxUeYQZaSgAPGorGOVcJxA?pwd=tina), 6个G左右, 用这个会快一些吧, 等这么长时间, 先去干别的事情吧...

# [2023-03-21 16:08:59]

首先需要明确的是 UART 调试口: 板子上的 E2 和 E3

[参考](https://blog.csdn.net/qq_28877125/article/details/126684892) 这篇文章，基本总结

通过git 下太慢了，最后直接使用百度网盘的那个包了. 另外记得切换会 bash, 否则 lunch 是 zsh 环境无法显示列表.

```sh
tar -xf Tina-Linux-20220815.tar.gz
cd Tina-Linux
source build/envsetup.sh

# lunch 选择 7. t113_mq_r-tina 
lunch

# make 需要等很长时间，大概 30 分钟，可先去干其他事情。
make

# 打包boot0和uboot为一个文件
mboot

# pack 将 boot 和 rootfs 打包成一个.img镜像 (但我的没成功)
pack
```

在 out 目录中输出很多东西. 

## 烧录

正如上面那篇避坑指南，应该先拿编译好的[固件 mq-r-t113-rtl8723ds-5135-dns-uart0.zip](https://mangopi.org.cn/_media/undefined/mq-r-t113-rtl8723ds-5135-dns-uart0.zip)测试一下烧写 

使用 [PhoenixCard](https://www.aw-ol.com/downloads?cat=5) 进行烧写。

选择制作卡的种类: 启动卡， 然后插入 SD 卡进行烧录

## boot 流程

全志 boot 版本历史:

* boot-2011 基本不维护
* boot-2014 有维护，但更新比较少
* boot-2018 主要版本

boot 流程:

BROM --> BOOT0 --> UBOOT --> KERNEL
(ROM)   (SRAM)     (DRAM)    (DRAM)

BROM 会依次检测 SD0, NAND, EMMC2, SPI-NOR, 若检测到 BOOT0 固件，则加载到内部的几百 KB 的 SRAM 执行, 否则进入 FEL 模式 (量产流程)
                                   
BOOT0 主要完成 CPU 初始化，外设初始化，DRAM 初始化，若初始化成功，则初始化 flash，加载 UBOOT 到 SDRAM 中运行 (通过更改UBoot头部参数传递参数), 若初始化失败则进入 FEL 模式

UBOOT 也可以直接进入 efex 量产模式, 可用于烧写固件到 SPI NAND FLASH。

## sd 镜像

https://blog.csdn.net/cookie2g/article/details/82785539?spm=1001.2014.3001.5502

简单写一个镜像制作

```sh
#!/bin/bash

OUTPUT_FILE=mysdcard_image.img
BOOT0_FILE=out/t113-mq_r/image/boot0_sdcard.fex

function main()
{
    [ ! -f "$BOOT0_FILE" ] && echo "No found $BOOT0_FILE" && exit -1

    # Create image
    dd if=/dev/zero of=$OUTPUT_FILE bs=1M count=256

    # bind img file as loop dev
    local loopdev=$(losetup -f)
    losetup $loopdev $OUTPUT_FILE

    # Create part
    (
        echo n
        echo p
        echo 1
        echo 40960      # offset 20MB, 20 * 1024 * 1024 / 512
        echo +32MB      # size: 32MB

        echo nlsblk
        echo p
        echo 2
        echo 131072     # offset 64MB, 64 * 1024 * 1024 / 512
        echo ""         # size: All the rest.

        echo p
        echo w
        echo q
    ) | fdisk $loopdev
    
    partprobe $loopdev

    # make file system
    mkfs.vfat -I ${loopdev}p1
    mkfs.ext4 ${loopdev}p2

    # write BOOT0
    dd if=$BOOT0_FILE of=$loopdev bs=1k seek=8

    # delete loop dev bind
    losetup -d $loopdev
}

main $@
```

# [2023-03-24 10:33:40]

很乱, 一般来说，至少先把官方的例子编译成功，运行成功后才有搞头，但是当我运行 pack 失败后却卡了很久，我为此接触了 Bash-Debug, 想着通过 Bash-Debug 来一步步调试来解决问题。但是 Bash-Debug 本身也很坑，似乎无法转跳单文件执行的情况，我就这么摸索2天，但是没有进展。

然后我受不了了，想着去直接看 OpenWrt，因为 Tina-Linux 是基于 OpenWrt 改造的，项目结构原理都差不多，这没什么问题，但现在急需将板子编译成功啊。

一堆 bash, 各种配置，各种全局变量。我在这些繁杂的细节中找不到任何方向。

虽说原理是很简单的，一般来说 Linux 无非就是 uboot + kernel + rootfs, 原始直接使用 uboot + kernel + rootfs 虽然简单易懂，但是对于代码复用方面却很繁杂，因为直接使用这种方式的话就不得不为每一块板子进行适配，各种软件要重新编译，从而进入更多的麻烦。OpenWrt, buildroot, armbian 等都在试图通过一个项目的形式解决快速构建出 uboot + kernel + rootfs，对于新板子，只需提供一些相关配置即可完成系统构建。

通常，CPU 启动后会执行 Soc 上自带的 ROM 进行一些初始化，这个 ROM 会有一些功能，比如扫描 SDIO、SPI、NAND 或 MMC 接口来探测是否有可信任 boot0, 有则加载到一个很小的内部 SRAM 执行，boot0 会初始化 DRAM, 并加载 uboot 到 DRAM 中运行。从而进入常规的 Linux 启动流程，至于 X86 平台常用的 UEFI 之类的，目前不是很清楚，但是比较高级的嵌入式也开始往 UEFI 靠了。

了解一个板子应当从启动流程开始，并能够根据启动流程构建镜像文件，例如全志平台的 ROM 中的代码会探测 SD 卡 8KB 偏移处的可信任固件 Boot0, 并加载到 SRAM 中运行，SRAM 中运行的 Boot0 再加载 uboot 到 DRAM 运行。了解每一个加载位置，在做镜像时才能将对应的代码写到对应的位置。

但是这些都是后话，安装官方例程，怎么把源码编译成功，打包成功才是现在急需做的，连源码都运行不起来，怎么进行探究。

其实，还是因为自己对错误的逃避，才这么就没去解决。其实错误报错很简单

```
mbr size = 16384
mbr magic softw411
disk name=boot-resource
disk name=env
disk name=env-redund
disk name=boot
ERROR: dl file boot.fex size too large
ERROR: filename = boot.fex 
ERROR: dl_file_size = 8068 sector
ERROR: part_size = 8064 sector
update_for_part_info -1
ERROR: update mbr file fail
```

它甚至提示了错误的原因: `dl file boot.fex size too large`, 即 dl_file_size 大小实际是 8068, 而该分区只设置了 8064 所以放不下去，最简单的方式就是搜索所有文件，找到 8064 定义的位置, 把分区改大一些就可以了。

果然在 `Tina-Linux/device/config/chips/t113/configs/mq_r/sys_partition.fex` 中找到，并更改大小:

```
[partition]
    name         = boot
    size         = 8068
    downloadfile = "boot.fex"
    user_type    = 0x8000
```

这个文件似乎是对镜像文件分区的描述，但是 boot0 肯定是直接dd 到对应的位置，而不是直接放在分区中。

生成镜像文件无法直接使用 dd 放到 sd 卡，而是需要使用官方 PhoenixCard 工具进行烧写...

## adb 

我发现编译后运行的系统开机默认就使用了 adb 功能，可以直接通过 adb shell 来连接板子就可以了，采用 adb 的这个方案很好，对调试板子来说很方便。

执行一些常用的 adb 命令

```sh
# 获取设备的状态, device：设备正常连接, offline, unknown
adb get-state

# 查看已连接的设备
adb devices

# 重启
adb reboot

# 重启后进入 recovery (板子没加入 recovery)
adb reboot recovery 

# 推送文件到 root 文件夹
adb push log.txt root/

# 拉取文件
adb pull root/hello.txt .

# 安装 package
adb install xxx

# 获取序列号, 即 adb devices 列出的
adb get-serialno

# 执行 xxx 命令, 如果 xxx 省略, 则直接进入 shell 交互模式
adb shell xxx

# 通过 shell 执行命令可将板子输出内容定向到主机的文件, 很方便
adb shell dmesg > log.txt
```

## Ubuntu base

wget http://cdimage.ubuntu.com/ubuntu-base/releases/22.04/release/ubuntu-base-22.04-base-riscv64.tar.gz

