#!/bin/bash  

ROOTFS=rootfs

# 检查 $ROOTFS 文件夹是否存在, 并自动下载 ubuntu-base 源码
function download()
{
    if [ ! -d "$ROOTFS" ]; then
        wget -c https://cdimage.ubuntu.com/ubuntu-base/releases/18.04.5/release/ubuntu-base-18.04.5-base-arm64.tar.gz
        mkdir -p $ROOTFS
        tar -xf ubuntu-base-18.04.5-base-arm64.tar.gz -C $ROOTFS
    fi
}

function mount_roofs()
{
    sudo mount -t proc /proc $ROOTFS/proc
    sudo mount -t sysfs /sys $ROOTFS/sys
    sudo mount -o bind /dev $ROOTFS/dev
    sudo mount -o bind /dev/pts $ROOTFS/dev/pts
    cp /etc/resolv.conf $ROOTFS/etc/resolv.conf
}

function umount_roofs()
{
    sudo umount $ROOTFS/proc
    sudo umount $ROOTFS/sys
    sudo umount $ROOTFS/dev/pts
    sudo umount $ROOTFS/dev
    rm $ROOTFS/etc/resolv.conf
    echo "umount rootfs"
}

function mkrootfs()
{
    mount_roofs
    cat << EOF | chroot $ROOTFS /bin/bash

    # DEBIAN_FRONTEND 变量保证安装时如遇到交互式配置, 使用默认配置
    export DEBIAN_FRONTEND=noninteractive
    sed -i 's/ports.ubuntu.com/mirrors.ustc.edu.cn/g' /etc/apt/sources.list
    apt-get update
    apt --fix-broken install -y
    apt-get install systemd -y

    # 安装基本工具包
    apt-get install vim net-tools dhcpcd5 network-manager openssh-server -y

    # 安装 APP 运行时依赖
    # apt-get install libopencv -y

    # X11
    apt-get install xorg i3 fonts-wqy-zenhei -y

    # TX2 的各种依赖包
    apt-get libwrap0 libmnl0 libnl-genl-3-dev libpcsclite-dev libssl1.0-dev libelf-dev openssh-server libparted-dev -y
    apt-get install inetutils-ping wpasupplicant parted kmod xxd iproute2 mtd-utils isc-dhcp-client -y 
    apt-get install python2.7 device-tree-compiler libegl1-mesa libdatrie1 libgles2 libharfbuzz0b libpangocairo-1.0-0 libpangoft2-1.0-0 \
        libasound2 libgstreamer1.0-0 libwayland-cursor0 libxkbcommon0 libwayland-egl1 libgtk-3-0 libjpeg-turbo8 libinput10 libunwind8 \
        libgstreamer-plugins-base1.0-0 libgstreamer-plugins-bad1.0-0 libpython-stdlib python python-minimal -y 

    # TX2 usb 依赖
    apt-get install bridge-utils -y

    # 安装 sudo 并配置, 这里配置成 4755, 注意如果是文件夹拷贝的方式需加 -p 保留文件权限
    apt-get install sudo -y
    chown root:root /usr/bin/sudo
    chmod 4755 /usr/bin/sudo

    # 确保 init 被指定为 systemd
    ln -s /lib/systemd/systemd /sbin/init

    # 添加一个用户
    echo "Add a user AWA"
    useradd AWA -s /bin/bash -m
    echo -e "awaasdasd\nawaasdasd" | passwd AWA
    usermod -a -G sudo AWA

    # 设置主机名
    echo "127.0.0.1 localhost 6280" > /etc/hosts
    echo "6280" > /etc/hostname

    echo "Leave rootfs"
EOF
    umount_roofs
}

# 下载 ubuntu-base
download

# 构建 rootfs
mkrootfs
