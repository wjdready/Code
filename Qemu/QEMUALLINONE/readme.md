环境2：WSL Ubuntu18.4 

```bash
echo "export LFS=~/lfs" >> ~/.bashrc

# 创建工作目录
mkdir ~/lfs -p
export LFS=~/lfs
mkdir $LFS/src $LFS/rootfs $LFS/dst -p

# 构建 Kernel
cd $LFS/src
wget https://mirrors.tuna.tsinghua.edu.cn/kernel/v5.x/linux-5.18.2.tar.xz
tar -xvf linux-5.18.2.tar.xz
cd linux-5.18.2
export ARCH=arm
export CROSS_COMPILE=arm-linux-gnueabi-
make vexpress_defconfig
make zImage dtbs -j8
cp arch/arm/boot/zImage arch/arm/boot/dts/vexpress-v2p-ca9.dtb $LFS/dst

############### 基于 Busybox #################

# 构建 Busybox
cd $LFS/src
wget https://busybox.net/downloads/busybox-1.35.0.tar.bz2
tar -xvf busybox-1.35.0.tar.bz2
cd busybox-1.35.0
export ARCH=arm
export CROSS_COMPILE=arm-linux-gnueabi-
make menuconfig
# 设置静态编译, 目前只能手动
make -j8
make install
cp _install/* $LFS/rootfs -r

# Busybox 初始化
# 首先需要做得就像创建必备目录 sys proc dev 这些是作为 kernel 内部虚拟文件系统的挂载点使用的
# 其次是创建 /ect 和 /root , 分别是配置目录和 root 用户的工作目录

mkdir $LFS/rootfs/sys $LFS/rootfs/proc $LFS/rootfs/dev  -p
mkdir $LFS/rootfs/etc/init.d $LFS/rootfs/root -p
mkdir $LFS/rootfs/lib -p

# 创建 root 用户, 默认无密码登录, 可在登录后设置
echo "root::0:0:root:/root:/bin/sh" > $LFS/rootfs/etc/passwd

# 让系统执行 rcS 下的脚本进行初始化工作
echo "::sysinit:/etc/init.d/rcS" > $LFS/rootfs/etc/inittab

# 设置可登录系统的终端
echo "ttyAMA0::respawn:/sbin/getty -L 0 ttyAMA0 vt100" >> $LFS/rootfs/etc/inittab

# 让 rcS 脚本挂载 kernel 虚拟文件系统, 
echo "mount -t sysfs sys /sys" > $LFS/rootfs/etc/init.d/rcS
echo "mount -t proc proc /proc" >> $LFS/rootfs/etc/init.d/rcS
echo "mount -t devtmpfs udev /dev" >> $LFS/rootfs/etc/init.d/rcS

# [可选] ssh 登录时需要挂载 /dev/pts 文件系统
mkdir $LFS/rootfs/dev/pts
echo "mount -t devpts devpts /dev/pts" >> $LFS/rootfs/etc/init.d/rcS

chmod 755 $LFS/rootfs/etc/init.d/rcS

# 复制动态库
cp /usr/arm-linux-gnueabi/lib/* $LFS/rootfs/lib -r


############### 基于 Ubuntu base #################

# 工具下载
sudo apt-get install arch-install-scripts binfmt-support qemu-system-arm qemu-user-binfmt qemu-user-static
cd $LFS/src
wget http://cdimage.ubuntu.com/ubuntu-base/releases/18.04/release/ubuntu-base-18.04.5-base-armhf.tar.gz
tar -pxf ubuntu-base-18.04.5-base-armhf.tar.gz -C $LFS/rootfs
sudo arch-chroot $LFS/rootfs
# 如果失败 No such file 则 sudo cp /usr/bin/qemu-arm-static /mnt/usr/bin/
## 进入 chroot  >>>>>>>>>>>>>>>> 
sed -i 's/ports.ubuntu.com/mirrors.ustc.edu.cn/g' /etc/apt/sources.list
apt-get update
apt-get install systemd vim gcc net-tools dhcpcd5 kmod make openssh-server
ln -s /lib/systemd/system/getty@.service /etc/systemd/system/getty.target.wants/getty@ttyAMA0.service
# 设置默认登录用户为 root 且免密登录
vim /lib/systemd/system/getty@.service
# ExecStart=-/sbin/agetty --autologin root --noclear %I $TERM
## <<<<<<<<<<<<<<<<<<< 离开 chroot 

# 创建系统镜像
dd if=/dev/zero of=$LFS/dst/rootfs.img bs=1G count=2
mkfs.ext4 $LFS/dst/rootfs.img
sudo mount -o loop $LFS/dst/rootfs.img /mnt
sudo cp $LFS/rootfs/* /mnt -r
sudo umount /mnt

qemu-system-arm                                     \
    -M vexpress-a9                                  \
    -m 512M                                         \
    -nographic                                      \
    -kernel $LFS/dst/zImage                         \
    -net nic -net user,hostfwd=tcp::10021-:22       \
    -dtb $LFS/dst/vexpress-v2p-ca9.dtb              \
    -sd $LFS/dst/rootfs.img                         \
    -append "root=/dev/mmcblk0 rw console=ttyAMA0"

### modules

ssh root@localhost -p 10021
scp -P 10021 xxx root@localhost:/root

make modules
make modules_install INSTALL_MOD_PATH=./outlib

make headers_install INSTALL_HDR_PATH=./outheader

cp -a outlib/lib/modules $LFS/rootfs/lib 
cp -a outheader/include $LFS/rootfs/usr/include
```
