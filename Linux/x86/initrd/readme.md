
创建工作目录

mkdir work
cd work

## 构建内核

```sh
wget https://mirrors.tuna.tsinghua.edu.cn/kernel/v5.x/linux-5.18.2.tar.xz
tar -xf linux-5.18.2.tar.xz
cd linux-5.18.2
make defconfig
make menuconfig
# 为了调试方便，打开
# CONFIG_FB_EFI=y
# CONFIG_FRAMEBUFFER_CONSOLE=y
make -j8

# 生成 bzImage, 复制过来备用
cp arch/x86_64/boot/bzImage ../
```

## 构建 busybox

```sh
wget https://busybox.net/downloads/busybox-1.35.0.tar.bz2
tar -xvf busybox-1.35.0.tar.bz2
cd busybox-1.35.0
make defconfig
make menuconfig
# 设置静态编译, 目前只能手动
make -j8

# 生成 busybox, 复制过来备用
cp busybox ../
```

## initramfs

```sh
mkdir initramfs/bin -p
cp busybox initramfs/bin
vim initramfs/init
# init 文件下面列出
chmod +x initramfs/init

cd initramfs
find . -print0 | cpio --null -ov --format=newc | gzip -9 > ../initramfs.cpio.gz
```

`/init` 文件

```sh
#!/bin/busybox sh

# Install symlinks to all busybox applets first.
/bin/busybox mkdir -p /usr/sbin /usr/bin /sbin /bin /dev /proc /sys /root
/bin/busybox --install -s

# Mount the /proc and /sys filesystems.
mount -t devtmpfs none /dev
mount -t proc none /proc
mount -t sysfs none /sys

# 调试的时候开启 sh 命令行模式
# sh

echo "Wait..."

# 增加 2s 延时, 等待 usb 设备连接
sleep 2

# 挂载真正的文件系统, 根据 rootfs 实际分区进行修改
mount -o rw /dev/sda2 /root

# Clean up.
umount /dev
umount /proc
umount /sys

# Boot the real thing.
exec switch_root /root /sbin/init
```

快速测试 initramfs:

```sh
qemu-system-x86_64 -accel tcg -M q35 -m 4096 -smp 8 -kernel bzImage -initrd initramfs.cpio.gz -nographic -append "console=ttyS0"
```


## 构建镜像

```sh
# 创建磁盘并分区
dd if=/dev/zero of=disk.img bs=1M count=1024
fdisk disk.img
# 分2个区，一个 boot 128M, 一个 rootfs 剩余大小

sudo losetup -f --show -P disk.img
sudo mkfs.fat /dev/loopXp1
sudo mkfs.ext4 /dev/loop20p2

mkdir mnt -p
sudo mount /dev/loopXp1 mnt

# 安装 grub
# qemu 不支持 efi, 用老的启动方式
sudo grub-install --target i386-pc --boot-directory=mnt/boot disk.img
# efi 启动方式
sudo grub-install --target x86_64-efi --efi-directory=mnt
# 修改
sudo mkdir mnt/boot/grub -p

sudo cp bzImage mnt/boot
sudo cp initramfs.cpio.gz mnt/boot

# 配置 grup
sudo vim mnt/boot/grub/grub.cfg
# 添加如下内容
set timeout=0
set default=0
menuentry 'Linux' {
    set root='hd0,msdos1'
    linux /boot/bzImage root=/dev/sda1 rw
    initrd /boot/initramfs.cpio.gz
    boot 
}

# 操作完成后手动卸载
sudo umount mnt
# sudo losetup -d /dev/loopX
```

## 选项1 ubuntu-base rootfs

```sh
# 挂载第二个分区
sudo mount /dev/loopXp2 mnt
wget https://cdimage.ubuntu.com/ubuntu-base/releases/20.04.5/release/ubuntu-base-20.04.5-base-amd64.tar.gz
sudo tar -xf ubuntu-base-20.04.5-base-amd64.tar.gz -C mnt
sudo arch-chroot mnt bash
# >>>>>>>>>>>>>> 进入 rootfs
sed -i 's@//.*archive.ubuntu.com@//mirrors.ustc.edu.cn@g' /etc/apt/sources.list
apt-get update

apt-get install systemd vim sudo net-tools iproute2 dhcpcd5
apt-get install openssh-server

# 保证 systemd 为 init 进程
ln -s /lib/systemd/systemd /sbin/init

# 注册一个用户
useradd shino -m
passwd shino
apt-get install sudo
usermod -a -G sudo shino

# 操作完成后手动卸载
sudo umount mnt
# sudo losetup -d /dev/loopX
```

## 选项2 buildroot rootfs

```sh
git clone https://github.com/buildroot/buildroot.git
cd buildroot
make defconfig
make -j8

# 编译后输出 output/images/rootfs.tar
cd images/rootfs
mkdir myrootfs
tar -xf rootfs.tar -C myrootfs
vim myrootfs/etc/fstab
# 添加如下行以挂载 dev
# devtmpfs	/dev		devtmpfs	defaults	0	0

# 最后将文件夹写入分区
sudo mount /dev/loopXp2 /mnt
cp -rf myrootfs/* /mnt
sudo umount /mnt
```

## qemu 仿真

```sh
# 验证 initramfs
qemu-system-x86_64 -accel tcg -M q35 -m 4096 -smp 8 -kernel bzImage -initrd initramfs.cpio.gz -nographic -append "console=ttyS0"

# 验证 disk.img, 需打开 GUI, 因为不能 -append "console=ttyS0"
qemu-system-x86_64 -accel tcg -M q35 -m 4096 -smp 8 -drive file=disk.img

# 单独验证 disk.img 的 rootfs 分区
qemu-system-x86_64 -accel tcg -M q35 -m 4096 -smp 8 -kernel bzImage -initrd initramfs.cpio.gz -drive file=disk.img -nographic -append "console=ttyS0"
```

## 写入 sd 卡

```sh
sudo dd if=disk.img of=/dev/sdb

# sd 写入后， rootfs 分区扩容方法

fdisk /dev/sdb
# 删除第二个分区 (rootfs 分区)
# 创建新的分区，大小可为剩余所有
# 然后 e2fsck 检查文件系统
sudo e2fsck -f /dev/sdb2
# 使用  resize2fs  扩展文件系统以使用新分区的大小
sudo resize2fs /dev/sdb2
```

## 最后

当 EFI 启动失败时，可手动启动

```sh
# 开机选项中选择进入 EFI shell
# 列出所有磁盘分区和可被 EFI 识别的文件系统
map
# 根据列出情况选择对应的文件系统
fs1:
cd /EFI/ubuntu
# 执行 EFI 程序
grubx64.efi

# 在 grub 命令中
# 列出所有磁盘
ls
# 设置 linux kernel 位置
linux (hd0, msdos1)/boot/bzImage
# 设置 initrd 
initrd (hd0, msdos1)/boot/initramfs.cpio.gz
# 启动
boot
```

另外其实也是可以直接使用 ubuntu 的 kernel 和 initrd 的

```sh
sudo mount /dev/loopXp1 mnt
# 生成 initrd
sudo mkinitramfs -o mnt/boot/initrd
# 复制内核
sudo cp /boot/vmlinuz-xxx mnt/boot/
sudo umount mnt

# grub 中将 linux 和 initrd 设置为复制过来的
linux (hd0, msdos1)/boot/vmlinuz-xxx
initrd (hd0, msdos1)/boot/initrd
boot

# 启动后进入 (initramfs) 
# 在 initramfs 挂载 rootfs, 然后手动执行 switch_root
# exec 确保 switch_root 执行的父亲是 PID 1
mount /dev/sda2 /root
exec switch_root /root /sbin/init
```

## 后续

怎么设置 efi 启动栏中的名称，这似乎是比较麻烦的
https://askubuntu.com/questions/1125920/how-can-i-change-the-names-of-items-in-the-efi-uefi-boot-menu
https://askubuntu.com/questions/1129269/install-grub-using-custom-boot-loader-id-e-g-myubuntu

参考文档: 
https://wiki.gentoo.org/wiki/Custom_Initramfs
