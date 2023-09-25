

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
```

启动脚本

~/start.sh

```sh
#!/bin/bash

# 设置屏幕方向
xrandr -o right

# 如果存在 .AWA6280_debug_mode 文件, 则开启 ssh 调试功能
if [ -f "/home/AWA/data/.AWA6280_debug_mode" ]; then
    /opt/nvidia/l4t-usb-device-mode/nv-l4t-usb-device-mode-start.sh
else
    systemctl start usb-mtp.service
    service serial-getty@ttyGS0 stop
fi

# 运行 6280 的守护进程
/home/AWA/6280/Daemon/Daemon &

# 运行 6280 主程序
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/cuda-10.2/targets/aarch64-linux/lib
/home/AWA/6280/App/run/Framework

# 最后运行黑不溜秋的 i3 桌面
i3
```

其中 Framework.sh

```sh
#!/bin/bash

appname=$(basename $0 .sh)
scriptpath=$(dirname $(readlink -f $0))

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$scriptpath:/usr/local/cuda-10.2/targets/aarch64-linux/lib

$scriptpath/$appname
```

#### 安装依赖


```sh
apt-get install libcufft-10-2 libopencv libopencv-core3.2 libQt5Widgets libqt5sql5 \
    libopencv-imgproc3.2 libopencv-videoio3.2 libopencv-highgui3.2 libqt5printsupport5 libfftw3-3
```

调试用的

```sh
# 重新配置 i3
sudo su
i3-config-wizard

sudo apt-get install synergy
synergyc -f --no-tray --debug INFO --name gogo 169.254.229.20:24800

sudo apt-get remove gnome-terminal
sudo apt-get autoremove

# 安装一些实用的软件
# xfce4-terminal nautilus gredit
```

#### 克隆镜像与用于生产的系统包

```sh
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
tar -czf AWA6280_System_V1.0.0.tar.gz AWA6280_Sys
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
