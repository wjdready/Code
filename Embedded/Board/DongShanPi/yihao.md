

# [2023-10-21 10:40:57]

[板子信息地址](https://dongshanpi.com/DongshanPi-One/03-QuickStart/)

[原理图](https://cowtransfer.com/s/9b745ce262544c)

上面有个拨码开关，用于设置是否只进入调试模式, 其实它调试模式默认总是连起来的.


官方的做法是总是从 nand flash 中启动 uboot, 并且烧录 uboot 需要特殊工具。

不过既然已经默认有 uboot, 其实不必纠结，直接使用 uboot 引导我们的 kernel 就可以了，直奔主题

# 源码编译

```sh
# 获取代码
git clone https://e.coding.net/codebug8/repo.git
mkdir DongshanPiOne-TAKOYAKI  && cd  DongshanPiOne-TAKOYAKI
../repo/repo init -u  https://gitee.com/weidongshan/manifests.git -b linux-sdk -m  SSD202D/dongshanpi-one_takoyaki_dlc00v030.xml --no-repo-verify
../repo/repo sync -j4

# 源码信息
tree -L 1
# .
# ├── boot                //使用的是uboot-2015  主要用于烧写引导kernel启动。
# ├── DevelopmentEnvConf  //自动配置开发环境的一些脚本文件
# ├── gcc-arm-8.2-2018.08-x86_64-arm-linux-gnueabihf   //交叉编译工具链。
# ├── kernel             //使用4.9.88版本的内核
# ├── project            //根文件系统和应用相关源码,主要包含了 编译构建根文件系统 APP 并自动打包生成合适的格式用来烧写。
# └── sdk  

# 编译 kernel
cd kernel
export ARCH=arm
export CROSS_COMPILE=arm-linux-gnueabihf-
make infinity2m_spinand_ssc011a_s01a_minigui_defconfig
make -j8

# 得到设备树和内核镜像
# arch/arm/boot/dts/infinity2m-spinand-ssc011a-s01a-display.dtb
# arch/arm/boot/uImage
```

# 系统加载

```sh
fatload mmc 0:1 0x21000000 uImage
fatload mmc 0:1 0x22000000 infinity2m-spinand-ssc011a-s01a-display.dtb
setenv bootargs "console=ttyS0,115200 root=/dev/mmcblk0p2 rw"
bootm 0x21000000 - 0x22000000

# 上面直接加载怎么都不行，kernel 总是没法解析 root (显示为 null), 只有设置后 saveenv 才行，真是日狗
setenv bootcmd 'fatload mmc 0:1 0x21000000 uImage;fatload mmc 0:1 0x22000000 infinity2m-spinand-ssc011a-s01a-display.dtb;bootm 0x21000000 - 0x22000000;'
setenv bootargs 'root=/dev/mmcblk0p2 rw console=ttyS0,115200'
```
