
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

# 2023-08-25 09:00:28

发现屏幕没反应是因为启动器的原因? 太离谱了, 我一直以为是驱动的问题。

通过安装 sddm 让其自动登录, 这时发现有鼠标了, 但是屏幕黑的, 使用 journalctl 查看日志

# 2023-09-04 22:50:20

sddm 居然要和 i3 结合才行吗, 而且这时怎么突然就可以联网了。

# [2023-09-11 16:40:30]
好吧，事已至此，不能联网是因为在 flash 时，kernel 版本不对导致的 uname 文件夹不存在，拷贝网卡 .ko 失败，导致开机无法初始化网卡。

```sh
apt-get install sddm
vim /etc/sddm.conf
# 自动登录 i3 xfce (xfce4 但是必须要这么写 ?)
# [Autologin]
# User=shino
# Session=i3

# -u(--unit) 要显示的单元, -r(--reverse) 最新消息在前
sudo journalctl -u sddm.service -r

# 查看当前登录管理器
cat /etc/X11/default-display-manager

# 安装包可以重新配置
dpkg-reconfigure sddm

# lightdm 使用时提示 
# PAM unable to dlopen(pam_kwallet.so): /lib/security/pam_kwallet.so: cannot open shared object f
# ile: No such file or directory
# 解决方法是查看 /lib/security/pam_kwallet.so 是否存在, 安装 
sudo apt-get install kwalletmanager
# 后依然存在问题, 通过查找
find /lib -name "pam_kwallet.so"
# 发现其位置在 /lib/aarch64-linux-gnu/security/pam_kwallet.so 于是通过软链接解决了
sudo ln -s /lib/aarch64-linux-gnu/security/pam_kwallet.so /lib/security/pam_kwallet.so
```

# 更改屏幕参数

```sh
vim /etc/profile
# 在 profile 中更改似乎不妥，有没有其他方法?
# xrandr -o 3
# xinput set-prop 7 'Coordinate Transformation Matrix' 1, 0, 0, 0, 1, 0, 0, 0, 1
```

#### 服务

```
[Unit]
Description=Xserver Autostart

[Service]
ExecStart=/usr/bin/startx
Type=simple
User=root
Restart=on-failure:10

[Install]
WantedBy=graphical.target
```

sudo systemctl daemon-reload

然后在 /root/.xsession 添加要启动的脚本

列出依赖关系:

systemctl list-dependencies graphical.target

设置主机名，之前一直启动不了原来是主机没设置? 服了
sudo hostnamectl set-hostname tx2

systemd-analyze plot > dependencies.svg

# [2023-09-05 10:30:01]

共享 windows 键盘和鼠标, 通过 USB RNDIS 共享用着就很舒服。

有 synergy 和 barrier, 前者收费，但似乎未注册也可以将就用, 都支持剪贴板功能。

synergy [windows下载](http://www.3h3.com/soft/269607.html)，ubuntu 上直接 apt 下载

安装完后可以使用一下命令方便开启, 也可通过 UI 进行配置
```sh
synergyc -f --no-tray --debug INFO --name gogo 169.254.229.20:24800
# --daemon 可作为守护进程总是在后台运行
# ps -e 或 top 查看进程信息并通过 kill 关闭
```

barrier [开源地址](https://github.com/debauchee/barrier)， 提供 win 和 mac 安装包, Linux 可通过源码自行编译安装 (注意 git 后更新 git 模块)

共享原理就像多个桌面一样, 鼠标移到桌面边缘就会进入另一个桌面，这时鼠标和键盘被另一个桌面独占了。非常方便。

> 建议: 配置时把 OpenSSL 关掉可以避免很多麻烦
> TODO: 似乎有另一种方案 USB/IP 可以通过网络共享整个 USB ?

另: ubuntu 自带的 onboard 虚拟键盘就很不错的

# [2023-09-05 14:41:57] 

似乎发现 lcd 无法显示的原因是因为lcd驱动问题。无法二次初始化，到第二次启动 startx 就失败了

而且这个文件 /etc/X11/xorg.conf 关系着驱动的加载，不能去掉。hdmi 的时候可以直接删除是因为 hdmi 更通用一些。


# [2023-09-05 17:42:42]

总结下来问题是只能 root 启动 startx 才能初始化屏幕, 目前不知道怎么解决。但是神奇的是 sddm 是可以通过 user 进入桌面的，

调试方面可以使用 `sudo journalctl -u sddm  -r`, 然后发现在启动 Xserver 后会以 root 的身份执行 `/usr/share/sddm/scripts/Xsetup` 里面的脚本.

于是把APP放里面，APP得到了root身份执行。至于 /etc/sddm.conf 则启动 xfce。这样一来当应用挂掉的话就可以以user的身份进入 xfce4 桌面。听起来不错。

但是 sddm 还是太臃肿了。我放弃了它。直接使用自己的 service，以 root 身份就以 root 身份吧。也不用回到桌面了。最多带个 i3 反正都是黑不溜秋的，即使APP挂掉，使用者也不会知道自己其实已经来到一个 Linux 桌面了。
