

开发文档:
http://www.orangepi.cn/html/hardWare/computerAndMicrocontrollers/service-and-support/Orange-Pi-3B.html

# [2023-12-06 09:52:02]

## Linux 

对于 linux 开发 orangepi 官方的 build 项目倒是很省心方便, 直接运行 ./build.sh 会进入 menuconfig, 然后可以选择下载相应的开发包和工具链。

```sh
git clone https://github.com/orangepi-xunlong/orangepi-build
./build.sh
# 选完之后会自动编译
```

u-boot 编译完成后会自动打包成一个 `rkspi_loader.img` 可用于通过 RKDevTool (瑞芯微开发工具) 下载

而 kernel 编译完成后自动打包成 deb 包，反而没有将 zImage 放出，因此目前可以通过 `dpkg -x package.deb directory` 来解压以得到 kernel 和 dtb。
同时可以看到解压后包括 kernel 和驱动在内所有东西。

## 烧录

首先到 `官方工具` 中下载烧录工具和驱动，DriverAssitant 是 usb 驱动, RKDevTool (瑞芯微开发工具) 是烧录工具。同时还要下载事先编译好的 `loader` 文件。

根据 orangepi 用户手册中的提示和教程，得出下载流程和原理如下:

RKDevTool 首先需要下载 `MiniLoaderAll.bin`, 它会负责和 RKDevTool 进行通信，里面包含多种对外部存储器的访问和读写的驱动。随后 RKDevTool 利用该 loader 向芯片下载镜像文件到指定的外部存储器，从而完成下载。

测试中我们主要是想下载 u-boot 到板子上的 spi-nor-flash, 然后再在 u-boot 中灵活的加载 kernel 和 rootfs

那么打开 RKDevTool 第一栏设置 loader 加载 `MiniLoaderAll.bin`, 第二栏加载上面编译好的 u-boot: `rkspi_loader.img`, 并把存储类型改为 `SPINOR`。

最后**强制按地址**写打上勾，然后点击执行即可.

## 系统加载

这些地址可通过 printenv 中打印出的变量 `fdt_addr_r`, `kernel_addr_r`, `ramdisk_addr_r` 得知

```sh
load mmc 0:1 ${kernel_addr_r} Image
load mmc 0:1 ${ramdisk_addr_r} uInitrd
load mmc 0:1 ${fdt_addr_r} dtb/rockchip/rk3566-orangepi-3b.dtb

setenv bootargs "console=ttyS2,1500000 root=/dev/mmcblk0p2 rw"
booti ${kernel_addr_r} ${ramdisk_addr_r} ${fdt_addr_r}

setenv bootcmd 'fatload mmc 0:1 0x21000000 uImage;fatload mmc 0:1 0x22000000 infinity2m-spinand-ssc011a-s01a-display.dtb;bootm 0x21000000 - 0x22000000;'
setenv bootargs 'root=/dev/mmcblk0p2 rw console=ttyS0,115200'
```


## u-boot 探究

首先执行 print 打印所有环境变量, 将其复制到 VSCode 中的某个 `.sh` 文件, 执行替换 `^(.+=)(.*)\n` --> `$1\n$2\n\n`, 然后格式化文档得到:

```sh
arch=
arm

autoload=
no

baudrate=
1500000

board=
evb_rk3568

board_name=
evb_rk3568

boot_a_script=
load ${devtype} ${devnum}:${distro_bootpart} ${scriptaddr} ${prefix}${script}
source ${scriptaddr}

boot_extlinux=
sysboot ${devtype} ${devnum}:${distro_bootpart} any ${scriptaddr} ${prefix}extlinux/extlinux.conf

boot_net_pci_enum=
pci enum

boot_net_usb_start=
usb start

boot_prefixes=
/ /boot/

boot_script_dhcp=
boot.scr.uimg

boot_scripts=
boot.scr.uimg boot.scr

boot_targets=
mmc0 mmc1 nvme mtd2 mtd1 mtd0 usb0 pxe dhcp

bootargs=storagemedia=mtd androidboot.storagemedia=mtd androidboot.mode=
normal

bootcmd=
run distro_bootcmd
boot_android ${devtype} ${devnum}
boot_fit
bootrkp

bootcmd_dhcp=
run boot_net_usb_start
run boot_net_pci_enum
if dhcp ${scriptaddr} ${boot_script_dhcp}; then source ${scriptaddr}; fi

bootcmd_mmc0=
if mmc dev 0; then
    mmc list
    setenv devnum 0
    run mmc_boot
else
    mmc list
    setenv devnum 1
    run mmc_boot
fi

bootcmd_mtd0=
setenv devnum 0
run mtd_boot

bootcmd_mtd1=
setenv devnum 1
run mtd_boot

bootcmd_mtd2=
setenv devnum 2
run mtd_boot

bootcmd_nvme=
pci enum
nvme scan
setenv devnum 0
run nvme_boot

bootcmd_pxe=
run boot_net_usb_start
run boot_net_pci_enum
dhcp
if pxe get; then pxe boot; fi

bootcmd_usb0=
setenv devnum 0
run usb_boot

bootdelay=
0

cpu=
armv8

devnum=
0

devtype=
mtd

distro_bootcmd=
for target in ${boot_targets}; do run bootcmd_${target}; done

eth1addr=
22:b9:9b:83:81:9f

ethaddr=
1e:b9:9b:83:81:9f

fdt_addr_r=
0x08300000

fdtoverlay_addr_r=
0x08200000

kernel_addr_c=
0x04080000

kernel_addr_r=
0x00280000

mmc_boot=
if mmc dev ${devnum}; then
    setenv devtype mmc
    run scan_dev_for_boot_part
fi

nvme_boot=
if nvme dev ${devnum}; then
    setenv devtype nvme
    run scan_dev_for_boot_part
fi

partitions=uuid_disk=${uuid_gpt_disk}
name=uboot,start=8MB,size=4MB,uuid=${uuid_gpt_loader2}
name=trust,size=4M,uuid=${uuid_gpt_atf}
name=misc,size=4MB,uuid=${uuid_gpt_misc}
name=resource,size=16MB,uuid=${uuid_gpt_resource}
name=kernel,size=32M,uuid=${uuid_gpt_kernel}
name=boot,size=32M,bootable,uuid=${uuid_gpt_boot}
name=recovery,size=32M,uuid=${uuid_gpt_recovery}
name=backup,size=112M,uuid=${uuid_gpt_backup}
name=cache,size=512M,uuid=${uuid_gpt_cache}
name=system,size=2048M,uuid=${uuid_gpt_system}
name=metadata,size=16M,uuid=${uuid_gpt_metadata}
name=vendor,size=32M,uuid=${uuid_gpt_vendor}
name=oem,size=32M,uuid=${uuid_gpt_oem}
name=frp,size=512K,uuid=${uuid_gpt_frp}
name=security,size=2M,uuid=${uuid_gpt_security}
name=userdata,size=-,uuid=
${uuid_gpt_userdata}

pxefile_addr_r=
0x00e00000

ramdisk_addr_r=
0x0a200000

rkimg_bootdev=
if nvme dev 0; then
    setenv devtype nvme
    setenv devnum 0
    echo Boot from nvme
elif mmc dev 1 && rkimgtest mmc 1; then
    setenv devtype mmc
    setenv devnum 1
    echo Boot from SDcard
elif mmc dev 0; then
    setenv devtype mmc
    setenv devnum 0
elif mtd_blk dev 0; then
    setenv devtype mtd
    setenv devnum 0
elif mtd_blk dev 1; then
    setenv devtype mtd
    setenv devnum 1
elif mtd_blk dev 2; then
    setenv devtype mtd
    setenv devnum 2
elif rknand dev 0; then
    setenv devtype rknand
    setenv devnum 0
elif rksfc dev 0; then
    setenv devtype spinand
    setenv devnum 0
elif rksfc dev 1; then
    setenv devtype spinor
    setenv devnum 1
    elsesetenv devtype ramdisk
    setenv devnum 0
fi

rkimg_bootdev_download=
scsi scan
nvme scan
if mmc dev 1; then
    setenv devtype mmc
    setenv devnum 1
elif mmc dev 0; then
    setenv devtype mmc
    setenv devnum 0
elif nvme dev 0; then
    setenv devtype nvme
    setenv devnum 0
elif scsi dev 0; then
    setenv devtype scsi
    setenv devnum 0
fi

scan_dev_for_boot=
echo Scanning ${devtype} ${devnum}:${distro_bootpart}...
for prefix in ${boot_prefixes}; do
    run scan_dev_for_extlinux
    run scan_dev_for_scripts
done

scan_dev_for_boot_part=
part list ${devtype} ${devnum} -bootable devplist
env exists devplist || setenv devplist 1
for distro_bootpart in ${devplist}; do
    if fstype ${devtype} ${devnum}:${distro_bootpart} bootfstype; then
        run scan_dev_for_boot
    fi
done

scan_dev_for_extlinux=
if test -e ${devtype} ${devnum}:${distro_bootpart} ${prefix}extlinux/extlinux.conf; then
    echo Found ${prefix}extlinux/extlinux.conf
    run boot_extlinux
    echo SCRIPT FAILED: continuing...
fi

scan_dev_for_scripts=
for script in ${boot_scripts}; do
    if test -e ${devtype} ${devnum}:${distro_bootpart} ${prefix}${script}; then
        echo Found U-Boot script ${prefix}${script}
        run boot_a_script
        echo SCRIPT FAILED: continuing...
    fi; 
done

scriptaddr=
0x00c00000

soc=
rockchip

stderr=
serial,vidconsole

stdin=
serial,usbkbd

stdout=
serial,vidconsole

usb_boot=
usb start
if usb dev ${devnum}; then
    setenv devtype usb
    run scan_dev_for_boot_part
fi

vendor=rockchip
```

从 boot_targets 可以看出支持从多个设备中启动，其顺序为 `mmc0 mmc1 nvme mtd2 mtd1 mtd0 usb0 pxe dhcp`。 

## 从 mmc0 启动

mmc0 为 sd 卡, 假设启动第一个设备即成功时，其启动流程为:

```sh

# 1. bootcmd_mmc0 (提供 devnum = 0)
# 2. mmc_boot  (提供 devtype = mmc)
# 3. scan_dev_for_boot_part (使用变量 devtype, devnum), (提供 distro_bootpart, bootfstype = fat)
    # 获取指定设备的所有分区, 并检查分区是否存在 u-boot 支持的文件系统
    part list ${devtype} ${devnum} -bootable devplist
    env exists devplist || setenv devplist 1
    for distro_bootpart in ${devplist}; do
        if fstype ${devtype} ${devnum}:${distro_bootpart} bootfstype; then
            run scan_dev_for_boot
        fi
    done

# 4. scan_dev_for_boot (使用变量 boot_prefixes) 提供 (prefix)
    # 对 boot_prefixes 所指定的每个目录进行两种启动方式的扫描
    echo Scanning ${devtype} ${devnum}:${distro_bootpart}...
    for prefix in ${boot_prefixes}; do
        run scan_dev_for_extlinux
        run scan_dev_for_scripts
    done

# 5.1 scan_dev_for_extlinux (使用变量 devtype, devnum, distro_bootpart, prefix)
    # 如果该设备该分区该目录下存在 extlinux/extlinux.conf 文件, 则执行启动 boot_extlinux
    if test -e ${devtype} ${devnum}:${distro_bootpart} ${prefix}extlinux/extlinux.conf; then
        echo Found ${prefix}extlinux/extlinux.conf
        run boot_extlinux
        echo SCRIPT FAILED: continuing...
    fi

# 5.2 scan_dev_for_scripts (当 scan_dev_for_extlinux 启动失败时执行) (使用变量 devtype, devnum, distro_bootpart, prefix) 
    # (提供 script)
    # 检测是否存在 boot_scripts 列出的文件名, 存在则执行 boot_a_script
    for script in ${boot_scripts}; do
        if test -e ${devtype} ${devnum}:${distro_bootpart} ${prefix}${script}; then
            echo Found U-Boot script ${prefix}${script}
            run boot_a_script
            echo SCRIPT FAILED: continuing...
        fi; 
    done

#6. boot_a_script (使用变量 devtype, devnum, distro_bootpart, scriptaddr, prefix, script)
    # 已经正确获取所有必须变量，现在只需利用这些变量，加载指定的脚本到内存并执行
    load ${devtype} ${devnum}:${distro_bootpart} ${scriptaddr} ${prefix}${script}
    source ${scriptaddr}
```

说白了就是扫描各个存储器的所有分区中的文件系统，识别 / 或 /boot 目录下是否存在 `extlinux/extlinux.conf` 或 `boot.scr.uimg` `boot.scr` 脚本文件，存在则加载并执行

## initfs

https://wiki.gentoo.org/wiki/Custom_Initramfs#:~:text=initramfs%20is%20for%20users%20with,or%20otherwise%20special%20root%20partition


# 2024-03-15 13:06:25

## 基于官方框架编译


首先 ./build.sh 使用 GUI 方式进行配置，编译完成后会提示刚才的配置等效的命令行编译:

```sh

sudo ./build.sh  BOARD=orangepi3b BRANCH=current BUILD_OPT=kernel KERNEL_CONFIGURE=no

# 另外还有一些附加的选项:
OFFLINE_WORK=yes    # 离线编译

```

## 适配 SPI 屏幕

#### fbtft

官方 Image 已经把 tinydrm 和 fbtft fbtft 都编译进去了，所以可以直接使用，只需添加 overlay 的设备树即可。

dtc -@ fbtft_lcd.dts -o fbtft_lcd.dtob
sudo cp fbtft_lcd.dtob /boot/dtb/rockchip/overlay/rk356x-fbtft.dtbo

还需改 /boot/orangepiEnv.txt 添加 overlays=fbtft

export QT_QPA_PLATFORM=linuxfb:tty=/dev/fb0
/usr/lib/aarch64-linux-gnu/qt5/examples/widgets/animation/moveblocks/moveblocks

# [2024-10-10 16:07:39]

发现 /boot/orangepiEnv.txt 添加 raspi-7inch-touchscreen 但是不接 mipi 屏时， fbtft 就可以正常显示屏幕内容了

overlays=fbtft raspi-7inch-touchscreen


```c
/dts-v1/;
/plugin/;
/ {
    compatible = "rockchip,rk3566-orangepi-3b";

    fragment@0 {
        target = <&spi3>;
        __overlay__ {
            /* needed to avoid dtc warning */
            #address-cells = <1>;
            #size-cells = <0>;
            status = "okay";
            pinctrl-names = "default";

            ili9341@0 {
                compatible = "ilitek,ili9341";
                reg = <0>;
                spi-max-frequency = <50000000>;
                rotate = <270>;
                bgr;
                fps = <30>;
                buswidth = <8>;
                // reset = <&gpio2 5 GPIO_ACTIVE_LOW>;
                // dc = <&gpio2 4 GPIO_ACTIVE_LOW>;
                reset-gpios = <&gpio1 0 1>;
                dc-gpios = <&gpio4 5 0>;
                led-gpios = <&gpio3 28 0>;
                debug = <1>;
            };

            // st7789v@0{
            //     compatible = "sitronix,st7789v";
            //     reg = <0>;
            //     spi-max-frequency = <50000000>;
            //     buswidth = <8>;
            //     debug = <0>;
            //     height = <240>;
            //     width = <240>;
            //     fps = <30>;
            //     reset-gpios = <&gpio1 0 1>;
            //     dc-gpios = <&gpio4 5 0>;
            //     led-gpios = <&gpio3 28 0>;
            //     rotate = <0>;
            //     spi-cpol;
            //     spi-cpha;
            // };

            // ssd1306@0 {
            //     compatible = "solomon,ssd1306";
            //     reg = <0>;
            //     spi-max-frequency = <1000000>;
            //     buswidth = <8>;
            //     debug = <1>;
            //     height = <64>;
            //     width = <128>;
            //     fps = <30>;
            //     reset-gpios = <&gpio1 RK_PC4 1>;
            //     dc-gpios = <&gpio1 RK_PC5 0>;
            //     led-gpios = <&gpio1 RK_PD0 0>;
            //     rotate = <0>;
            //     bgr;
            // };
        };
    };
};
```

st7789v 卡在 spi 的 cpol 和 cpha 没设置，后来参考了以前的代码才设置成功。

#### tinydrm

```sh
modinfo ili9341
cat /sys/kernel/debug/device_component/display-subsystem
```

tinydrm 和 panel 是一样的，两个效果一样，现在的问题在于驱动加载后 modetest 无法显示 connectors 和 modes 信息

也不知道怎么路由系统界面到 lcd 屏幕上，下面设备树 port 节点写了但是没有用，只是作为参考，去掉和加上区别不大。

不过值得一提的是我的 sdl-shader-toy 代码编译后能跑，但是不知道 opengl 调用是软件模拟还是硬件加速

```c
/dts-v1/;
/plugin/;
/ {
    compatible = "rockchip,rk3566-orangepi-3b";

    fragment@0 {
        target = <&spi3>;
        __overlay__ {
            /* needed to avoid dtc warning */
            #address-cells = <1>;
            #size-cells = <0>;
            status = "okay";
            pinctrl-names = "default";

            // ====> panel
            display@0{
                status = "okay";
                compatible = "adafruit,yx240qv29";
                reg = <0>;
                spi-max-frequency = <50000000>;
                dc-gpios = <&gpio4 5 0>;
                reset-gpios = <&gpio1 0 0>;
                rotation = <0>;

                port {
                    panel_in: endpoint {
                        remote-endpoint = <&vp0_out_spi>;
                        status = "okay";
                    };
                };
            };
        };
    };

    fragment@1 {
        target = <&hdmi>;
        __overlay__ {
            status = "disabled";
        };
    };

    fragment@2 {
        target = <&vop>;
        __overlay__ {
            status = "okay";
            ports {
                #address-cells = <1>;
                #size-cells = <0>;
                port@1 {
                    reg = <1>;
                    vp0_out_spi: endpoint {
                        remote-endpoint = <&panel_in>;
                        status = "okay";
                    };
                };
            };
        };
    };
};

// sudo cp fbtft_lcd.dtob /boot/dtb/rockchip/overlay/rk356x-fbtft.dtbo
// dtc -I fs -O dts /sys/firmware/devicetree/base > ~/curent_device_tree.txt && vim ~/curent_device_tree.txt
```

#### panel


怎么关闭控制台: https://www.cnblogs.com/yddeboke/p/15246900.html


## VTCONSOLE 

很关键的一点是 vtconsole 默认就加载并绑定到 tty0, 然后似乎屏幕默认就是作为 vtconsole

```sh
# 查看 vtconsole 设备名称, 显示 frame buffer
cat /sys/class/vtconsole/vtcon1/name
cat /proc/fb
# 取消绑定到 tty 
echo 0 > /sys/class/vtconsole/vtcon1/bind
```
