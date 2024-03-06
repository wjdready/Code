
# [2023-12-06 10:33:08]

官方 wiki: https://wiki.t-firefly.com/zh_CN/Core-3399-JD4/started.html

资料下载:
Core-3399-JD4 V1: https://www.t-firefly.com/doc/download/page/id/66.html
Core-3399-JD4 V2: https://www.t-firefly.com/doc/download/page/id/92.html

DriverAssitant 是 usb 驱动, RKDevTool (瑞芯微开发工具) 是烧录工具

> SocToolKit (瑞芯微工具套件) 是另一个烧录工具，适用于 RV1106 等 Soc

RKDevTool 支持多种烧录模式 (使用 USB3.0 口)

但最基本的还是 `Maskrom` 模式, 其他模式都在一定程度上需要事先运行一段加载程序。在核心板 EMMC 位置处有两个触点，上电时短接这两个触点进入 Maskrom 模式


## 构建

根据官方提供的 linux-sdk 解压后执行 build.sh

FATAL ERROR: Unable to parse input tree

    u-boot/scripts/Makefile.lib 修改 cmd_dtc 中的 `echo '\#include ` 为 `echo '#include `

kernel/scripts/gcc-wrapper.py
    
    注释掉 interpret_warning

一些依赖

    sudo apt install expect liblz4-tool

## 烧录

启动流程

参考
https://www.cnblogs.com/zyly/p/17380243.html

上电后先启动固化在芯片内部的 BootROM，BootROM 识别 BOOT 配置引脚并从指定设备加载 FSBL 到内部的 192KB SRAM 中，该 FSBL 再通过初始化 DRAM 来将 u-boot 加载到 DRAM

#### 烧写原理

空板子的情况下处于 MaskRom 需要先下载一个由官方提供的 loader，该 loader 首先被加载到 DRAM 并运行， 然后烧录软件再通过和该 loader 进行通信来完成 emmc 固件烧录

emmc 固件一般也都存在该 loader，下次烧录可按住 recovery 键进入 loader 模式即可

