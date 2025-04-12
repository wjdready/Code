

## 备份和恢复Android手机分区的方法

[参考](https://www.jianshu.com/p/232d1522f2ce)

```sh
ls dev/block/platform/bootdevice/by-name -l
cat proc/partitions
# 备份分区，根据上面两条命令得出一下命令参数
dd if=/dev/block/mmcblk0p7 of=/sdcard/boot.img bs=1024 count=32768
# 恢复备份的分区
adb pull /sdcard/boot.img .
dd of=/dev/block/mmcblk0p5 if=/sdcard/boot.img bs=1024 count=32768
```

## USB gadget

https://gitcode.com/gh_mirrors/an/android-usb-gadget.git


## 联发科 MTK

烧录器官方地址: https://spflashtools.com


## adb 获取 App 应用

```shell
# 监视当前活动的应用，打开该应用
adb shell am monitor

# 然后查询该应用完整路径
adb shell pm path com.ahyq.multichannel

# 从而获取该应用
adb pull /data/app/com.ahyq.multichannel-2/base.apk

# 获取失败也可以先通过 shell 拷贝到 sdcard 上，然后再拷贝到本地
adb pull /sdcard/base.apk

adb shell pm list packages       # 列出所有app
adb shell pm list packages -3    # 列出除了系统应用的第三方应用包名

adb install test.apk             # 安装应用, -r 安装到 SD 卡
adb uninstall cn.com.test.mobile # 卸载应用，需要指定包

# 启动某个应用
pm list packages | grep launcher3
dumpsys package com.android.launcher3
am start -n com.android.launcher3/.Launcher
```
