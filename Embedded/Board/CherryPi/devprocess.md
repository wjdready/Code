

### [2023-02-21 00:04:54]

这个板子是在优信电子买的, 链接下面只有一个[资料包](https://www.yourcee.com/filedownload/533969)

但后问了官方客服, 他还是会热情地给资料的 [链接](https://pan.baidu.com/s/17mdauNgMzGciwAJATmHYsQ), 提取码：k19q

### [2023-02-21 08:05:10]

发现官方资料包里面的源码就一个 buildroot, 有点难搞, 先烧写官方固件试试看吧...

先使用 Zadig 安装USB驱动, 官方包里工具软件目录下有相关工具, 创建新设备, FEL Device --> USB ID: 1F3A EFE8, DFU Device --> 1F3A 1010 并安装驱动

插入 USB (OTG 那个), 先按 BOOT 键不放, 再按 RST, 板子进入 FEL 模式 (注意拔掉 SD卡, 我发现 SD卡的优先级居然高于 BOOT键, 导致有 SD 卡存在的话, CPU 立刻就去启动SD卡里的系统而不管 BOOT键了)

通过这个命令可以测试FEL是否OK

```
./sunxi-fel.exe -l
USB device 002:004   Allwinner F1C100s
```

进入 FEL 模式后通过烧入 uboot 来转入 DFU 模式, 并使用 dfu 下载固件到 nand flash

```
sunxi-fel.exe -p uboot u-boot-sunxi-with-spl.bin
dfu-util.exe -R -a all -D sysimage-nand-120MB.img
```

好像不太顺利, 进入 DFU 后过一段时间 windows 报错, dfu-util.exe 也没成功, 我一开始以为需要重启电脑, 但是不想重启电脑, 后来就重新打开 Zadig, 试图 Replace Device, 过了好一会, 弹出失败, 但是后来却下载成功了...

然后发现设备管理器中能够看到 `USB download gadget` 正确列出了...



**[坑点-->]**

下载 nand 固件后启动发现 Linux 启动失败, 缺少 rootfs...

然后我发现官方客服给的百度云资料里面镜像文件竟然只有 sysimage-nand-120MB.img 了, 原本应该是一个 image 压缩包, 里面有三个镜像, 分包是 nand nor 和 sd 卡的. 而优信电子给的那个 [资料包](https://www.yourcee.com/filedownload/533969) 是之前的包, 里面就有完整镜像, 还不如直接使用优信给的那个得了...

**[<--]**

OK 现在将之前的包解压, 使用工具里面的 Win32DiskImage 烧写sd镜像, 板子顺利启动了, 发现系统是 MangoPi R3 的, 登录用户直接 root 不需要密码.

### [2023-02-21 11:52:04]

我发现官方完全就是搬运 MangoPi R3 的内容, MangoPi R3 有一个[入门教程](https://wiki.dfrobot.com.cn/_SKU_DFR0780_MangoPi-R3), 里面的内容完全适合该开发板. 

**[重拾建议]** 后面重拾的话, 建议直接使用 MangoPi R3 的相关教程来进行实验

根据上面的[入门教程](https://wiki.dfrobot.com.cn/_SKU_DFR0780_MangoPi-R3)给的 [SPI固件下载](https://download.dfrobot.top/f1c/SPI_firmwareV0.2.zip) 可以很快验证功能.

下载原理和上面提到的差不多, 先进入 FEL, 然后烧入一个临时的 uboot 到内存, 该 uboot 将 USB 转成 DFU 模式, 然后通过 dfu-util 来完成不同分区的烧写操作.

操作如下: 
1. 板子进入 FEL 模式
3. 直接执行 fel-flash-all-i2c-spi.bat 脚本会自带进入 DFU 然后将所有文件写入 nand 不同分区

进入系统后登录 root 密码 dfrobot

发现系统默认开启 USB 网卡功能， 只需下载[windows 端 RNDIS 驱动](http://download.dfrobot.top/f1c/RNDIS_driver.zip) 安装即可...

然后启动系统后通过 ifconfig 就能看到 USB 网络, 电脑端可以直接 ssh 连接.

### [2023-02-22 10:14:30]

经过了一段时间的摸索, 我发现官方 buildroot 其实是基于一个开源的[buildroot-tiny200](https://github.com/aodzip/buildroot-tiny200), 一开始我看见 buildroot 我就很拒绝, 因为之前编译过 buildroot 源代码, 即使科学上网了, 编译还要好几个小时，最后还失败了, 后来对 buildroot 印象就特差. 但这次我抱着试试的态度尝试编译 buildroot， 因为我打算和之前一样, 后面直接使用 Lichee Pi 源码来编译. 但没想到这次还挺快的, 而且输出还非常贴心, 各种 rootfs 格式都给输出了(这个应该在 buildroot 可以设置), 特别是 `sysimage-sdcard.img`, 里面直接包含了 kernel, 设备树和rootfs, 使用 dd 命令可以直接烧写到 SD 卡.

```
dd if=sysimage-sdcard.img /dev/sdb bs=10M
```

当然, 若在 windows 上还可以使用 Win32DiskImage 或 balenaEtcher 进行烧写.

关于 F1C200s SD 卡启动原理, 其实是官方内置在芯片的 bootloader 会自动检测 SD 卡并从 SD 卡指定扇区中加载 SPL 代码, 当然前面还需加一些 Magic code 防止误加载, 加载到内存的 SPL 会进一步加载 uboot, 或者直接将 SPL 内置到 uboot 中, buildroot 生成的 `u-boot-sunxi-with-spl.bin` 就是这么干的.

**[坑点-->]**

我发现 buildroot-tiny200 生成的 `sysimage-sdcard.img` 烧写后启动内核的参数 root 并没有指向 mmcblk0, 而是执行默认的 mmcblk0p2, 导致 Kernel panic 出错在挂载根目录上, 当然我知道可以改 u-boot 的内核启动参数或者 dts 中的 bootargs 属性.

然后我全局搜索 `/dev/mmcblk0p2` 定位到了 `board/widora/mangopi/r3/devicetree/linux/devicetree.dts`, 改成 mmcblk0p3 后发现还是启动失败. 但是 mmcblk0 确实列出来了.

困扰了好久才发现启动参数中还有 `rootfstype=squashfs`, 难怪 kernel 说 unknown-block(179,3), 因为 mmcblk0p3 是 ext4 而你却把它强行说成 squashfs, kernel 自然无法识别.

最后只要删掉 `rootfstype=squashfs` 或改成 ext4 即可.

**[<--]**

OK 现在正常启动, 登录 root 无密码

我发现默认启动的 USB 是 MTP 模式的, 不过这次不同的是, 这个 MTP 是在内核启动后应用层设置的, 那么我应该可以进行切换...


### USB 调试

上文中 USB 默认的 MTP 到底是如何设置成功的呢, 我觉得应该有与 gadget 的相关 config 文件, 但是它们在哪呢? 

于是我尝试使用以下命令搜索:

```
# find / -name "*gadget*"
/sys/kernel/config/usb_gadget
/sys/devices/platform/soc/1c13000.usb/musb-hdrc.1.auto/gadget
```

应该就是这个了吧, 默认是在 kernel/config 中设置, 那如何自定义配置成其他模式呢?

为此, 我找到了[官方文档](https://www.kernel.org/doc/html/next/usb/gadget_configfs.html)

从官方文档中我们了解到, 要自定义 gadget, 可先挂载 configfs 到一个目录, 然后通过 mkdir 新建一个 usb_gadget 的 config:

```shell
export CONFIGFS_HOME=/root/configfs
mount none $CONFIGFS_HOME -t configfs

# 创建一个 g2 因为上面默认的那个 MTP 的配置就是 g1
# 在 configfs 中使用 mkdir 命令, configfs 会自动帮我们创建好配置模板
mkdir $CONFIGFS_HOME/usb_gadget/g2
cd $CONFIGFS_HOME/usb_gadget/g2

# 可以看到 configfs 帮我们创建了一系列文件, 现在要做的就是对这些文件进行自定义配置
ls
# UDC              bMaxPacketSize0  functions        strings
# bDeviceClass     bcdDevice        idProduct
# bDeviceProtocol  bcdUSB           idVendor
# bDeviceSubClass  configs          os_desc

```

###### 1. USB 设备基本信息

```shell
# 每个 gadget 都应给出其供应商ID和产品ID:
echo <VID> > idVendor
echo <PID> > idProduct

# 同时还需要给出 Vendor 和 Product 的相关描述字符串以及序列号
# 为了有一个存储它们的位置，必须为每种语言创建一个 strings 的子目录，例如:
# (美式英语是0x0409, 中文 0x1404)
mkdir strings/0x409

# 然后就可以写入上述信息
echo <serial number> > strings/0x409/serialnumber
echo <manufacturer> > strings/0x409/manufacturer
echo <product> > strings/0x409/product
```

###### 2. 创建配置 (configurations)

```shell
# 每个 gadget 将由许多配置组成，必须创建其对应的目录：
# mkdir configs/<name>.<number>
mkdir configs/c.1

# 每种配置还需要其字符串描述，因此必须为每种语言创建子目录
mkdir configs/c.1/strings/0x409

# 然后就可以写入关于该配置的描述
echo <configuration> > configs/c.1/strings/0x409/configuration

# 此外还可以为配置设置一些属性，例如：
echo 120 > configs/c.1/MaxPower
```

##### 3. 创建功能 (functions)

```shell
# gadget 将提供一些功能，对于每个功能，必须创建其对应的目录：
# mkdir functions/<name>.<instance name>
# usb_f_ncm.ko gets loaded with request_module()
mkdir functions/ncm.usb0
```


###### 4. 将功能 (functions) 与配置 (configurations) 相关联

```shell
# 此时，假设我们已经创建了许多 gadget，每个 gadget 都有指定的一些配置和一些可用的功能。
# 剩下的就是指定哪些功能在哪个配置中可用(相同的功能可以在多个配置中使用)。
# 这是通过创建符号链接实现的 (创建一个 functions/ncm.usb0 的软链接放到 configs/c.1 目录下)：
ln -s functions/ncm.usb0 configs/c.1
```

##### 5. 启用 gadget

```shell
# 完成上述步骤后, 示例目录结构可能如下所示:
# ./strings
# ./strings/0x409
# ./strings/0x409/serialnumber
# ./strings/0x409/product
# ./strings/0x409/manufacturer
# ./configs
# ./configs/c.1
# ./configs/c.1/ncm.usb0 -> ../../../../usb_gadget/g1/functions/ncm.usb0
# ./configs/c.1/strings
# ./configs/c.1/strings/0x409
# ./configs/c.1/strings/0x409/configuration
# ./configs/c.1/bmAttributes
# ./configs/c.1/MaxPower
# ./functions
# ./functions/ncm.usb0
# ./functions/ncm.usb0/ifname
# ./functions/ncm.usb0/qmult
# ./functions/ncm.usb0/host_addr
# ./functions/ncm.usb0/dev_addr
# ./UDC
# ./bcdUSB
# ./bcdDevice
# ./idProduct
# ./idVendor
# ./bMaxPacketSize0
# ./bDeviceProtocol
# ./bDeviceSubClass
# ./bDeviceClass

# 要启用 gadget，必须将其绑定到UDC(USB设备控制器), 在 gadget 目录下：
echo <udc name> > UDC
# <udc name> 是 /sys/class/udc/* 下的其中一个目录, 例如
echo s3c-hsotg > UDC
```

##### 6. 关闭 gadget

```shell
echo "" > UDC
```

##### 7. 清除配置或功能 (使用 rmdir)

```shell
# 解除功能与配置中的关联
rm configs/c.1/ncm.usb0

# 删除配置中的字符串目录
rmdir configs/c.1/strings/0x409

# 删除配置
rmdir configs/c.1

# 删除功能
rmdir functions/ncm.usb0

# 删除 gadget 中的字符串目录
# rmdir strings/<lang>
rmdir strings/0x409

# 最后删除 gadget 目录
rmdir g1
```

#### 通过脚本配置

因为 configfs 是保存在内存中的, 掉电后里面的信息就会消失, 因此我们需要通过脚本来进行动态配置, 例如上面 buildroot 生成的镜像中, 启动 MTP 功能 `/etc/init.d/S98uMTPrd` 配置如下:

```shell
#!/bin/sh
#
# Start uMTPrd....
#

start() {
    printf "Starting uMTPrd: Good"
    mount none /sys/kernel/config -t configfs
    mkdir /sys/kernel/config/usb_gadget/g1
    cd /sys/kernel/config/usb_gadget/g1
    mkdir configs/c.1
    mkdir functions/ffs.mtp
    mkdir strings/0x409
    mkdir configs/c.1/strings/0x409

    echo 0x0100 > idProduct
    echo 0x1D6B > idVendor

    echo "Widora" > strings/0x409/manufacturer
    echo "AWA6280" > strings/0x409/product

    echo "Conf 1" > configs/c.1/strings/0x409/configuration
    echo 120 > configs/c.1/MaxPower
    ln -s functions/ffs.mtp configs/c.1
    mkdir /dev/ffs-mtp
    mount -t functionfs mtp /dev/ffs-mtp
    start-stop-daemon -S -q -m -b -p /var/run/umtprd.pid -x /usr/sbin/umtprd
    sleep 1
    ls /sys/class/udc/ > /sys/kernel/config/usb_gadget/g1/UDC
    [ $? = 0 ] && echo "OK" || echo "FAIL"
}

stop() {
    printf "Stopping uMTPrd: "
    echo > /sys/kernel/config/usb_gadget/g1/UDC
    start-stop-daemon -K -q -p /var/run/umtprd.pid -x /usr/sbin/umtprd
    [ $? = 0 ] && echo "OK" || echo "FAIL"
}

case "$1" in
    start)
    start
    ;;
    stop)
    stop
    ;;
    restart|reload)
    stop
    sleep 1
    start
    ;;
  *)
    echo "Usage: $0 {start|stop|restart}"
    exit 1
esac

exit $?
```

由此我们可以自己实现一个 gadget 脚本如下

```shell 
#! /bin/sh
mount none /sys/kernel/config -t configfs

mkdir /sys/kernel/config/usb_gadget/g2
cd /sys/kernel/config/usb_gadget/g2

echo "0x0525" > idVendor
echo "0xa4a2" > idProduct
# 0525:a4a2 Netchip Technology, Inc. Linux-USB Ethernet/RNDIS Gadget
mkdir strings/0x409
echo "0123456789" > strings/0x409/serialnumber
echo "Netchip Technology, Inc." > strings/0x409/manufacturer
echo "Linux-USB Ethernet/RNDIS Gadget" > strings/0x409/product
mkdir functions/rndis.usb0
mkdir configs/c.1
mkdir configs/c.1/strings/0x409
echo "CDC RNDIS" > configs/c.1/strings/0x409/configuration
ln -s functions/rndis.usb0 configs/c.1

ls /sys/class/udc/ > /sys/kernel/config/usb_gadget/g2/UDC
```

ifconfig usb0 up
ifconfig usb0 192.168.9.1


```
# ifconfig
lo        Link encap:Local Loopback  
          inet addr:127.0.0.1  Mask:255.0.0.0
          UP LOOPBACK RUNNING  MTU:65536  Metric:1
          RX packets:4 errors:0 dropped:0 overruns:0 frame:0
          TX packets:4 errors:0 dropped:0 overruns:0 carrier:0
          collisions:0 txqueuelen:1000 
          RX bytes:336 (336.0 B)  TX bytes:336 (336.0 B)

usb0      Link encap:Ethernet  HWaddr 4A:78:8F:B7:1D:48  
          inet addr:192.168.9.1  Bcast:192.168.9.255  Mask:255.255.255.0
          UP BROADCAST RUNNING MULTICAST  MTU:1500  Metric:1
          RX packets:68 errors:0 dropped:20 overruns:0 frame:0
          TX packets:7 errors:0 dropped:0 overruns:0 carrier:0
          collisions:0 txqueuelen:1000 
          RX bytes:8399 (8.2 KiB)  TX bytes:1062 (1.0 KiB)
```


```shell
cd /sys/kernel/config/usb_gadget/
mkdir -p multigadget
cd multigadget
echo 0x04b3 > idVendor
echo 0x4010 > idProduct
echo 0x0100 > bcdDevice # v1.0.0
echo 0x0200 > bcdUSB # USB2
mkdir -p strings/0x409
echo "badc0deddeadbeef" > strings/0x409/serialnumber
echo "Knoppix Industries" > strings/0x409/manufacturer
echo "USB Everything" > strings/0x409/product
mkdir -p configs/c.1/strings/0x409
echo "0x80" > configs/c.1/bmAttributes
echo 250 > configs/c.1/MaxPower
echo "Config 1: RNDIS network, Mass Storage and Serial Device" > configs/c.1/strings/0x409/configuration

echo "1" > os_desc/use
echo "0xcd" > os_desc/b_vendor_code
echo "MSFT100" > os_desc/qw_sign
# Network function (RNDIS)
mkdir -p functions/rndis.usb0
echo "42:61:64:55:53:42" > functions/rndis.usb0/dev_addr
echo "48:6f:73:74:50:42" > functions/rndis.usb0/host_addr
echo RNDIS > functions/rndis.usb0/os_desc/interface.rndis/compatible_id
echo 5162001 > functions/rndis.usb0/os_desc/interface.rndis/sub_compatible_id
ln -s functions/rndis.usb0 configs/c.1/


# add more functions, same device
# Alternate network card
mkdir -p functions/ecm.usb0
echo "42:61:64:55:53:43" > functions/ecm.usb0/dev_addr
echo "48:6f:73:74:50:43" > functions/ecm.usb0/host_addr
ln -s functions/ecm.usb0 configs/c.1/
# Serial device
mkdir -p functions/acm.gs0
# Auf dem Pi eintragen/ausführen, um das Login zu aktivieren
# sudo systemctl enable getty@ttyGS0.service
# sudo systemctl start getty@ttyGS0.service
# Device unter Linux (PC): /dev/ttyACM0
# Device auf dem Pi: /dev/ttyGS0
ln -s functions/acm.gs0 configs/c.1/

# Hard Disk
mkdir -p functions/mass_storage.usb0/lun.0
echo 0 > functions/mass_storage.usb0/stall
echo 0 > functions/mass_storage.usb0/lun.0/cdrom
echo 0 > functions/mass_storage.usb0/lun.0/ro
echo 0 > functions/mass_storage.usb0/lun.0/nofua
echo /data.img > functions/mass_storage.usb0/lun.0/file
ln -s functions/mass_storage.usb0 configs/c.1/
# *** End functions
ln -s configs/c.1 os_desc/
ls /sys/class/udc/ > UDC
# Network masquerading etc.
iptables -t nat -I POSTROUTING -j MASQUERADE
echo 1 > /proc/sys/net/ipv4/ip_forward
# Hinweis: auch dnsmasq konfigurieren, damit die USB-NW-Karten eine IP-Adresse
verteilen
```


[USB 压力测试?](https://www.gl2.cc/1160.html)