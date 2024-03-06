

#### 基础镜像

```sh
sudo ./rootfs_base.sh
sudo ./apply_binaries.sh

sudo ./flash.sh jetson-tx2-devkit mmcblk0p1
```

#### 基于基础镜像

```sh
sudo vim /etc/systemd/system/AWA6280.service
# [Unit]
# Description=Xserver Autostart
# [Service]
# ExecStart=/usr/bin/startx
# User=root
# [Install]
# WantedBy=graphical.target

sudo systemctl enable AWA6280

# 希望进入命令行后能自动导出 DISPLAY 变量
echo "export DISPLAY=:0" >> ~/.bashrc

chmod +x ~/start.sh

# 让 x11 启动后执行我们的 start.sh
sudo su
echo "/home/AWA/start.sh" > /root/.xsession
touch /root/.config/i3/config

# 修改关机键行为
echo "HandlePowerKey=ignore" >> /etc/systemd/logind.conf

# 设置网络, 防止自动设置 IP
sudo vim /etc/NetworkManager/NetworkManager.conf
# 改为 managed=true

# 然鹅，设置这个后 usb 网络无法使用了(删掉就可以，但是 eth0 ip 会变, 故只能舍弃 rndis), 不知
sudo vim /etc/network/interfaces
# auto eth0
# iface eth0 inet static
# address 192.168.3.102
# netmask 255.255.255.0

# 直接复制 cuda 库到 /usr/lib 更简单直接
sudo mv /usr/local/cuda-10.2/targets/aarch64-linux/lib/* /usr/lib
```

#### 启动脚本

~/start.sh

```sh
#!/bin/bash

# 设置屏幕方向
xrandr -o right

# 禁用屏幕保护程序
xset s off

# 禁用屏幕电源管理功能
xset -dpms

# 配置网口信息
ifconfig eth0 192.168.3.102 netmask 255.255.255.0

mkdir /home/AWA/tfcard -p
mount /dev/mmcblk2p1 /home/AWA/tfcard

systemctl start usb-mtp.service

# 进入恢复模式
if [ -f "/home/AWA/data/.AWA6280_recovery_mode" ]; then
    rm -f /home/AWA/data/.AWA6280_recovery_mode
    reboot --force forced-recovery

# 进入调试模式
elif [ -f "/home/AWA/data/.AWA6280_debug_mode" ]; then
    service serial-getty@ttyGS0 start

# 正常启动
else
    service serial-getty@ttyGS0 stop
    # 运行 6280 的守护进程
    /home/AWA/6280/Daemon/Daemon
fi

i3
```

#### 安装依赖


```sh
apt-get install libcufft-10-2 libopencv libopencv-core3.2 libQt5Widgets libqt5sql5 \
    libopencv-imgproc3.2 libopencv-videoio3.2 libopencv-highgui3.2 libqt5printsupport5 libfftw3-3

# 支持 ntfs 和 exfat 文件系统
sudo apt-get install ntfs-3g exfat-fuse exfat-utils
```

# 开发调试用的

```sh
# 重新配置 i3
sudo su
i3-config-wizard

sudo apt-get install synergy
sudo synergyc -f --no-tray --debug INFO --name 6280 172.20.10.3:24800

sudo apt-get remove gnome-terminal
sudo apt-get autoremove

# 安装一些实用的软件
# xfce4-terminal nautilus gredit

# 开发用到的库
sudo apt-get install libqt5serialport5-dev qtmultimedia5-dev 

sudo apt-get install libopencv-core-dev libopencv-calib3d-dev libopencv-objdetect-dev libopencv-photo-dev \
    libopencv-shape-dev libopencv-stitching-dev libopencv-superres-dev libopencv-videostab-dev libopencv-viz-dev 

vim /etc/apt/sources.list.d/nvidia-l4t-apt-source.list
# deb https://repo.download.nvidia.com/jetson/common r32.7 main
# deb https://repo.download.nvidia.com/jetson/<SOC> r32.7 main
sudo apt-get update
sudo apt-get install libcufft-dev-10-2

# zsh
sudo apt-get install zsh
sudo chsh -s /usr/bin/zsh
sh -c "$(wget -O- https://gitee.com/pocmon/ohmyzsh/raw/master/tools/install.sh)"

# 中文支持
vim /etc/default/locale
LANG="zh_CN.UTF-8"
LC_ALL="zh_CN.UTF-8"

sudo apt-get install language-pack-zh-hans

# 输入法, 其实自带输入法也够用, 配置在窗口共享状态后按 Shift 可以将输入一般的英文输入到当前文本，还是不错的
sudo apt-get install fcitx fcitx-bin fcitx-table fcitx-table-all

# 因为使用 root 进入桌面，希望打开终端默认进入的是 user, 那么可在进入终端时执行
vim /root/entry_terminal.sh
# cd /home/AWA
# su AWA
```

#### 克隆镜像与用于生产的系统包

```sh
# 现在发现可以直接在系统中进入恢复模式
sudo reboot --force forced-recovery

# 进行克隆, 克隆完成后生产两个文件, 一个是原始分区文件 .raw 另一个是 Spare image 重整的大小后的
sudo ./flash.sh -r -k APP -G system.img jetson-tx2-devkit mmcblk0p1

# 将克隆的重整镜像替换系统镜像
sudo cp system.img bootloader/system.img

# 只烧录不制作, -r 可跳过 system 制作过程, 直接使用给定的
sudo ./flash.sh -r jetson-tx2-devkit mmcblk0p1

# 打包生产
# wget -c https://developer.nvidia.com/embedded/l4t/r32_release_v6.1/t186/secureboot_r32.6.1_aarch64.tbz2
# tar -xf secureboot_r32.6.1_aarch64.tbz2 -C ..

# # 将生成 mfi_jetson-tx2-devkit.tar.gz 这个过程会重新制作 system.img, 需要将自己的 system 过去
# sudo ./tools/kernel_flash/l4t_initrd_flash.sh --no-flash --massflash jetson-tx2-devkit mmcblk0p1

# sudo cp system.img ./mfi_jetson-tx2-devkit/tools/kernel_flash/images/internal/system.img
# tar -czf AWA6280_System_V1.0.0.tar.gz mfi_jetson-tx2-devkit

mkdir -p AWA6280_System/rootfs
cp -r bootloader kernel flash.sh AWA6280_System 
cp jetson-tx2-devkit.conf l4t_sign_image.sh p2771-0000.conf.common AWA6280_System
tar -czf AWA6280_System_V1.0.1.tar.gz AWA6280_System
```

然后根目录下的系统烧录程序

```sh
#!/bin/bash

function flash()
{
	sudo sudo ./flash.sh -r jetson-tx2-devkit mmcblk0p1
}

while true; do
	flash
	echo -e "\n按回车键继续...\n"
	read 
done
```
