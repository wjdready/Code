#!/bin/bash

tf_state_old=0
tf_state=0

mkdir /home/AWA/tfcard -p

# 第一次启动时就检测到

if [ -e "/dev/mmcblk2p1" ]; then
    mount /dev/mmcblk2p1 /home/AWA/tfcard
    tf_state=1
    tf_state_old=1
    # 插入该行
    sed -i '/^storage/ a\storage "\/home\/AWA\/tfcard" "TFCARD" "rw"' /home/AWA/6280/usb-mode/umtprd.conf
else
    # 删除该行
    sed -i '/\/home\/AWA\/tfcard.*TFCARD.*rw/d' /home/AWA/6280/usb-mode/umtprd.conf
fi

systemctl restart usb-mtp.service

# 重启 usb 模式后需要重新设置虚拟串口功能
if [ -f "/home/AWA/data/.AWA6280_debug_mode" ]; then
    service serial-getty@ttyGS0 start
else
    service serial-getty@ttyGS0 stop
fi

while true; do
    if [ -e "/dev/mmcblk2p1" ]; then
        tf_state=1
    else
        tf_state=0
    fi

    if [ $tf_state -ne $tf_state_old ]; then
        if [ $tf_state -eq 1 ]; then
            echo "TF card inserted"
            sed -i '/^storage/ a\storage "\/home\/AWA\/tfcard" "TFCARD" "rw"' /home/AWA/6280/usb-mode/umtprd.conf
            umount /home/AWA/tfcard
            mount /dev/mmcblk2p1 /home/AWA/tfcard
            systemctl restart usb-mtp.service
        else
            echo "TF card removed"
            sed -i '/\/home\/AWA\/tfcard.*TFCARD.*rw/d' /home/AWA/6280/usb-mode/umtprd.conf
            umount /home/AWA/tfcard
            systemctl restart usb-mtp.service
        fi

        # 重启 usb 模式后需要重新设置虚拟串口功能
        if [ -f "/home/AWA/data/.AWA6280_debug_mode" ]; then
            service serial-getty@ttyGS0 start
        else
            service serial-getty@ttyGS0 stop
        fi
    fi

    tf_state_old=$tf_state
    sleep 1
done
