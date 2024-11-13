

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
