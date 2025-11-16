https://wiki.archlinux.org/title/Udev#Mounting_drives_in_rules


## 自动挂载 (失败，未成功)

#### 自动挂载规则

```sh
# /etc/udev/rules.d/60-usb-auto-mount.rules
ACTION=="add", KERNEL=="sd*[0-9]", SUBSYSTEM=="block", ENV{ID_BUS}=="usb", RUN+="/usr/local/bin/usb-mount.sh %k"
ACTION=="remove", KERNEL=="sd*[0-9]", SUBSYSTEM=="block", ENV{ID_BUS}=="usb", RUN+="/usr/local/bin/usb-mount.sh %k remove"
ACTION=="add", SUBSYSTEMS=="usb", SUBSYSTEM=="block", ENV{ID_FS_USAGE}=="filesystem", RUN{program}+="/usr/bin/systemd-mount --no-block --automount=yes --collect $devnode /media"
```

#### 自动挂载脚本

```sh
#!/bin/bash

# 
# /usr/local/bin/usb-mount.sh
# 

# 添加日志记录
echo "$(date) - 脚本被调用，参数: $1 $2" >> /var/log/usb-mount.log

MOUNT_BASE="/media/usb"
# USER="$(logname)"
USER="shino"
GROUP="users"

# 记录用户信息
echo "$(date) - 当前用户: $USER, 组: $GROUP" >> /var/log/usb-mount.log

mkdir -p $MOUNT_BASE

if [ "$2" != "remove" ]; then
    DEVICE="/dev/$1"
    MOUNT_POINT="$MOUNT_BASE/$1"
    mkdir -p $MOUNT_POINT
    
    echo "$(date) - 尝试挂载 $DEVICE 到 $MOUNT_POINT" >> /var/log/usb-mount.log
    mount -o rw -t vfat $DEVICE $MOUNT_POINT
    
    if [ $? -eq 0 ]; then
        echo "$(date) - 挂载成功: $DEVICE -> $MOUNT_POINT" >> /var/log/usb-mount.log
    else
        echo "$(date) - 挂载失败，尝试自动检测文件系统" >> /var/log/usb-mount.log
        mount -o rw -t auto $DEVICE $MOUNT_POINT
        ret=$?
        if [ $? -eq 0 ]; then
            echo "$(date) - 使用自动检测文件系统挂载成功" >> /var/log/usb-mount.log
        else
            echo "$(date) - 挂载失败，错误代码: $?" >> /var/log/usb-mount.log
        fi
    fi
else
    MOUNT_POINT="$MOUNT_BASE/$1"
    echo "$(date) - 尝试卸载 $MOUNT_POINT" >> /var/log/usb-mount.log
    umount -l $MOUNT_POINT 2>> /var/log/usb-mount.log
    rmdir $MOUNT_POINT 2>> /var/log/usb-mount.log
    echo "$(date) - 卸载完成" >> /var/log/usb-mount.log
fi
```

#### 重新加载 udev 规则

```sh
sudo udevadm control --reload-rules
sudo udevadm trigger
```


#### 调试排除故障

```sh
sudo journalctl -u systemd-udevd.service -r
```
