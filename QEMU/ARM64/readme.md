# QEMU ARM64

测试环境 Win10 VMWare: Ubuntu18.04

## 编译 Linux Kernel

#### 创建工作目录
```sh
mkdir ~/lfs -p
export LFS=~/lfs
mkdir $LFS/src $LFS/rootfs $LFS/dst -p
```

### 构建 Kernel

#### 下载源码

```sh
cd $LFS/src
wget https://mirrors.tuna.tsinghua.edu.cn/kernel/v5.x/linux-5.18.2.tar.xz
tar -xvf linux-5.18.2.tar.xz
cd linux-5.18.2
```

#### 内核配置

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

#### 虚拟机图形支持

图形也可用，但与 x86 不同，没有启用默认显示设备：您应该从 “-device ?” 的显示设备部分选择打开。一个好的选择是使用：
```php
-device virtio-gpu-pci
```
并启用以下 Linux 内核选项（以及前面列出的通用 “virtio PCI for the virt machine” 选项）：
```php
CONFIG_DRM=y 
CONFIG_DRM_VIRTIO_GPU=y
```

### 编译导出内核镜像

```sh
make zImage
cp arch/arm64/boot/Image $LFS/dst
```

### 制作根文件系统

```sh
# 工具下载
sudo apt-get install arch-install-scripts binfmt-support qemu-system-arm qemu-user-binfmt qemu-user-static

# 制作镜像
dd if=/dev/zero of=$LFS/dst/rootfs.img bs=1G count=2
mkfs.ext4 $LFS/dst/rootfs.img
sudo mount -o loop $LFS/dst/rootfs.img /mnt

# 解压 Ubuntu base
cd $LFS/src
wget http://cdimage.ubuntu.com/ubuntu-base/releases/20.04/release/ubuntu-base-20.04.5-base-riscv64.tar.gz
tar -pxf ubuntu-base-18.04.5-base-armhf.tar.gz -C /mnt

# 进入 chroot >>>
sudo arch-chroot /mnt

# 必备软件下载
sed -i 's/ports.ubuntu.com/mirrors.ustc.edu.cn/g' /etc/apt/sources.list
apt-get update
apt-get install systemd vim gcc net-tools dhcpcd5 kmod make openssh-server

# 可选下载 
apt-get qt5-defeault libsdl2-dev libsdl-dev  libdrm-tests git 

# 添加串口服务
ln -s /lib/systemd/system/getty@.service /etc/systemd/system/getty.target.wants/getty@ttyAMA0.service

# 设置默认登录用户为 root 且免密登录
vim /lib/systemd/system/getty@.service
# 替换改行 ExecStart=-/sbin/agetty --autologin root --noclear %I $TERM

# 离开 chroot <<<
sudo unmount /mnt
```

## 启动参数

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

## 测试

* 测试 libdrm
    ```
    apt-get install libdrm-tests
    modetest -M virtio_gpu -s 36@35:640x480
    ```

* 测试 frambuffer
  https://www.jianshu.com/p/21c7c1911409

* 测试 QT
  ```
  apt-get install qt5-default
  export QT_QPA_PLATFORM=linuxfb:tty=/dev/fb0
  https://www.jianshu.com/p/caaf9361abe8
  ```


## 参考
* https://wiki.qemu.org/Documentation/Platforms/ARM
* https://www.cnblogs.com/pengdonglin137/p/6431234.html

