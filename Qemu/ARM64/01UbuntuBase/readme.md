---
mytags: myblog
title: QEMU ARM64 仿真
date: 2023-01-18 08:04:16
categories: [QEMU, ARM64]
tags: [QEMU, 仿真]
---

本文主要介绍如何通过 QEMU 仿真 ARM64 板子
<!-- more -->

# QEMU ARM64A

测试环境 Win10 VMWare: Ubuntu18.04

### 1. 创建工作目录
```sh
mkdir ~/lfs -p
export LFS=~/lfs
mkdir $LFS/src $LFS/rootfs $LFS/dst -p
```

### 2. 构建 Kernel

##### 2.1 下载源码

```sh
cd $LFS/src
wget https://mirrors.tuna.tsinghua.edu.cn/kernel/v5.x/linux-5.18.2.tar.xz
tar -xvf linux-5.18.2.tar.xz
cd linux-5.18.2
```

##### 2.2 内核配置

```sh
export ARCH=arm64 
export CROSS_COMPILE=aarch64-linux-gnu-

make defconfig
make menuconfig
```

Linux arm 和 arm64 的 “defconfig” 应该包括 virtio 和 PCI 控制器的正确设备驱动程序；一些较旧的内核版本，尤其是 32 位 Arm，默认情况下并未启用所有功能。如果您没有看到您期望的 PCI 设备，请检查您的配置是否具有：
```php
CONFIG_PCI=y
CONFIG_VIRTIO_PCI=y
CONFIG_PCI_HOST_GENERIC=y
```

##### 2.3 虚拟机图形支持

图形也可用，但与 x86 不同，没有启用默认显示设备：您应该从 “-device ?” 的显示设备部分选择打开。一个好的选择是使用：
```php
-device virtio-gpu-pci
```
并启用以下 Linux 内核选项（以及前面列出的通用 “virtio PCI for the virt machine” 选项）：
```php
CONFIG_DRM=y 
CONFIG_DRM_VIRTIO_GPU=y
```

##### 2.4 编译导出内核镜像

```sh
make zImage
cp arch/arm64/boot/Image $LFS/dst
```

### 3. 制作根文件系统

```sh
# 工具下载
sudo apt-get install arch-install-scripts binfmt-support qemu-system-arm qemu-user-binfmt qemu-user-static

# 制作镜像
dd if=/dev/zero of=$LFS/dst/rootfs.img bs=1G count=2
mkfs.ext4 $LFS/dst/rootfs.img
sudo mount -o loop $LFS/dst/rootfs.img /mnt

# 解压 Ubuntu base
wget -P $LFS/src -c https://cdimage.ubuntu.com/ubuntu-base/releases/18.04/release/ubuntu-base-18.04.5-base-armhf.tar.gz
sudo tar -pxf $LFS/src/ubuntu-base-18.04.5-base-armhf.tar.gz -C /mnt

# 进入 chroot >>>
sudo arch-chroot /mnt

# 必备软件下载
sed -i 's/ports.ubuntu.com/mirrors.ustc.edu.cn/g' /etc/apt/sources.list
apt-get update
apt-get install systemd vim net-tools dhcpcd5 kmod 

# 可选下载 (可在启动后再安装)
apt-get install qt5-defeault libsdl2-dev libsdl-dev libdrm-tests git gcc make openssh-server

# 添加串口服务
ln -s /lib/systemd/system/getty@.service /etc/systemd/system/getty.target.wants/getty@ttyAMA0.service

# 设置默认登录用户为 root 且免密登录
vim /lib/systemd/system/getty@.service
# 替换该行 ExecStart=-/sbin/agetty --autologin root --noclear %I $TERM

# 离开 chroot <<<
sudo umount /mnt
```

### 4. 启动参数

基本启动参数

```sh
qemu-system-aarch64         \
    -M  virt                \
    -cpu cortex-a53         \
    -smp 2                  \
    -m 4096M                \
    -kernel $LFS/dst/Image  \
    -nographic              \
    -append "root=/dev/vda rw rootfstype=ext4 console=ttyAMA0 ignore_loglevel" \
    -drive if=none,file=$LFS/dst/rootfs.img,id=hd0 \
    -device virtio-blk-device,drive=hd0
```

可选参数
```sh
# 需要连接网络或通过 ssh 远程操作时
-net nic -net user,hostfwd=tcp::10021-:22

# 启动图形时, 将当前终端作为模拟串口
-serial stdio

-usb
-device nec-usb-xhci
-device usb-host,hostbus=2,hostaddr=1
-device usb-mouse
-device usb-kbd
-show-cursor
-fsdev local,security_model=passthrough,id=fsdev0,path=/nfsroot
-device virtio-9p-pci,id=fs0,fsdev=fsdev0,mount_tag=hostshare
```


### 5. 远程登录

```shell
# 确保 QEMU 启动时带如下参数, 并且 root 设置了密码 passwd
-net nic -net user,hostfwd=tcp::10021-:22

# 安装 openssh-server 和配置 sshd
apt-get install openssh-server

# 将 PermitRootLogin 改成 yes
vim /etc/ssh/sshd_config

# 然后重启 sshd, 如果出错使用 sshd -T 查看原因
service sshd restart

# 远程登录
ssh root@localhost -p 10021

# 上传文件
scp -P 10021 xxx root@localhost:/root

# 免密登录 cat ~/.ssh/id_rsa.pub
ssh-keygen
vim authorized_keys
```

### 6. 测试图形

##### 1. 测试 libdrm
```shell
apt-get install libdrm-tests
modetest -M virtio_gpu -s 36@35:640x480
```

##### 2. 测试 frambuffer

fbtest.c

```c 
#include <stdio.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <linux/fb.h>
#include <sys/mman.h>

int main(int argc, char const *argv[])
{
    int i, j, fd, var;

    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;

    if (argc < 2)
    {
        fprintf(stderr, "Example: %s /dev/fb0\n", argv[0]);
        return -1;
    }

    fd = open(argv[1], O_RDWR);
    if (fd < 0)
    {
        fprintf(stderr, "Can't open file %s: %s\n", argv[1], strerror(errno));
        return -1;
    }

    /* Get variable screen information */
    if (ioctl(fd, FBIOGET_VSCREENINFO, &vinfo))
    {
        perror("Can't get FBIOGET_VSCREENINFO");
        return -1;
    }

    /* Get fixed screen information */
    if (ioctl(fd, FBIOGET_FSCREENINFO, &finfo))
    {
        perror("Can't get FBIOGET_VSCREENINFO");
        return -1;
    }

    /* show these information*/
    printf("vinfo.xres          = %d\n", vinfo.xres);
    printf("vinfo.yres          = %d\n", vinfo.yres);
    printf("vinfo.bits_per_bits = %d\n", vinfo.bits_per_pixel);
    printf("vinfo.xoffset       = %d\n", vinfo.xoffset);
    printf("vinfo.yoffset       = %d\n", vinfo.yoffset);
    printf("finfo.line_length   = %d\n", finfo.line_length);

    /* Figure out the size of the screen in bytes */
    int screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;

    /* Map the device to memory */
    char *fbp = mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (fbp == MAP_FAILED)
    {
        perror("mmap error");
        return -1;
    }

    memset(fbp, 0xff, screensize);
    
    /* Where we are going to put the pixel */
    for (int x = 0; x < vinfo.xres; x++)
    {
        for (int y = 0; y < vinfo.yres; y++)
        {
            int location = (x + vinfo.xoffset) * (vinfo.bits_per_pixel / 8) +
                       (y + vinfo.yoffset) * finfo.line_length;

            *(fbp + location) = 0xff; /*  blue */
            *(fbp + location + 1) = 0x00;
        }
    }
    
    /* release the memory */
    munmap(fbp, screensize); 
    close(fd);
    printf("all ok\n");

    return 0;
}
```

编译运行

```
apt-get install gcc
gcc fbtest.c
./a.out
```

##### 3. 测试 QT (基于 frambuffer)

main.cpp

```c
#include <QApplication>
#include <QLabel>

int main(int argc, char *argv[]) 
{
    QApplication a(argc, argv);
    
    QLabel *label = new QLabel("Hello, world!");
    label->show();
    
    return a.exec();
}
```

demo.pro

```
QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = qtdemo
SOURCES += main.cpp
```

编译运行

```shell
apt-get install qt5-default g++ make
qmake && make -j8
export QT_QPA_PLATFORM=linuxfb:tty=/dev/fb0
./qtdemo
```

## 参考文献
* https://wiki.qemu.org/Documentation/Platforms/ARM
* https://www.cnblogs.com/pengdonglin137/p/6431234.html

