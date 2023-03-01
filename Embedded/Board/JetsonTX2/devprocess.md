

### [2023-02-20 22:48:21] 起始

不得不吐槽官方找资料真的好难定位到准确位置, 之前找到过, 过了一段时间忘记了. 各个链接点来点去后来找不到位置. 找了好久, 这里就给出 [Jetson Linux 相关文档链接](https://developer.nvidia.com/embedded/jetson-linux)

在这里可以看到有官方已经编译好的驱动和示例根文件系统, 然后下面是驱动和跟文件示例的源代码. 这四组文件都要下载...

然后这里是[官方开发教程汇总在线版](https://docs.nvidia.com/jetson/archives/r35.2.1/DeveloperGuide/index.html), 也可以直接[下载离线版本](https://developer.nvidia.com/downloads/l4t-r35-release-v21-jetson-linux-driver-package-zip), 这个教程很好, 要仔细读...


#### [2023-02-23 08:04:36] 

今天重拾, 需要重新连接到调试串口, 下载官方开发板的[用户手册](https://developer.nvidia.com/embedded/downloads/#?search=Developer%20Kit%20User%20Guide).

通过之前对 CherryPi 的调试, 使我很快知道如何操作 USB gadget, 于是我找到官方板的系统下 `ls /sys/class/udc/` 果然官方默认是有 udc 驱动的了, 然后执行 `ls /sys/kernel/config/usb_gadget/` 默认还有一个 usb_gadget config.

当我连接 USB 口到电脑时, 电脑弹出了一个磁盘, 发现官方系统中已经默认有 usb mass_storage (不是 MTP) 功能了, 我们可以看一下官方默认的 usb_gadget 配置:

```
cd /sys/kernel/config/usb_gadget/l4t
ls -ld `find .`
drwxr-xr-x 6 root root    0 1月   1 09:46 .
-rw-r--r-- 1 root root 4096 9月   6  2022 ./bcdDevice
-rw-r--r-- 1 root root 4096 1月   1 09:46 ./bcdUSB
-rw-r--r-- 1 root root 4096 9月   6  2022 ./bDeviceClass
-rw-r--r-- 1 root root 4096 9月   6  2022 ./bDeviceProtocol
-rw-r--r-- 1 root root 4096 9月   6  2022 ./bDeviceSubClass
-rw-r--r-- 1 root root 4096 1月   1 09:46 ./bMaxPacketSize0
drwxr-xr-x 3 root root    0 9月   6  2022 ./configs
drwxr-xr-x 3 root root    0 1月   1 09:45 ./configs/c.1
lrwxrwxrwx 1 root root    0 9月   6  2022 ./configs/c.1/acm.GS0 -> ../../../../usb_gadget/l4t/functions/acm.GS0
-rw-r--r-- 1 root root 4096 1月   1 09:46 ./configs/c.1/bmAttributes
lrwxrwxrwx 1 root root    0 9月   6  2022 ./configs/c.1/mass_storage.0 -> ../../../../usb_gadget/l4t/functions/mass_storage.0
-rw-r--r-- 1 root root 4096 1月   1 09:46 ./configs/c.1/MaxPower
lrwxrwxrwx 1 root root    0 9月   6  2022 ./configs/c.1/ncm.usb0 -> ../../../../usb_gadget/l4t/functions/ncm.usb0
lrwxrwxrwx 1 root root    0 9月   6  2022 ./configs/c.1/rndis.usb0 -> ../../../../usb_gadget/l4t/functions/rndis.usb0
drwxr-xr-x 3 root root    0 9月   6  2022 ./configs/c.1/strings
drwxr-xr-x 2 root root    0 1月   1 09:45 ./configs/c.1/strings/0x409
-rw-r--r-- 1 root root 4096 9月   6  2022 ./configs/c.1/strings/0x409/configuration
drwxr-xr-x 6 root root    0 9月   6  2022 ./functions
drwxr-xr-x 2 root root    0 1月   1 09:45 ./functions/acm.GS0
-r--r--r-- 1 root root 4096 1月   1 09:46 ./functions/acm.GS0/port_num
drwxr-xr-x 3 root root    0 1月   1 09:45 ./functions/mass_storage.0
drwxr-xr-x 2 root root    0 1月   1 09:45 ./functions/mass_storage.0/lun.0
-rw-r--r-- 1 root root 4096 1月   1 09:46 ./functions/mass_storage.0/lun.0/cdrom
-rw-r--r-- 1 root root 4096 9月   6  2022 ./functions/mass_storage.0/lun.0/file
-rw-r--r-- 1 root root 4096 1月   1 09:46 ./functions/mass_storage.0/lun.0/inquiry_string
-rw-r--r-- 1 root root 4096 1月   1 09:46 ./functions/mass_storage.0/lun.0/nofua
-rw-r--r-- 1 root root 4096 1月   1 09:46 ./functions/mass_storage.0/lun.0/removable
-rw-r--r-- 1 root root 4096 9月   6  2022 ./functions/mass_storage.0/lun.0/ro
-rw-r--r-- 1 root root 4096 1月   1 09:46 ./functions/mass_storage.0/stall
drwxr-xr-x 2 root root    0 1月   1 09:45 ./functions/ncm.usb0
-rw-r--r-- 1 root root 4096 9月   6  2022 ./functions/ncm.usb0/dev_addr
-rw-r--r-- 1 root root 4096 9月   6  2022 ./functions/ncm.usb0/host_addr
-r--r--r-- 1 root root 4096 1月   1 09:46 ./functions/ncm.usb0/ifname
-rw-r--r-- 1 root root 4096 1月   1 09:46 ./functions/ncm.usb0/qmult
drwxr-xr-x 3 root root    0 1月   1 09:45 ./functions/rndis.usb0
-rw-r--r-- 1 root root 4096 9月   6  2022 ./functions/rndis.usb0/dev_addr
-rw-r--r-- 1 root root 4096 9月   6  2022 ./functions/rndis.usb0/host_addr
-r--r--r-- 1 root root 4096 1月   1 09:46 ./functions/rndis.usb0/ifname
drwxr-xr-x 3 root root    0 9月   6  2022 ./functions/rndis.usb0/os_desc
drwxr-xr-x 2 root root    0 1月   1 09:45 ./functions/rndis.usb0/os_desc/interface.rndis
-rw-r--r-- 1 root root 4096 9月   6  2022 ./functions/rndis.usb0/os_desc/interface.rndis/compatible_id
-rw-r--r-- 1 root root 4096 9月   6  2022 ./functions/rndis.usb0/os_desc/interface.rndis/sub_compatible_id
-rw-r--r-- 1 root root 4096 1月   1 09:46 ./functions/rndis.usb0/qmult
-rw-r--r-- 1 root root 4096 9月   6  2022 ./idProduct
-rw-r--r-- 1 root root 4096 9月   6  2022 ./idVendor
drwxr-xr-x 2 root root    0 1月   1 09:45 ./os_desc
-rw-r--r-- 1 root root 4096 9月   6  2022 ./os_desc/b_vendor_code
lrwxrwxrwx 1 root root    0 9月   6  2022 ./os_desc/c.1 -> ../../../usb_gadget/l4t/configs/c.1
-rw-r--r-- 1 root root 4096 9月   6  2022 ./os_desc/qw_sign
-rw-r--r-- 1 root root 4096 9月   6  2022 ./os_desc/use
drwxr-xr-x 3 root root    0 9月   6  2022 ./strings
drwxr-xr-x 2 root root    0 1月   1 09:45 ./strings/0x409
-rw-r--r-- 1 root root 4096 9月   6  2022 ./strings/0x409/manufacturer
-rw-r--r-- 1 root root 4096 9月   6  2022 ./strings/0x409/product
-rw-r--r-- 1 root root 4096 9月   6  2022 ./strings/0x409/serialnumber
-rw-r--r-- 1 root root 4096 1月   1 09:19 ./UDC
```

可以看到官方在该 gadget 中添加了一个配置, 该配置关联到了4种功能, 即该 USB gadget 是一个复用设备, 可以用USB同时模拟出多个功能. 从列出来的功能来看, 这些功能包括:

* `acm.GS0` USB串口通信
* `mass_storage.0` USB大容量存储设备
* `ncm.usb0` Network Control Model，用于Host和Device之间交换以太网帧
* `rndis.usb0` USB网卡

rndis.usb0 功能意味着我们可以直接通过 USB 连接到板子的 SSH 端口.

于是我们来查看一下板子的网络配置信息:

```shell
ifconfig
# ... 这里省略一些无关配置
l4tbr0: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1500
        inet 192.168.55.1  netmask 255.255.255.0  broadcast 192.168.55.255
        inet6 fe80::84f7:f4ff:fef0:6915  prefixlen 64  scopeid 0x20<link>
        inet6 fe80::1  prefixlen 128  scopeid 0x20<link>
        ether 86:f7:f4:f0:69:15  txqueuelen 1000  (以太网)

        RX packets 436  bytes 47007 (47.0 KB)

        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 96  bytes 11263 (11.2 KB)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0

rndis0: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1500

        inet6 fe80::84f7:f4ff:fef0:6915  prefixlen 64  scopeid 0x20<link>
        ether 86:f7:f4:f0:69:15  txqueuelen 1000  (以太网)

        RX packets 664  bytes 72892 (72.8 KB)
        RX errors 0  dropped 4  overruns 0  frame 0
        TX packets 118  bytes 20844 (20.8 KB)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0

usb0: flags=4099<UP,BROADCAST,MULTICAST>  mtu 1500
        ether 86:f7:f4:f0:69:17  txqueuelen 1000  (以太网)
        RX packets 0  bytes 0 (0.0 B)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 0  bytes 0 (0.0 B)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0
```

目前我比较疑惑的是, 相比之前调试的 CherryPi, 为什么这个 usb rndis 会有三个网络配置, CherryPi 中我只看到 usb0, 主要是当时也没有通信成功, 只是列出了 usb0 网络, 然后就来搞这块板子了. usb rndis 功能到底是如何配置网络的呢? 我目前尚不清楚...

不过从列出的三个网络来看, 似乎只有 l4tbr0 可以用, 因为只有它给到了 IP 地址? 反正我通过电脑 ssh 到 192.168.55.1 是可以连的, 在 Windows 中 ipconfig 也可以看到已经分配到 `192.168.55.xxx` 的 IP 地址.

然后我强行给 usb0 设置 ip 地址

```
ifconfig usb0 192.168.9.1
```

虽然板子是列出来 IP 了, 但是在 host 端 ping 该 IP 根本就 ping 不通. 那就来找一下 rndis 相关文件吧...

### [2023-02-23 16:31:10]

也是经过一番摸索, 主要是下午去搞 5920 生产时的程序快速下载问题. 然后到这个点才开始继续搞.

我怎么也不会想到 usb 的配置脚本竟然是放在 /opt 里面的, 因为一般配置不都放在 /etc 里面吗, 也有可能在 /lib 因为 systemd, 我找遍了自认为的情况却没有找 /opt, 直接找 / 太慢还崩溃了. 最后偶然间使用 /opt 才发现配置就在里面. 如下命令, 就能找到 usb 的配置:

```
grep -nr "rndis" /opt
```

不过现在想来, /opt 一般不就是放扩展包的地方吗, 官方把 usb 功能作为扩展安装包放在 /opt 其实是合理的, 因为放在 /opt 而不是 /etc 还可以避免扩展包和系统配置的耦合性.

### [2023-02-24 08:08:16]

在知道了 usb 功能的启动配置是放在 /opt 目录的情况下, 很快就可以知道其确切位置: `/opt/nvidia/l4t-usb-device-mode`, 在这个目录下有这些文件:

```
-rw-r--r-- 1 root root      130 9月   6  2022 dhcpd.conf
-rw-r--r-- 1 root root 16777216 9月   6  2022 filesystem.img
-rw-r--r-- 1 root root     2887 7月  10  2021 LICENSE.filesystem.img
-rw-r--r-- 1 root root      116 12月 26  2022 mac-addresses
-rwxr-xr-x 1 root root     6085 1月   1 21:22 nv-l4t-usb-device-mode-config.sh
-rw-r--r-- 1 root root     1811 7月  10  2021 nv-l4t-usb-device-mode-runtime.service
-rwxr-xr-x 1 root root     3141 7月  10  2021 nv-l4t-usb-device-mode-runtime-start.sh
-rwxr-xr-x 1 root root     1944 7月  10  2021 nv-l4t-usb-device-mode-runtime-stop.sh
-rw-r--r-- 1 root root     1917 7月  10  2021 nv-l4t-usb-device-mode.service
-rwxr-xr-x 1 root root     9537 1月   1 22:03 nv-l4t-usb-device-mode-start.sh
-rwxr-xr-x 1 root root     2250 7月  10  2021 nv-l4t-usb-device-mode-state-change.sh
-rwxr-xr-x 1 root root     2575 7月  10  2021 nv-l4t-usb-device-mode-stop.sh
```

看到 `.service` 这些 systemd 文件, 很快知道它是如何在开机时启动的了, 而且估计是通过 deb 包安装的, 然后被放到该目录下. 关于 deb 包的相关知识以及它是如何安装后是如何注册到系统启动服务的, 这些内容也是一个篇章, 后面有机会再研究了. 目前来说先看 USB 网卡, 大容量存储以及 MTP 等如何工作.

### USB gadget 大容量存储

从 [Mass Storage Gadget 官方介绍](https://www.kernel.org/doc/html/next/usb/mass-storage.html) 中可以知道, Mass Storage Gadget 支持多个逻辑单元 (LUN)。每个 LUN 的后备存储由常规文件或块设备提供，访问可以限制为只读，gadget 可以指示它是可移动的 或 CD-ROM（后者表示只读访问）.


#### 1. 模块参数

需要说明的是, 我们在 usb_gadget 的 configfs 中写入的参数实际上是为了方便对模块进行传参, 里面写的参数内容将被最终传递到对应的小工具模块.

mass storage gadget 允许以下模块参数:

##### 1) file=filename[,filename…]

此参数列出用于每个逻辑单元的后备存储器 (backing storage) 的文件或块设备的路径。最多可以设置 FSG_MAX_LUNS (8) 个 LUN。如果指定了更多文件，它们将被忽略。

```shell
echo "./demo.img" > "functions/mass_storage.0/lun.0/file"
echo "/dev/mmcblk2p1" > "functions/mass_storage.0/lun.0/file"
```

需要注意的是, 如果将文件用作后备存储器，则任何其他进程都应当不能修改该文件 (可把文件临时地设为只读)。这是因为将大容量存储设备连接到主机, 主机假定数据不会在不知情的情况下被修改, 虽然这时进程读取文件是可以的, 但是如果写入的话, 由于主机端缓冲区的存在, 写入的数据可能没法同步更新到主机端, 这时主机端的内容和设备端内容就没法很好统一和定义了.

逻辑单元的大小将向下舍入为完整的逻辑块。对于模拟CD-ROM的LUN，逻辑块大小为2048字节，如果后备文件 (backing file) 是块设备，则为设备的块大小，否则为512字节.

##### 2) removable=b[,b…]

此参数指定每个逻辑单元是否应为可拆卸。这里的`b`可以是`y`、`Y`或`1`表示真，也可以是`n`、`N`或`0`表示假。

```shell
echo "1" > "functions/mass_storage.0/lun.0/removable"
```

如果为逻辑单元设置了此选项，小工具将接受 `弹出` SCSI请求 (启动/停止单元)。当收到该请求时, 后备文件将关闭以模拟弹出 (这时 `functions/mass_storage.0/lun.0/file` 内容为空) ，并且主机将无法再挂载逻辑单元，直到设备上的用户空间指定了新的后备文件 (请参阅`sysfs条目`部分), 即需要重新将后备文件写到 `functions/mass_storage.0/lun.0/file`.

如果逻辑单元不可移除(默认)，则在加载模块时必须使用 `file` 参数为其指定一个后备文件。而不是模块启动后再指定文件. 如果模块是内置的，情况也是如此，没有例外。

该标志的默认值为 false，但它过去曾为 true。这已经被更改，以更好地匹配 File Storage Gadget，因为它似乎毕竟是一个更理智的默认设置。因此，为了保持与旧内核的兼容性，最好指定一个值.

请注意，`可拆卸`意味着可以弹出或移除逻辑单元的介质(CD-ROM驱动器或读卡器也是如此)。但这并不意味着整个设备都可以从主机上拔出；正确的术语是`热插拔`。

##### 3） cdrom=b[,b…]

此参数指定每个逻辑单元是否应模拟CD-ROM。默认值为 false

##### 4) ro=b[,b…]

此参数指定是否应将每个逻辑单元报告为只读。这将防止主机修改后备文件。

请注意，如果给定逻辑单元 lun.x 的此标志为 false，但后备文件无法在读/写模式下打开，则该小工具无论如何都将回退到只读模式。非CD-ROM逻辑单元的缺省值为 false；对于模拟CD-ROM的逻辑单元，它被强制设置为 true。

##### 5) nofua=b[,b…]

此参数指定是否应在发送到给定逻辑单元的SCSI Write10和Write12命令中忽略Fua标志, 默认值为 false。

通常，MS Windows 以 `Removal optimised mode (卸载优化模式)` 挂载可移动存储。对介质的所有写入都是同步的，这是通过设置SCSI WRITE(10，12)命令中的FUA(强制单元访问)位来实现的。这会强制每次写入都要等到数据实际写出，并防止I/O请求在数据块层聚合. 但这样做会显著降低性能。

请注意，如果 nofua 为true, 这可能意味着，如果设备是从USB供电的，并且用户在没有先卸载设备的情况下拔下设备(至少有些Windows用户会这样做)，数据可能会丢失。


##### 6) luns=N

此参数指定小工具将具有的逻辑单元数。 它受FSG_MAX_LUNS(8)的限制，设置更大的值将被限制在上限。如果提供此参数，并且在`file`参数中指定的文件数大于`LUNs`的值，则所有多余的文件都将被忽略。

如果该参数不存在，则逻辑单元的数量将从`file`参数中指定的文件数量中推断出来。如果文件参数也缺失，则假定只有一个文件参数。

##### 7) stall=b

指定是否允许小工具暂停批量终结点。默认设置取决于USB设备控制器的类型，但通常为真。


除上述外，该小工具还接受由复合框架定义的以下参数(它们是所有复合小工具的通用参数，仅供快速列出)：

* idVendor – USB Vendor ID (16 bit integer)
* idProduct – USB Product ID (16 bit integer)
* bcdDevice – USB Device version (BCD) (16 bit integer)
* iManufacturer – USB Manufacturer string (string)
* iProduct – USB Product string (string)
* iSerialNumber – SerialNumber string (string)

#### 2. 实现脚本

有了以上基础, 我们就可以在脚本中设置自定义的大容量存储设备了. 只需创建一个 function, 这里假定有一个创建好了的 usb 小工具配置 `configs/c.1/`

```shell
# 创建大容量存储功能
mkdir -p functions/mass_storage.usb0/lun.0
echo 0 > functions/mass_storage.usb0/stall
echo 0 > functions/mass_storage.usb0/lun.0/cdrom
echo 0 > functions/mass_storage.usb0/lun.0/ro
echo 0 > functions/mass_storage.usb0/lun.0/nofua
echo /data.img > functions/mass_storage.usb0/lun.0/file

# 关联到小工具配置
ln -s functions/mass_storage.usb0 configs/c.1/
```

不过对于官方给的示例系统, 只需更改 `nv-l4t-usb-device-mode-start.sh` 中 `echo "${fs_img}" > "${func}/lun.0/file"` 即可, 另外， 若需要设备是可读写的, 还需则 `echo 1 > "${func}/lun.0/ro"` 将改成 `echo 0 > "${func}/lun.0/ro"`.

### [2023-02-24 11:47:23]

经过上面的教程, mass_storage 已经能够正确设置和运行了, 把他关联到一个 sd 卡分区上就可以了, 而且因为 nofua=0 的原因, 似乎写入的数据是同步的, 即在 host 端读写文件, 在设备上是可以立刻刷新出来的.不过就是传输时有点慢, 才 6MB/s 左右， 而 RNDIS 通过 scp 传文件可达 30MB. 

按理说这样勉强可用了, 但还是想再研究一下 MTP 模式, 昨晚尝试了 MTP, 发现需要额外的用户空间代码, 但是有开源的 umtprd 可以用. 而且也才注意到 CherryPi 其实使用的就是 umtprd. 不过当我模仿 CherryPi 在 Jetson TX2 上设置了 umtprd时, 挂载总是出问题, host 端无法识别该设备, 不知是模块驱动问题还是用户空间没配置好...

### [2023-02-25 11:41:02]

今天继续调试 MTP, 尝试把官方原来的 usb 功能给禁止, 然后开机后自己创建一个 gadget, 完全照搬 CherryPi 的 mtp 配置, 结果成功了.

那我后面是应该继续使用官方的 usb 启动脚本还是自己写一个类似的 deb 包, 然后往里面添加自己的 systemd service 和脚本? 

感觉要优化系统的话, systemd 肯定还是得熟悉, 那么开新坑吧, 先去学 deb 打包, Startup udev 之类的, 把守护进程这些知识搞清楚再来继续...


```shell

cd /sys/kernel/config/usb_gadget/l4t

# 添加 mtp 功能
mkdir functions/ffs.mtp

mkdir /dev/ffs-umtp
mount -t functionfs mtp /dev/ffs-umtp

# 开启 umtprd
/home/dk/uMTP-Responder-master/umtprd

# 关闭 UDC
echo  > /sys/kernel/config/usb_gadget/l4t/UDC

# 开启 UDC
ls /sys/class/udc/ > /sys/kernel/config/usb_gadget/l4t/UDC
```


尝试创建新配置 

```shell
cd /sys/kernel/config/usb_gadget/l4t

mkdir configs/c.2
mkdir configs/c.2/strings/0x409
echo "Conf 2" > configs/c.2/strings/0x409/configuration
echo 120 > configs/c.2/MaxPower

mkdir functions/ffs.mtp
ln -s functions/ffs.mtp configs/c.2

echo  > /sys/kernel/config/usb_gadget/g1/UDC
ls /sys/class/udc/ > /sys/kernel/config/usb_gadget/g1/UDC
```