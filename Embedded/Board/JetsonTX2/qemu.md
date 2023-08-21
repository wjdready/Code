

尝试在 Windows qemu 中进行 flash

```sh
# 首先编译 x86 内核，这里就不过多解释，直接 make defconfig, make -j8
# 甚至直接使用安装过的 ubuntu 系统中 boot 目录里的内核直接拿来用即可

wget https://cdimage.ubuntu.com/ubuntu-base/releases/20.04.5/release/ubuntu-base-20.04.5-base-amd64.tar.gz

mkdir rootfs -p
sudo tar -pxf ubuntu-base-20.04.5-base-amd64.tar.gz -C rootfs

sudo arch-chroot rootfs bash

# >>>>>>>> 进入 rootfs >>>>>>>>>>>>>
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

# 创建要自动登录的 tty, tty1 和 ttyS0 
vim /etc/systemd/system/getty@tty1.service
[Service]
ExecStart=-/sbin/agetty --noissue --autologin shino %I $TERM
Type=idle

dd if=/dev/zero of=ubuntu_amd64_rootfs.img bs=1G count=10
mkfs.ext4 ubuntu_amd64_rootfs.img
sudo mount -o loop ubuntu_amd64_rootfs.img /mnt/
sudo cp rootfs/* /mnt/ -rf
sudo umount /mnt
# <<<<<<<< 离开 rootfs <<<<<<<<<<<<<
```

# [2023-08-18 23:07:30]

不知为何安装 `systemd` 后 `/sbin/init` 应该链接到 `/lib/system/systemd` 的, 但是这次启动后提示 `bin/sh: can't access tty; job control turned off`, 并且有看到内核尝试了几个 init 的启动:

```
Run /sbin/init as init process
Run /etc/init as init process
Run /bin/init as init process
Run /bin/sh as init process
```
一开始还以为是 systemd 启动过程出现错误退出，搞了大半天，当试图使用 systemctl 时提示 `systemd` 不是 `init` 进程 (PID 1) 才注意到， `Run /sbin/init as init process` 后还往后执行是因为没有 init 这个文件。这时只需将其软链接到 `/lib/system/systemd` 即可.

```sh
qemu-system-x86_64 -accel tcg -M q35 -m 4096 -smp 8 -kernel bzImage -hda ubuntu_amd64_rootfs.img --append "console=tty1 root=/dev/sda rootfstype=ext4 rw"  -net nic -net user,hostfwd=tcp::10021-:22 -device nec-usb-xhci,id=xhci0 -device nec-usb-xhci,id=xhci1 -device usb-host,vendorid=0x0bda,productid=0x9210
```



# [2023-08-19 00:12:47]

添加网络还需 ifconfig eth0 up 和安装 dhcpcd5 才能使用，systemd 似乎没帮我们做这些工作? 虽然遇到了一些问题， 但总的来说最后还是联网成功了

ok 加入网络后就可以直接运行时下载了

```sh
tar -xf Jetson_Linux_R32.7.3_aarch64.tbz2

# 安装软件 usbutils, 支持 lsusb
# nv flash.py 需要装的软件
# cpio binutils python

# 查看加速支持
qemu-system-x86_64 -accel help

# 添加usb 设备, 技巧是在 qemu 控制台中 info usbhost, 然后添加设备即可
# 可以动态添加，在控制台中执行 device_add usb-host,vendorid=0x0955,productid=0x7c18

# Bus 003 Device 005: ID 0955:7c18 NVIDIA Corp. APX
# USB\VID_FFFF&PID_FFFF&REV_0100&MI_00
# USB\VID_0955&PID_7C18\6&52C0812&0&2
# Bus 003 Device 006: ID 2e8a:0003 Raspberry Pi RP2 Boot
-device nec-usb-xhci -device usb-host,hostbus=2,hostaddr=3
-device nec-usb-xhci,id=xhci0 -device nec-usb-xhci,id=xhci1 -device usb-host,vendorid=0x0955,productid=0x7c18 
-usb -device nec-usb-xhci -device usb-host,vendorid=0x0955,productid=0x7c18 
-usb -device usb-host,hostbus=2,hostaddr=3,hostport=2
-device usb-host,hostbus=0,hostport=2
-device qemu-xhci,id=xhci

# 虽然已经可以 lsusb 看到设备并下载，但是每次弹出都需要在 qemu 控制台中添加设备
# 解决这个问题似乎可以参考这个热插拔脚本
https://github.com/darkguy2008/hotplugger
```

参考文档:

* https://wiki.archlinux.org/title/QEMU#Pass-through_host_USB_device
* https://en.wikibooks.org/wiki/QEMU/Monitor#usb_add
* https://qemu-project.gitlab.io/qemu/system/devices/usb.html

