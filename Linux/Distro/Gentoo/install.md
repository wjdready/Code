
官方文档: https://wiki.gentoo.org/wiki/Handbook:AMD64/Full/Installation

安装流程: 
1. 先下载一个可用于系统安装的镜像
2. 在这个系统镜像中捣鼓, 通过 emerge 构建完整系统

## 镜像系统准备

```sh
# 系统镜像
# https://mirrors.tuna.tsinghua.edu.cn/gentoo/releases/amd64/autobuilds/ 选择 current-install-amd64-minimal
# 启动后进入系统，开启 ssh

rc-service sshd start
# 设置密码
passwd root
```

## 安装


```sh
# 进入安装系统后进行磁盘分区
# 分 efi 和 rootfs 主分区就够了

# 格式化成官方推荐的 xfs
mkfs.xfs /dev/sda2

# 挂载主分区
mount /dev/sda2 /mnt/gentoo

# 下载预构建的 rootfs 然后解压
cd /mnt/gentoo
wget https://mirrors.tuna.tsinghua.edu.cn/gentoo/releases/amd64/autobuilds/current-stage3-amd64-desktop-openrc/stage3-amd64-desktop-openrc-20241117T163407Z.tar.xz
tar -xf stage3-amd64-desktop-openrc-20241117T163407Z.tar.xz

# 将当前的 dns 配置复制过去
cp /etc/resolv.conf /mnt/gentoo/etc/resolv.conf

# 同步一下时间
chronyd -q

# 之后切换到该系统
arch-chroot /mnt/gentoo

# 更新软件包
emerge-webrsync

# 接下来就可以进行各种定制安装了

# 比如安装 vim 
emerge vim

# 安装内核和 efi

```

/etc/portage/make.conf 是源码构建系统中设置全局编译选项

```sh
# 编译进程
MAKEOPTS="-j12"
USE="dist-kernel"
```
