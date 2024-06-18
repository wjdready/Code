
# [2023-11-20 12:23:17]

官方 [wiki](https://wiki.luckfox.com/zh/Luckfox-Pico/Luckfox-Pico-quick-start)

## 源码编译

```sh
# 
git clone https://gitee.com/LuckfoxTECH/luckfox-pico.git

./build.sh -h
```

构建脚本的框架类似 AOSP, 最主要的是 project 目录, 包含 build.sh, 工程目录下的 build.sh 只是 project/build.sh 的软链接

首先 `build.sh lunch` 选择板子, 然后 `build.sh [target]` 来选择要构建的目标

project/cfg-all-items-introduction.txt 给出了所有配置项

根据 build.sh 中

```sh
RK_TARGET_BOARD_ARRAY=( $(cd ${TARGET_PRODUCT_DIR}/; ls BoardConfig*.mk BoardConfig_*/BoardConfig*.mk | sort) )
```

可知每个板子的配置都在 cfg 下, 必须以 BoardConfig 为前缀

## 烧录

利用 RK 官方工具 SocToolKit 将 uboot 烧写到 flash，然后采用原始方式从 SD 卡加载内核

```sh
fatload mmc 1:1 0x21000000 Image
fatload mmc 0:1 0x22000000 infinity2m-spinand-ssc011a-s01a-display.dtb
setenv bootargs "console=ttyS0,115200 root=/dev/mmcblk0p2 rw"
bootm 0x21000000 - 0x22000000

# 上面直接加载怎么都不行，kernel 总是没法解析 root (显示为 null), 只有设置后 saveenv 才行，真是日狗
setenv bootcmd 'fatload mmc 0:1 0x21000000 uImage;fatload mmc 0:1 0x22000000 infinity2m-spinand-ssc011a-s01a-display.dtb;bootm 0x21000000 - 0x22000000;'
setenv sys_bootargs 'root=/dev/mmcblk1p2 rootfstype=ext4 rw console=ttyFIQ0,115200'
```

## 调试音频

参考:
https://www.jianshu.com/p/3fdaf77f29fa
simple-card 设备树配置文档: https://www.kernel.org/doc/Documentation/devicetree/bindings/sound/simple-card.txt
这一篇对于调试过程给了不错的启发: https://www.cnblogs.com/zyly/p/17591417.html

```sh
# 录制5s,通道数为2, 采样率为16000, 采样精度为16bit, 保存为wav文件
arecord  -f S32_LE -r 48000 -c 2 -d 25 test.wav

# 播放音频
aplay test.wav

# 检查设备树
sudo dtc -I dtb -O dts /sys/firmware/fdt | grep dummy-codec -C 20
```

```sh
mount -t debugfs none /sys/kernel/debug
cd /sys/kernel/debug/pinctrl/pinctrl-rockchip-pinctrl
group: i2s0-pins
pin 65 (gpio2-1) LRCK
pin 66 (gpio2-2) MCLK
pin 64 (gpio2-0) SCLK
pin 69 (gpio2-5) SDI0
pin 68 (gpio2-4) SDO0
pin 71 (gpio2-7) SDI3
pin 70 (gpio2-6) SDI2
pin 67 (gpio2-3) SDI1
```
