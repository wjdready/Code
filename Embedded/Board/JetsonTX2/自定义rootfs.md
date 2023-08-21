
#### 基本系统

```sh
export TX2=~/TX2

# 解压文件
cd $TX2/packages
tar -xf Jetson_Linux_R32.7.3_aarch64.tbz2 -C $TX2
sudo tar -pxf ubuntu-base-18.04.5-base-arm64.tar.gz -C $TX2/Linux_for_Tegra/rootfs

# >>>>>> 进入 rootfs >>>>>>>>
sudo arch-chroot $TX2/Linux_for_Tegra/rootfs

# 添加源
sed -i 's/ports.ubuntu.com/mirrors.ustc.edu.cn/g' /etc/apt/sources.list
apt-get update

# 安装如下软件
# systemd 
# libwrap0 libmnl0 libnl-genl-3-dev libpcsclite-dev libssl1.0-dev libelf-dev openssh-server 
# libparted-dev inetutils-ping wpasupplicant parted kmod xxd iproute2 mtd-utils isc-dhcp-client

# 安装完成后执行如下命令，安装 NV 官方提供的组件
sudo $TX2/Linux_for_Tegra/apply_binaries.sh
sudo rm $TX2/Linux_for_Tegra/rootfs/dev/random
sudo rm $TX2/Linux_for_Tegra/rootfs/dev/urandom
sudo rm $TX2/Linux_for_Tegra/rootfs/dev/random
apt --fix-broken install

# 安装软件, 必装的是 usb 和 network-manager, 因为这两个装好后可以 ssh 和联网
# gcc vim network-manager

# 安装 nv 提供的 deb 包, 位于 Linux_for_Tegra/nv_tegra/l4t_deb_packages
# nvidia-l4t-xusb-firmware 需要安装 bridge-utils 才能启动

# 并注册一个用户, 可能需要 chroot, 而不是 arch-chroot
useradd shino -m
passwd shino
apt-get install sudo
usermod -a -G sudo shino

# 创建要自动登录的 tty, tty1 和 ttyS0 
vim /etc/systemd/system/getty@tty1.service
[Service]
ExecStart=-/sbin/agetty --noissue --autologin shino %I $TERM
Type=idle

# 支持 ssh Root 登录, 进入文件搜索 Root, 改成 yes
vim /etc/ssh/sshd_config
# <<<<<<< 离开 rootfs <<<<<<<

# 备份包
sudo tar -cf $TX2/LFS/rootfs.tar $TX2/Linux_for_Tegra/rootfs

# 最后通过 usb 烧写, -r 可跳过 system.img 的构建
sudo ./flash.sh jetson-tx2-devkit mmcblk0p1
```

#### 后续操作 

可在系统启动后完成, 也可在 rootfs 中完成

```sh
# wifi 连接, 需安装 network-manager
nmcli device wifi list
sudo nmcli device wifi connect SSID password PASSWORD

# Bash 自动补全
sudo apt-get install bash-completion
source /etc/bash_completion

# 安装 X11 和 i3 或 fluxbox, 进入系统后执行 startx
apt-get install xorg i3
vim ~/.xsession

# 登录自动进入桌面
vim ~/.bashrc
# if [[ ! $DISPLAY && $XDG_VTNR -eq 1 ]]; then
#     exec startx
# fi

# 远程桌面
apt-get install xrdp
systemctl start xrdp start      # 启动
netstat -tlnp                   # 查看端口 

# 中文字体, 安装后才可显示中文
sudo apt-get install fonts-wqy-zenhei

# 设置语言环境为中文 zh_CN.UTF-8, 可以不设置
sudo vim /etc/default/locale

# 常用软件包
nautilus # 文件浏览
```
