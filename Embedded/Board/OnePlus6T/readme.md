
一加 6T

# [2023-09-07 19:36:00]

              |-->fastboot
bootloader--->|-->kernel
              |-->recovery

> 按上下键+开机键强制关机和进入 fastboot (不连着 USB)

```sh
# adb 进入 fastboot 
adb reboot bootloader

# 解BL锁
fastboot oem unlock

# 切换槽 a/b
fastboot set_active a
```

fastboot 可用来 flash, 刷入加载内核或自定义镜像

[TWRP](https://twrp.me/) (Team Win Recovery Project) 它是一个开源的第三方恢复模式，提供了比原始恢复模式更多的功能和选项。Github 仓库[地址](https://github.com/TeamWin/android_bootable_recovery)

通过 fastboot 烧写第三方 Recovery。[参考](https://www.xda-developers.com/how-to-install-twrp/)

首先在官网支持的设备中找到 [OnePlus6T](https://twrp.me/Devices/OnePlus/), 下载最新的 img

```sh
# 刷入 img
fastboot boot twrp.img
```

定制的安卓系统 Omnirom, CM/LineageOS

