


# 2023-08-24 19:32:12

参照源码包中的 panel-s-wuxga-8-0 进行修改

## 源码编译

解压源码 tar -xf ../packages/public_sources.tbz2 -C Linux_for_Tegra/source/

在 Linux_for_Tegra/source 中执行 nv_src_build.sh 可将源码导出, 需要设置工具链和板子:

```sh
# 这里直接使用包管理下载好的
apt-get install gcc-aarch64-linux-gnu 
apt-get install gcc-arm-linux-gnueabihf
export CROSS_COMPILE_ARM_PATH=/usr
export CROSS_COMPILE_AARCH64_PATH=/usr
export NV_TARGET_BOARD=jetson-tx2-devkit
```

nv_src_build.sh 过程会顺带执行编译。

这时 kernel 被解压到了 src_out 目录，后续可以直接在 src_out 内核源码中执行 ./nvbuild.sh 进行编译

有一个错误, 在  kernel/nvidia/drivers/net/wireless/realtek/rtl8812au/hal/rtl8812a/usb/rtl8812au_xmit.c

```c
while ((rtw_read32(adapter, REG_PKTBUF_DBG_CTRL) & BIT23) == 1)

改为 while ((rtw_read32(adapter, REG_PKTBUF_DBG_CTRL) & BIT23))
```

## 烧写程序

```sh
sudo ./flash.sh jetson-tx2-devkit mmcblk0p1
```


TX2 根就是这个设备树, 顶层设备树文件 tegra186-quill-p3310-1000-c03-00-base
```sh
tegra186-quill-p3310-1000-c03-00-base.dts
sudo find ./ -name "*p3310-1000-c03-00-base.dts"

# 然后看到, 打开该文件
# #include "tegra186-quill-p3310-1000-a00-00-base.dts"

# 仅更新设备树, 根据提示, 需将编译好的设备树文件放到 kernel/dtb, 一直放到的是 kernel, 因为该目录下也有该文件, 一直被误导了
# Copy /home/shino/TX2/dk/Linux_for_Tegra/kernel/dtb/tegra186-quill-p3310-1000-c03-00-base.dtb to /home/shino/TX2/dk/Linux_for_Tegra/kernel/dtb/tegra186-quill-p3310-1000-c03-00-base.dtb.rec
```


# [2023-09-06 21:03:09]

Nvidia Jetson Orin：[开发技巧](https://blog.csdn.net/u010018991/article/details/131092069)

```sh

# DTS很多，你要修改哪一个？
sudo dmesg | grep dts

# 设备树调试过程中，如何检查我的更改/补丁是否在dts中生效？
cd /proc/device-tree
xxd i2c@7000c000/status

# 在 vim 中查看
dtc -I fs -O dts /sys/firmware/devicetree/base >> ~/curent_device_tree.txt && vim ~/curent_device_tree.txt
```
