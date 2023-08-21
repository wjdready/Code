

```sh
# 创建磁盘并分区
dd if=/dev/zero of=disk.img bs=1M count=32
fdisk disk.img

sudo losetup -f --show -P disk.img
sudo mkfs.ext4 /dev/loopXp1

mkdir rootfs -p
sudo mount /dev/loopXp1 rootfs

sudo grub-install --target i386-pc --boot-directory=rootfs/boot disk.img
sudo apt-get install grub-efi-ia32
sudo grub-install --target i386-efi --efi-directory=rootfs/boot

sudo cp bzImage rootfs/boot

# 配置 grup
sudo vim rootfs/boot/grub/grub.cfg
# 添加如下内容
set timeout=0
set default=0
menuentry 'Linux' {
    set root='hd0,msdos1'
    linux /boot/bzImage root=/dev/sda1 rw
    boot 
}

# 操作完成后手动删除
sudo umount rootfs
sudo losetup -d /dev/loopX
```


```sh
qemu-system-x86_64 -drive file=disk.img 
```

制作虚拟磁盘, 用于虚拟机启动

```sh
# 可以事先创建 支持 vpc vdi vmdk
qemu-img create -f vpc disk.vhd 20G
qemu-img convert -f raw -O vdi disk.img disk.vdi
```
