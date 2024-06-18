#!/bin/bash

service serial-getty@ttyGS0 stop

umtprd_pid=$(ps -e | awk "\$4==\"umtprd\" {print \$1}")

if [ "$umtprd_pid" ]; then
    kill $umtprd_pid
    echo "Umtprd is running, Now we kill it."
fi

cd /sys/kernel/config/usb_gadget
echo "" > l4t/UDC

if [ -f "/dev/ffs-mtp/ep0" ]; then
    umount /dev/ffs-mtp
fi

rmdir l4t/configs/c.1/strings/0x409
rm -f l4t/configs/c.1/ffs.mtp
rmdir l4t/functions/ffs.mtp
rm -f l4t/configs/c.1/acm.GS0
rmdir l4t/functions/acm.GS0
rmdir l4t/configs/c.1/
rmdir l4t/strings/0x409
rmdir l4t

# /opt/nvidia/l4t-usb-device-mode/nv-l4t-usb-device-mode-start.sh
