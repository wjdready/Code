


# 整个文件夹作为一个设备

 目前没有实现

-virtfs local,id=root,mount_tag=/dev/root,security_model=passthrough,path=/path/to/rootfs

-device virtio-net-device,netdev=usernet -netdev user,id=usernet

qemu-system-x86_64 -hda none -fsdev local,id=fsdev,path=/path/to/folder,security_model=passthrough -device virtio-9p-pci,fsdev=fsdev,mount_tag=hostshare

