#!/bin/bash

function create_mtp_device {

    mkdir -p /sys/kernel/config/usb_gadget/l4t

    cd /sys/kernel/config/usb_gadget/l4t

    mkdir -p configs/c.1
    mkdir -p functions/ffs.mtp
    mkdir -p strings/0x409
    mkdir -p configs/c.1/strings/0x409

    echo 0x0100 >idProduct
    echo 0x1D6B >idVendor

    echo "Widora" >strings/0x409/manufacturer
    echo "AWA6280" >strings/0x409/product

    echo "Conf 1" >configs/c.1/strings/0x409/configuration
    echo 120 >configs/c.1/MaxPower
    ln -s functions/ffs.mtp configs/c.1

    if [ -f /proc/device-tree/serial-number ]; then
        serialnumber="$(cat /proc/device-tree/serial-number|tr -d '\000')"
    else
        serialnumber=no-serial
    fi

    echo "${serialnumber}" > strings/0x409/serialnumber

    mkdir -p functions/acm.GS0
    ln -sf functions/acm.GS0 configs/c.1

    service serial-getty@ttyGS0 stop
}

function main {

    # /opt/nvidia/l4t-usb-device-mode/nv-l4t-usb-device-mode-stop.sh

    script_dir="$(cd "$(dirname "$0")" && pwd)"

    if [ ! -d "/sys/kernel/config/usb_gadget/l4t" ]; then
        create_mtp_device
    fi

    if [ ! -f "/dev/ffs-mtp/ep0" ]; then
        mkdir -p /dev/ffs-mtp 
        mount -t functionfs mtp /dev/ffs-mtp
    fi

    [ ! -f "$script_dir/umtprd" ] && echo "ERROR: We can not find umtprd in $script_dir" && exit -1

    [ ! -f "$script_dir/umtprd.conf" ] && echo "ERROR: We can not find umtprd.conf in $script_dir" && exit -1

    mkdir -p /etc/umtprd
    cp $script_dir/umtprd.conf /etc/umtprd/umtprd.conf

    umtprd_pid=$(ps -e | awk "\$4==\"umtprd\" {print \$1}")

    if [ "$umtprd_pid" ]; then
        kill $umtprd_pid
        echo "Umtprd is running, Now we restart it."
    fi

    $script_dir/umtprd &

    sleep 1

    UDC=$(cat /sys/kernel/config/usb_gadget/l4t/UDC)

    if [ "$UDC" == "" ]; then
        ls /sys/class/udc/ >/sys/kernel/config/usb_gadget/l4t/UDC
    fi

    echo "MTP Started!"
}

main
