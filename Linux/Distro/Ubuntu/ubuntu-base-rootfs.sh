#!/bin/bash  

# 检查 _rootfs 文件夹是否存在, 并自动下载 ubuntu-base 源码
function download()
{
    if [ ! -d "_rootfs" ]; then
        wget -c https://cdimage.ubuntu.com/ubuntu-base/releases/20.04.5/release/ubuntu-base-20.04.5-base-arm64.tar.gz
        mkdir -p _rootfs
        tar -xf ubuntu-base-20.04.5-base-arm64.tar.gz -C _rootfs
    fi
}

function mount_roofs()
{
    sudo mount -t proc /proc _rootfs/proc
    sudo mount -t sysfs /sys _rootfs/sys
    sudo mount -o bind /dev _rootfs/dev
    sudo mount -o bind /dev/pts _rootfs/dev/pts
    cp /etc/resolv.conf _rootfs/etc/resolv.conf
}

function umount_roofs()
{
    sudo umount _rootfs/proc
    sudo umount _rootfs/sys
    sudo umount _rootfs/dev/pts
    sudo umount _rootfs/dev
    rm _rootfs/etc/resolv.conf
    echo "umount rootfs"
}

function mkrootfs()
{
    mount_roofs
    cat << EOF | chroot _rootfs /bin/bash

    # DEBIAN_FRONTEND 变量保证安装时如遇到交互式配置, 使用默认配置
    export DEBIAN_FRONTEND=noninteractive
    sed -i 's/ports.ubuntu.com/mirrors.ustc.edu.cn/g' /etc/apt/sources.list
    apt-get update
    apt-get install systemd -y
    apt-get install vim net-tools dhcpcd5 kmod -y

    # 安装 sudo 并配置, 这里配置成 4755, 注意如果是文件夹拷贝的方式需加 -p 保留文件权限
    apt-get install sudo -y
    chown root:root /usr/bin/sudo
    chmod 4755 /usr/bin/sudo

    # 确保 init 被指定为 systemd
    ln -s /lib/systemd/systemd /sbin/init

    # 添加一个用户
    echo "Add a user shino"
    useradd shino -s /bin/bash -m
    echo -e "asdasd\nasdasd" | passwd shino
    usermod -a -G sudo shino

    # 在任何 tty 开启自动登录 (当然需要比如 systemctl enable getty@ttyAMA0.service )
    sed -i '/^ExecStart/s/.*/ExecStart=-\/sbin\/agetty --autologin shino --noclear %I \$TERM/' /lib/systemd/system/getty@.service
    
    # 手动创建软连接, 其实 systemctl enable getty@ttyAMA0.service 就是创建软连接
    # ln -s /lib/systemd/system/getty@.service /etc/systemd/system/getty.target.wants/getty@.service
    
	# 自定义登录服务, 原理和 /lib/systemd/system/getty@.service 一样 (但目前有问题, 总是 Waiting jobs..., 虽然已经登录上了);
    # 这里 - 可以忽略前面的制表符, 不被写到文件, EOF 加双引号可以避免在嵌套的 EOF 中 \$TERM 仍被展开
#     cat <<- "INNER_EOF" > /etc/systemd/system/getty@.service
# 		[Unit]
# 		Description=Getty on ttyAMA0
# 		[Service]
# 		ExecStart=-/sbin/agetty --noissue --autologin shino %I \$TERM
# 		Type=idle
# 		[Install]
# 		WantedBy=getty.target
# INNER_EOF
    # 使能在 ttyAMA0 登录
    systemctl enable getty@ttyAMA0.service
    echo "Leave rootfs"
EOF
    umount_roofs
}

# 下载 ubuntu-base
download

# 构建 rootfs
mkrootfs

